#include "SSBoneWeight.hpp"

namespace sssdk
{
	SSBoneWeight::SSBoneWeight()
		: m_weight{}
		, m_weight_f{}
		, m_bone{}
		, m_offset{}
		, m_bindBoneNum{}
		, m_length{}
		, m_lengthTotal{}
	{
	}

	SSBoneWeight::~SSBoneWeight()
	{
	}

	void SSBoneWeight::Cleanup()
	{
		m_bindBoneNum = 0;
		m_lengthTotal = 0.0f;

		for (int32 i = 0; i < MeshPartBoneMax; i++)
		{
			m_weight[i] = 0;
			m_weight_f[i] = 0.0f;
			m_bone[i] = nullptr;
			m_offset[i] = Vec3::Zero();
			m_length[i] = 0.0f;
		}
	}

	void SSBoneWeight::setWeight(int32 n, int32 weight)
	{
		m_weight[n] = weight;
	}

	void SSBoneWeight::setBonePart(int32 n, SSDrawPart* part)
	{
		m_bone[n] = part;
	}

	void SSBoneWeight::setOffset(int32 n, const Float3& offset)
	{
		m_offset[n] = offset;
	}

	void SSBoneWeight::setBindBoneNum(int32 num)
	{
		m_bindBoneNum = num;
	}

	int32 SSBoneWeight::getBoneNum() const
	{
		return m_bindBoneNum;
	}

	int32 SSBoneWeight::getWeight(int32 n) const
	{
		return m_weight[n];
	}

	SSDrawPart* SSBoneWeight::getBonePart(int32 n) const
	{
		return m_bone[n];
	}

	const Float3& SSBoneWeight::getOffset(int32 n) const
	{
		return m_offset[n];
	}

}
