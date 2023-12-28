#pragma once
#ifndef SS_BONE_WEIGHT_H_
#define SS_BONE_WEIGHT_H_

#include <Siv3D.hpp>

namespace sssdk
{
	constexpr int32 MeshPartBoneMax = 128;
	constexpr int32 MeshPartCheckRange = 4;

	class SSDrawPart;
	class SSBoneWeight
	{
	public:
		explicit SSBoneWeight();
		virtual ~SSBoneWeight();

		void Cleanup();

		int32 getBoneNum() const;

		int32 getWeight(int32 n) const;
		SSDrawPart* getBonePart(int32 n) const;
		const Float3& getOffset(int32 n) const;

	private:

		int32 m_weight[MeshPartBoneMax];
		float m_weight_f[MeshPartBoneMax];

		SSDrawPart* m_bone[MeshPartBoneMax];
		Float3 m_offset[MeshPartBoneMax];

		int32 m_bindBoneNum;

		float m_length[MeshPartBoneMax]; //temp
		float m_lengthTotal; //temp
	};
}

#endif // !SS_BONE_WEIGHT_H_
