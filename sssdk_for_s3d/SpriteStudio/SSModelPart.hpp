#pragma once
#ifndef SS_MODEL_PART_H_
#define SS_MODEL_PART_H_

#include <Siv3D.hpp>
#include "SSDefine.hpp"

namespace sssdk
{
	class SSModelPart
	{
	public:

		explicit SSModelPart();
		explicit SSModelPart(const XMLElement& element);
		virtual ~SSModelPart();

		bool load(const XMLElement& element);

		SIV3D_NODISCARD_CXX20 StringView getName() const;

		SIV3D_NODISCARD_CXX20 int32 getArrayIndex() const;
		SIV3D_NODISCARD_CXX20 int32 getParentIndex() const;

		SIV3D_NODISCARD_CXX20 ModelPartType getType() const;
		SIV3D_NODISCARD_CXX20 ModelPartBoundsType getBoundsType() const;
		SIV3D_NODISCARD_CXX20 ModelPartInheritType getInheritType() const;

		SIV3D_NODISCARD_CXX20 float getInheritRate(ATTRIBUTE_KIND kind) const;
		SIV3D_NODISCARD_CXX20 DRAW_MODE getAlphaBlendType() const;

		int32 getBoneLength() const;
		const Vec2& getBonePosition() const;
		double getBoneRotation() const;

		SIV3D_NODISCARD_CXX20 bool IsMaskInfluence() const;

	private:

		String m_name;
		int32 m_arrayIndex;
		int32 m_parentIndex;
		ModelPartType m_type;
		ModelPartBoundsType m_boundsType;
		ModelPartInheritType m_inheritType;
		float m_inheritRates[ATTRIBUTE_KIND_NUM];
		DRAW_MODE m_alphaBlendType;
		int32 m_boneLength;
		Vec2 m_bonePosition;
		double m_boneRotation;
		bool m_isMaskInfluence;
	};
}

#endif // !SS_MODEL_PART_H_
