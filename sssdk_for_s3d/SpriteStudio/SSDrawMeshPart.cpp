
#include "SSDrawMeshPart.hpp"
#include "SSCell.hpp"
#include "SSCellMap.hpp"

namespace
{
	// nが2のべき乗かどうかチェックする
	inline bool IsPow2(int32 n)
	{
		for (int i = 0; i < 16; i++)
		{
			if (n == (1 << i)) return true;
		}
		return false;
	}

	bool TextureIsPow2(const Texture& tex)
	{
		if (not IsPow2(tex.width())) return false;
		if (not IsPow2(tex.height())) return false;
		return true;
	}
}

namespace sssdk
{
	SSDrawMeshPart::SSDrawMeshPart(StringView ssae, const SSAnimationPart* anim, const ISSCellmaps* cellmaps)
		: SSDrawCellPart{ ssae, anim, cellmaps }
		, m_pTargetTexture{ nullptr }
		, m_pMesh2D{ nullptr }
		, m_offsetWorldVertices{}
		, m_vertices{}
		, m_weightColors{}
		, m_bindBoneInfomations{}
	{
		makeMesh();
	}

	SSDrawMeshPart::~SSDrawMeshPart()
	{
	}

	void SSDrawMeshPart::draw() const
	{
		// TODO
		SSDrawCellPart::draw();
	}

	size_t SSDrawMeshPart::getVertexNum() const
	{
		if (m_pMesh2D == nullptr)
		{
			return 0;
		}
		return m_pMesh2D->vertices.size();
	}

	Array<SSBoneWeight>& SSDrawMeshPart::getBindBoneInfomations()
	{
		return m_bindBoneInfomations;
	}

	void SSDrawMeshPart::makeMesh()
	{
		if (m_pCell == nullptr)
		{
			return;
		}
		if (m_texture.isEmpty())
		{
			return;
		}

		m_offsetWorldVertices.clear();
		m_vertices.clear();
		m_weightColors.clear();
		m_bindBoneInfomations.clear();

		const size_t meshPointCount = m_pCell->getMeshPoints().size();
		Array<Vertex2D> drawVertices(meshPointCount);

		m_offsetWorldVertices.resize(meshPointCount);
		m_vertices.resize(meshPointCount);
		m_weightColors.resize(meshPointCount);
		m_bindBoneInfomations.resize(meshPointCount);

		for (auto& bindBoneInfo : m_bindBoneInfomations)
		{
			bindBoneInfo.Cleanup();
		}

		const auto& cellSize = m_pCell->getSize();
		Float2 offs; // 中央
		offs.x = static_cast<float>(-cellSize.x * 0.5f);
		offs.y = static_cast<float>(cellSize.y * 0.5f);

		offs.x -= static_cast<float>(m_pCell->getPivot().x) * cellSize.x;
		offs.x -= static_cast<float>(m_pCell->getPivot().y) * cellSize.y;

		const float textureWidth = static_cast<float>(m_texture.width());
		const float textureHeight = static_cast<float>(m_texture.height());

		float uvPixelX = 1.0f / textureWidth;
		float uvPixelY = 1.0f / textureHeight;

		//べき乗でないテクスチャはUVをピクセルにしなくてはならないらしい
		if (not TextureIsPow2(m_texture))
		{
			uvPixelX = 1.0f;
			uvPixelY = 1.0f;
		}

		for (size_t i = 0; i < meshPointCount; i++)
		{
			const Point& v = m_pCell->getMeshPoints().at(i);
			m_vertices[i].x = v.x + offs.x;
			m_vertices[i].y = -v.y + offs.y;
			m_offsetWorldVertices[i] = Float2::Zero();

			drawVertices[i].color = ColorF::One().toFloat4();
			drawVertices[i].tex.x = (m_pCell->getPos().x + v.x) * uvPixelX;
			drawVertices[i].tex.y = (m_pCell->getPos().y + v.y) * uvPixelY;

			drawVertices[i].pos = m_vertices[i];
		}

		const size_t indexCount = m_pCell->getMeshTriangleIndices().size();
		Array<TriangleIndex> indices(indexCount);
		for (size_t i = 0; i < indexCount; i++)
		{
			const auto& t = m_pCell->getMeshTriangleIndices().at(i);
			indices[i].i0 = static_cast<uint16>(t.i0);
			indices[i].i1 = static_cast<uint16>(t.i1);
			indices[i].i2 = static_cast<uint16>(t.i2);
		}

		m_pMesh2D = std::make_unique<Buffer2D>(drawVertices, indices);
	}

	void SSDrawMeshPart::updateTransformMesh()
	{
		auto& drawVertices = m_pMesh2D->vertices;
		const size_t vertexCount = m_vertices.size();
		for (size_t i = 0; i < vertexCount; i++)
		{
			SSBoneWeight& boneWeightInfo = m_bindBoneInfomations[i];
			Float2 out;
			Float2 outTotal = Float2::Zero();

			SSDrawPart* pState = this;

			//デフォームアトリビュートを使用している
			if (m_isUsedDeform)
			{
				// キーからデフォームアトリビュートを取り出す
				Float2 offset = Float2::Zero();
				const size_t size1 = m_deform.vchgs.size();
				const size_t size2 = vertexCount;
				if (size1 == size2)
				{
					offset = getOffsetWorldVerticesFromKey(i);
				}
				// 描画用デフォームアトリビュートを更新
				m_offsetWorldVertices[i] = offset;
			}

			if (boneWeightInfo.getBoneNum() == 0)
			{
				// バインドされていないメッシュの場合
				// デフォームオフセットを加える
				if (m_isUsedDeform)
				{
					drawVertices[i].pos = m_vertices[i] + getOffsetLocalVertices(i);
				}
			}
			else
			{
				const int32 bindBoneNum = boneWeightInfo.getBoneNum();
				for (int32 n = 0; n < bindBoneNum; n++)
				{
					auto* pBonePart = boneWeightInfo.getBonePart(n);
					if (pBonePart != nullptr)
					{
						if (bindBoneNum > 0)
						{
							pState = pBonePart;
						}
						const float w = boneWeightInfo.getWeight(n) / 100.0f;
						out = pState->getWorldMatrix().transformPoint(boneWeightInfo.getOffset(n)).xy();
						out *= w;
						outTotal += out;
					}
				}
				// デフォームオフセットを加える
				if (m_isUsedDeform)
				{
					// ボーンにより影響を受けた座標(ローカル座標)
					Mat4x4 invMat = m_worldMatrix.inverse();
					Float3 out2 = invMat.transformPoint(Float3{ outTotal, 0.0f });

					// デフォームによる影響(ローカル座標)
					Float2 vec = getOffsetLocalVertices(i);
					outTotal = out2.xy() + vec;

					// ワールド座標に変換
					out2 = m_worldMatrix.transformPoint(Float3{ outTotal, 0.0f });
					outTotal = out2.xy();
				}

				drawVertices[i].pos = outTotal;
			}
		}
	}

	// デフォームアトリビュート
	// ワールド座標を取得
	Float2 SSDrawMeshPart::getOffsetWorldVerticesFromKey(size_t index)
	{
		Float3 out1, out2;
		{
			Mat4x4 mat = m_worldMatrix;
			Float3 trans{
				 m_vertices[index].x + m_deform.vchgs[index].point.x,
				 m_vertices[index].y + m_deform.vchgs[index].point.y,
				 0.0f
			};
			out1 = mat.transformPoint(trans);
		}
		{
			Mat4x4 mat = m_worldMatrix;
			Float3 trans{
				 m_vertices[index].x,
				 m_vertices[index].y,
				 0.0f
			};
			out2 = mat.transformPoint(trans);
		}

		return Float2{
			out1.x - out2.x,
			out1.y - out2.y
		};
	}

	// デフォームアトリビュート取得
	// ローカル座標系を取得（バインドがない場合）
	Float2 SSDrawMeshPart::getOffsetLocalVertices(size_t index)
	{
		Float3 out1, out2;
		{
			Mat4x4 mat = m_worldMatrix;
			Float3 trans{
				m_vertices[index],
				0.0f
			};
			out1 = mat.transformPoint(trans);
		}
		{
			Mat4x4 mat = m_worldMatrix;
			mat = mat.inverse();
			Float3 trans{
				m_vertices[index] + m_offsetWorldVertices[index],
				0.0f
			};
			out2 = mat.transformPoint(trans);
		}
		return Float2{
			out2.x - m_vertices[index].x,
			out2.y - m_vertices[index].y
		};
	}

}
