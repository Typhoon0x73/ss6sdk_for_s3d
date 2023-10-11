#include <stdio.h>
#include <cstdlib>

#include "../Loader/ssloader.h"

#include "ssplayer_animedecode.h"
#include "ssplayer_mesh.h"
#include "ssplayer_macro.h"
#include "ssplayer_matrix.h"
// #include "ssplayer_animedecode.h"

namespace spritestudio6
{
	// nが2のべき乗かどうかチェックする
	inline bool SsUtTextureisPow2(int32 n)
	{
		for (int i = 0; i < 16; i++)
		{
			if (n == (1 << i)) return true;
		}
		return false;
	}

	bool isPow2(int32 w, int32 h)
	{
		if (not SsUtTextureisPow2(w)) return false;
		if (not SsUtTextureisPow2(h)) return false;
		return true;
	}

void	SsMeshPart::makeMesh()
{
	//パーツステートの初期化の際にターゲットセルが作られる、その際にマップもコピーする？
	if (targetCell == 0)return;
	if (targetTexture.isEmpty()) return;
	size_t psize = targetCell->meshPointList.size();

	buffer2D.reset();
	vertices.reset();
	//colors.reset();
	//uvs.reset();
	//indices.reset();
	//draw_vertices.reset();
	update_vertices_outer.reset();
	vertices_outer.reset();
	bindBoneInfo.reset();
	//weightColors.reset();
	offset_world_vertices.reset();

	//draw_vertices.reset( new std::vector<float>(3 * psize) );
	offset_world_vertices.reset( new std::vector<float>(3 * psize) );

	vertices_outer.reset( new std::vector<SsVector2>(3 * psize) );// //ツール用
	update_vertices_outer.reset( new std::vector<SsVector2>(3 * psize) );// //ツール用

	buffer2D.reset(new Buffer2D());
	buffer2D->vertices.resize(psize);
	vertices.reset( new std::vector<float>(3 * psize) );
	//colors.reset( new std::vector<float>(4 * psize) );
	//uvs.reset( new std::vector<float>(2 * psize) );
	//weightColors.reset( new std::vector<float>(4 * psize) );

	bindBoneInfo.reset( new std::vector<StBoneWeight>(psize) );
	std::vector<StBoneWeight>& bindBoneInfoRaw = *(bindBoneInfo.get());
	for(int i=0; i<psize; i++)
	{
		bindBoneInfoRaw[i].Cleanup();
	}

	SsVector2 offs; //中央
	offs.x = (-targetCell->size.x / 2.0f);
	offs.y = (targetCell->size.y / 2.0f);

	offs.x -= targetCell->pivot.x * targetCell->size.x;
	offs.y -= targetCell->pivot.y * targetCell->size.y;

	ver_size = (int)(targetCell->meshPointList.size());

	float txsizew = (float)(this->targetTexture.width());
	float txsizeh = (float)(this->targetTexture.height());

	float uvpixel_x = 1.0f / txsizew;
	float uvpixel_y = 1.0f / txsizeh;
	
	//べき乗でないテクスチャはUVをピクセルにしなくてはならない
	if (isPow2(this->targetTexture.width(), (this->targetTexture.height())) == false)
	{
		uvpixel_x = 1.0f;
		uvpixel_y = 1.0f;
	}

	std::vector<float>&	verticesRaw = *(vertices.get());
	//std::vector<float>&	colorsRaw = *(colors.get());
	//std::vector<float>&	weightColorsRaw = *(weightColors.get());
	//std::vector<float>&	uvsRaw = *(uvs.get());
	//std::vector<float>&	draw_verticesRaw = *(draw_vertices.get());
	std::vector<float>&	offset_world_verticesRaw = *(offset_world_vertices.get());
	for (size_t i = 0; i < targetCell->meshPointList.size(); i++)
	{
		SsVector2& v = targetCell->meshPointList[i];
		verticesRaw[i * 3 + 0] = v.x + offs.x;
		verticesRaw[i * 3 + 1] = -v.y + offs.y;
		verticesRaw[i * 3 + 2] = 0;
		offset_world_verticesRaw[i * 3 + 0] = 0;
		offset_world_verticesRaw[i * 3 + 1] = 0;
		offset_world_verticesRaw[i * 3 + 2] = 0;

		buffer2D->vertices[i].color.x = 1.0f;
		buffer2D->vertices[i].color.y = 1.0f;
		buffer2D->vertices[i].color.z = 1.0f;
		buffer2D->vertices[i].color.w = 1.0f;
		//colorsRaw[i * 4 + 0] = 1.0f;
		//colorsRaw[i * 4 + 1] = 1.0f;
		//colorsRaw[i * 4 + 2] = 1.0f;
		//colorsRaw[i * 4 + 3] = 1.0f;
		buffer2D->vertices[i].tex.x = (targetCell->pos.x + v.x) * uvpixel_x;
		buffer2D->vertices[i].tex.y = (targetCell->pos.y + v.y) * uvpixel_y;
		//uvsRaw[i * 2 + 0] = (targetCell->pos.x + v.x) * uvpixel_x;
		//uvsRaw[i * 2 + 1] = (targetCell->pos.y + v.y) * uvpixel_y;

		buffer2D->vertices[i].pos.x = verticesRaw[i * 3 + 0];
		buffer2D->vertices[i].pos.y = verticesRaw[i * 3 + 1];
		//draw_verticesRaw[i * 3 + 0] = verticesRaw[i * 3 + 0];
		//draw_verticesRaw[i * 3 + 1] = verticesRaw[i * 3 + 1];
		//draw_verticesRaw[i * 3 + 2] = verticesRaw[i * 3 + 2];
	}

	outter_vertexnum = targetCell->outerPoint.size();
	std::vector<SsVector2>&	vertices_outerRaw = *(vertices_outer.get());
	for (size_t i = 0; i < outter_vertexnum; i++)
	{
		SsVector2& v = targetCell->outerPoint[i];

		vertices_outerRaw[i].x = v.x + offs.x;
		vertices_outerRaw[i].y = -v.y + offs.y;
	}

	tri_size = (int)(targetCell->meshTriList.size());

	buffer2D->indices.resize((size_t)(tri_size * 3));
	//indices.reset( new std::vector<unsigned short>((size_t)(tri_size * 3)) );
	//std::vector<unsigned short>&	indicesRaw = *(indices.get());
	for (size_t i = 0; i < targetCell->meshTriList.size(); i++)
	{
		SsTriangle& t = targetCell->meshTriList[i];
		buffer2D->indices[i].i0 = static_cast<uint16>(t.idxPo1);
		buffer2D->indices[i].i1 = static_cast<uint16>(t.idxPo2);
		buffer2D->indices[i].i2 = static_cast<uint16>(t.idxPo3);
		//indicesRaw[i * 3 + 0] = static_cast<uint16>(t.idxPo1);
		//indicesRaw[i * 3 + 1] = static_cast<uint16>(t.idxPo2);
		//indicesRaw[i * 3 + 2] = static_cast<uint16>(t.idxPo3);
	}
}


void	SsMeshPart::Cleanup()
{
	buffer2D.reset();
	vertices.reset();
	//colors.reset();
	//weightColors.reset();
	//uvs.reset();

	//indices.reset();
	indices_num = 0;

	//draw_vertices.reset();
	vertices_outer.reset();
	update_vertices_outer.reset();

	bindBoneInfo.reset();

	offset_world_vertices.reset();

	myPartState = 0;
}


void    SsMeshPart::updateTransformMesh()
{
//	float matrix[16];
	//std::vector<float>&	draw_verticesRaw = *(draw_vertices.get());
	std::vector<float>&	verticesRaw = *(vertices.get());
	std::vector<StBoneWeight>&	bindBoneInfoRaw = *(bindBoneInfo.get());

	for (int i = 0; i < ver_size; i++)
	{
		StBoneWeight& info = bindBoneInfoRaw[i];
		SsVector3 out;
		SsVector3 outtotal;


		SsPartState* matrixState = myPartState;

		//デフォームアトリビュートを使用している
		if (myPartState->is_defrom == true)
		{
			// キーからデフォームアトリビュートを取り出して
			SsVector3 offset(0, 0, 0);

			int size1 = (int)myPartState->deformValue.verticeChgList.size();
			int size2 = (int)getVertexNum();
			if (size1 == size2)
			{
				// キーからデフォームアトリビュートを取り出して
				offset = getOffsetWorldVerticesFromKey(i);
			}
			// 描画用デフォームアトリビュートを更新
			setOffsetWorldVertices(i, offset);
		}


		if (info.bindBoneNum == 0)
		{
			//バインドされていないメッシュの場合

			this->isBind = false;
			//MatrixTransformVector3(matrixState->matrix, info.offset[n], out);

			//デフォームオフセットを加える
			if(myPartState->is_defrom == true )
			{
				buffer2D->vertices[i].pos.x = verticesRaw[i * 3 + 0] + getOffsetLocalVertices(i).x;
				buffer2D->vertices[i].pos.y = verticesRaw[i * 3 + 1] + getOffsetLocalVertices(i).y;
				//draw_verticesRaw[i * 3 + 0] = verticesRaw[i * 3 + 0] + getOffsetLocalVertices(i).x;
				//draw_verticesRaw[i * 3 + 1] = verticesRaw[i * 3 + 1] + getOffsetLocalVertices(i).y;
				//draw_verticesRaw[i * 3 + 2] = 0;
			}
		}
		else 
		{
			this->isBind = true;
			for (int n = 0; n < info.bindBoneNum; n++)
			{
				if (info.bone[n])
				{
					if (info.bindBoneNum > 0) matrixState = info.bone[n];
					float w = info.weight[n] / 100.0f;
					MatrixTransformVector3(matrixState->matrix, info.offset[n], out);
					out.x *= w;
					out.y *= w;

					outtotal.x += out.x;
					outtotal.y += out.y;
					outtotal.z = 0;
				}
			}
			//デフォームオフセットを加える
			if (myPartState->is_defrom == true)
			{
				SsOpenGLMatrix mtx;

				// ボーンにより影響を受けた座標(ローカル座標)
				SsVector3   out;
				mtx.pushMatrix(myPartState->matrix);
				mtx.inverseMatrix();
				mtx.TransformVector3(outtotal, out);

				// デフォームによる影響(ローカル座標)
				SsVector3   vec;
				vec.x = getOffsetLocalVertices(i).x;
				vec.y = getOffsetLocalVertices(i).y;
				vec.z = 0.0f;

				outtotal = SsVector3(out.x + vec.x, out.y + vec.y, out.z + vec.z);

				// ワールド座標に変換
				mtx.pushMatrix(myPartState->matrix);
				mtx.TransformVector3(outtotal, out);

				outtotal = out;
			}

			buffer2D->vertices[i].pos.x = outtotal.x * 1.0f;
			buffer2D->vertices[i].pos.y = outtotal.y * 1.0f;
			//draw_verticesRaw[i * 3 + 0] = outtotal.x * 1.0f;
			//draw_verticesRaw[i * 3 + 1] = outtotal.y * 1.0f;
			//draw_verticesRaw[i * 3 + 2] = 0;
		}

	}
}

// デフォームアトリビュート
// ワールド座標を取得
SsVector3 SsMeshPart::getOffsetWorldVerticesFromKey(int index)
{
	SsVector3 out1, out2;
	std::vector<float>&	verticesRaw = *(vertices.get());
	{
		SsOpenGLMatrix mtx;
		SsVector3   vec;
		vec.x = verticesRaw[index * 3 + 0] + myPartState->deformValue.verticeChgList[index].x;
		vec.y = verticesRaw[index * 3 + 1] + myPartState->deformValue.verticeChgList[index].y;
		vec.z = 0.0f;

		mtx.pushMatrix(myPartState->matrix);
		mtx.TransformVector3(vec, out1);
	}

	{
		SsOpenGLMatrix mtx;
		SsVector3   vec;
		vec.x = verticesRaw[index * 3 + 0];
		vec.y = verticesRaw[index * 3 + 1];
		vec.z = 0.0f;

		mtx.pushMatrix(myPartState->matrix);
		mtx.TransformVector3(vec, out2);
	}


	SsVector3 offset;

	offset.x = out1.x - out2.x;
	offset.y = out1.y - out2.y;
	offset.z = out1.z - out2.z;

	return offset;
}

// デフォームアトリビュート取得
// ワールド座標を設定（バインドがある場合）
void	SsMeshPart::setOffsetWorldVertices(int index, const SsVector3 & v)
{
	std::vector<float>&	offset_world_verticesRaw = *(offset_world_vertices.get());
	offset_world_verticesRaw[index * 3 + 0] = v.x;
	offset_world_verticesRaw[index * 3 + 1] = v.y;
	offset_world_verticesRaw[index * 3 + 2] = v.z;
}

// デフォームアトリビュート取得
// ローカル座標系を取得（バインドがない場合）
SsVector2 SsMeshPart::getOffsetLocalVertices(int index)
{
	SsVector3 out1, out2;
	std::vector<float>&	verticesRaw = *(vertices.get());
	std::vector<float>&	offset_world_verticesRaw = *(offset_world_vertices.get());

	{
		SsOpenGLMatrix mtx;
		SsVector3   vec;
		//vec.x = verticesRaw[index * 3 + 0];
		//vec.y = verticesRaw[index * 3 + 1];
		vec.z = 0.0f;

		mtx.pushMatrix(myPartState->matrix);
		mtx.TransformVector3(vec, out1);
	}

	{
		SsOpenGLMatrix mtx;
		SsVector3   vec;
		vec.x = out1.x + offset_world_verticesRaw[index * 3 + 0];
		vec.y = out1.y + offset_world_verticesRaw[index * 3 + 1];
		vec.z = out1.z + offset_world_verticesRaw[index * 3 + 2];

		mtx.pushMatrix(myPartState->matrix);
		mtx.inverseMatrix();
		mtx.TransformVector3(vec, out2);
	}

	SsVector2 offset;

	offset.x = out2.x - verticesRaw[index * 3 + 0];
	offset.y = out2.y - verticesRaw[index * 3 + 1];

	return offset;
}

//-----------------------------------------------------------

SsMeshAnimator::SsMeshAnimator() : bindAnime(0)
{

}

void	SsMeshAnimator::setAnimeDecoder(SsAnimeDecoder* s)
{
	bindAnime = s;
}


void	SsMeshAnimator::makeMeshBoneList()
{
	if (bindAnime == 0)return;
	meshList.clear();
	animeboneList.clear();
	jointList.clear();


	size_t num = bindAnime->getStateNum();
	std::vector<SsPartState>& indexState = bindAnime->getPartState();
	for (int i = 0; i < num; i++)
	{
		if (indexState[i].partType == SsPartType::mesh)
		{
			meshList.push_back(&indexState[i]);
		}
		if (indexState[i].partType == SsPartType::armature)
		{
			animeboneList.push_back(&indexState[i]);
		}
		if (indexState[i].partType == SsPartType::joint)
		{
			jointList.push_back(&indexState[i]);
		}
	}

	modelLoad();


}

void	SsMeshAnimator::update()
{
	if (bindAnime == 0)return;

	SPRITESTUDIO6SDK_foreach(std::vector<SsPartState*>, meshList, it)
	{
		SsPartState* state = (*it);
		SsMeshPart* meshPart = state->meshPart.get();
		if (meshPart)
			meshPart->updateTransformMesh();
	}
}



void	SsMeshAnimator::copyToSsMeshPart(SsMeshBind* src, SsMeshPart* dst, std::map<int, SsPartState*> boneIdxList)
{
	int bnum = (int)boneIdxList.size();

	bool isbind = false;	//バインドするボーンが存在するか？
	std::vector<StBoneWeight>&	bindBoneInfoRaw = *(dst->bindBoneInfo.get());

	for (size_t i = 0; i < src->meshVerticesBindArray.size(); i++)
	{
		SsMeshBindInfo & bi = src->meshVerticesBindArray[i];

		if (dst->getVertexNum() > (int) i)
		{
			int cntBone = 0;
			for (int n = 0; n < bi.bindBoneNum; n++)
			{
				bindBoneInfoRaw[i].offset[n] = bi.offset[n];
				bindBoneInfoRaw[i].weight[n] = bi.weight[n];

				if (boneIdxList.count(bi.boneIndex[n]) > 0)
				{
					bindBoneInfoRaw[i].bone[n] = boneIdxList[bi.boneIndex[n]];
					isbind = true;	//バインドするボーンがある
					cntBone++;
				}

			}

			bindBoneInfoRaw[i].bindBoneNum = cntBone;
		}
	}
}


void	SsMeshAnimator::modelLoad()
{
	if (bindAnime == 0)return;
	if (meshList.empty()) return;
	if (animeboneList.empty()) return;
	if (jointList.empty()) return;

	SsModel* model = bindAnime->getMyModel();

	std::map<SsString, int>& boneListRef = model->boneList;
	

	std::map<int, SsPartState*> boneIdxList;

	for (size_t i = 0; i < animeboneList.size(); i++)
	{
		int idx = boneListRef[animeboneList[i]->part->name];
		boneIdxList[idx] = animeboneList[i];
	}

	if (meshList.size() == model->meshList.size() )
	{
		for (size_t i = 0; i < model->meshList.size(); i++)
		{
			copyToSsMeshPart(model->meshList[i], meshList[i]->meshPart.get(), boneIdxList);
		}
	}
}

}	// namespace spritestudio6
