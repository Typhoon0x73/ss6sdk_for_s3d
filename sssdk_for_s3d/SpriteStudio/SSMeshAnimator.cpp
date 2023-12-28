#include "SSMeshAnimator.hpp"
#include "SSAnimationController.hpp"
#include "SSDrawMeshPart.hpp"
#include "SSMeshBind.hpp"
#include "SSModel.hpp"

namespace sssdk
{
	SSMeshAnimator::SSMeshAnimator()
		: m_pAnimationController{ nullptr }
		, m_meshList{}
		, m_animationBoneList{}
		, m_jointList{}
	{
	}
	
	SSMeshAnimator::~SSMeshAnimator()
	{
	}

	void SSMeshAnimator::update()
	{
		if (m_pAnimationController == nullptr)
		{
			return;
		}
		for (auto* pMesh : m_meshList)
		{
			if (pMesh == nullptr)
			{
				continue;
			}
			pMesh->updateTransformMesh();
		}
	}

	void SSMeshAnimator::makeMeshBoneList()
	{
		if (m_pAnimationController == nullptr)
		{
			return;
		}
		m_animationBoneList.clear();
		m_jointList.clear();

		size_t num = m_pAnimationController->getDrawPartNum();
		for (size_t i = 0; i < num; i++)
		{
			auto* pDrawPart = m_pAnimationController->getDrawPart(i);
			if (pDrawPart->getModelPartType() == ModelPartType::armature)
			{
				m_animationBoneList.emplace_back(pDrawPart);
			}
			else if (pDrawPart->getModelPartType() == ModelPartType::joint)
			{
				m_jointList.emplace_back(pDrawPart);
			}
		}

		modelLoad();
	}

	void SSMeshAnimator::copyToSSMeshPart(const Array<SSMeshBind>& src, SSDrawMeshPart* dst, const HashTable<int32, SSDrawPart*>& boneIndexDict)
	{
		bool isBind = false; // バインドするボーンがあるか

		auto& bindBoneInfoRaw = dst->getBindBoneInfomations();
		for (size_t i = 0; i < src.size(); i++)
		{
			const auto& bindInfo = src[i];

			if (dst->getVertexNum() > i)
			{
				int boneCount = 0;
				const auto& boneInfos = bindInfo.getInfomations();
				for (int32 n = 0; n < boneInfos.size(); n++)
				{
					const auto& boneInfo = boneInfos[n];
					bindBoneInfoRaw[i].setOffset(n, boneInfo.offset);
					bindBoneInfoRaw[i].setWeight(n, boneInfo.weight);

					if (boneIndexDict.count(boneInfo.boneIndex) > 0)
					{
						bindBoneInfoRaw[i].setBonePart(n, boneIndexDict.at(boneInfo.boneIndex));
						isBind = true;
						boneCount++;
					}
				}
				bindBoneInfoRaw[i].setBindBoneNum(boneCount);
			}
		}
	}

	void SSMeshAnimator::clearMeshList()
	{
		m_meshList.clear();
	}

	void SSMeshAnimator::addMeshPart(SSDrawMeshPart* p)
	{
		m_meshList.emplace_back(p);
	}

	void SSMeshAnimator::setAnimationController(SSAnimationController* controller)
	{
		m_pAnimationController = controller;
	}

	void SSMeshAnimator::modelLoad()
	{
		if (m_pAnimationController == nullptr
			or m_meshList.isEmpty()
			or m_animationBoneList.isEmpty()
			or m_jointList.isEmpty())
		{
			return;
		}

		const SSModel* pModel = m_pAnimationController->getModel();

		const auto& bones = pModel->getBones();
		HashTable<int32, SSDrawPart*> boneIndexList;
		for (auto& it : m_animationBoneList)
		{
			auto boneItemItr = std::find_if(
				bones.begin(), bones.end(),
				[&](const SSBoneItem& boneItem) {
					return (boneItem.getKey() == it->getAnimPart()->getName());
				}
			);
			if (boneItemItr == bones.end())
			{
				continue;
			}
			int32 index = boneItemItr->getIndex();
			boneIndexList[index] = it;
		}

		const auto& meshBinds = pModel->getMeshBinds();
		const size_t meshBindCount = meshBinds.size();
		if (m_meshList.size() == meshBindCount)
		{
			for (size_t i = 0; i < meshBindCount; i++)
			{
				copyToSSMeshPart(meshBinds, m_meshList[i], boneIndexList);
			}
		}
	}
}
