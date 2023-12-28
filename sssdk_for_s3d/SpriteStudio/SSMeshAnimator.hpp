#pragma once
#ifndef SS_MESH_ANIMATOR_H_
#define SS_MESH_ANIMATOR_H_

#include <Siv3D.hpp>

namespace sssdk
{
	class SSMeshBind;
	class SSDrawPart;
	class SSDrawMeshPart;
	class SSAnimationController;

	class SSMeshAnimator
	{
	public:

		explicit SSMeshAnimator();
		virtual ~SSMeshAnimator();

		void update();

		void makeMeshBoneList();
		void copyToSSMeshPart(const Array<SSMeshBind>& src, SSDrawMeshPart* dst, const HashTable<int32, SSDrawPart*>& boneIndexDict);

		void setAnimationController(SSAnimationController* controller);

		void clearMeshList();
		void addMeshPart(SSDrawMeshPart* p);

	private:

		void modelLoad();

	private:

		SSAnimationController* m_pAnimationController;
		Array<SSDrawMeshPart*> m_meshList;
		Array<SSDrawPart*> m_animationBoneList;
		Array<const SSDrawPart*> m_jointList;
	};
}

#endif // !SS_MESH_ANIMATOR_H_
