
#include "SSModelPart.hpp"

namespace sssdk
{
	SSModelPart::SSModelPart()
		: m_name{ U"" }
		, m_arrayIndex{ 0 }
		, m_parentIndex{ -1 }
		, m_type{ ModelPartType::null }
		, m_boundsType{ ModelPartBoundsType::none }
		, m_inheritType{ ModelPartInheritType::self }
		, m_inheritRates()
		, m_alphaBlendType{ DRAW_MODE_MIX }
		, m_boneLength{ 0 }
		, m_bonePosition{ 0.0, 0.0 }
		, m_boneRotation{ 0.0 }
		, m_isMaskInfluence{ false }
	{
		std::fill_n(m_inheritRates, ATTRIBUTE_KIND_NUM, 0.0f);
	}

	SSModelPart::SSModelPart(const XMLElement& element)
		: SSModelPart()
	{
		load(element);
	}

	SSModelPart::~SSModelPart()
	{
	}

	bool SSModelPart::load(const XMLElement& element)
	{
		if (element.name() != U"value")
		{
			return false;
		}
		for (auto child = element.firstChild(); child; child = child.nextSibling())
		{
			const auto& name = child.name();
			if (name == U"name")
			{
				m_name = child.text();
			}
			else if (name == U"arrayIndex")
			{
				m_arrayIndex = ParseOr<int32, int32>(child.text(), 0);
			}
			else if (name == U"parentIndex")
			{
				m_parentIndex = ParseOr<int32, int32>(child.text(), -1);
			}
			else if (name == U"type")
			{
				const auto& text = child.text();
				if (text == U"null")
				{
					m_type = ModelPartType::null;
				}
				else if (text == U"normal")
				{
					m_type = ModelPartType::normal;
				}
				else if (text == U"shape")
				{
					m_type = ModelPartType::shape;
				}
				else if (text == U"text")
				{
					m_type = ModelPartType::text;
				}
				else if (text == U"nines")
				{
					m_type = ModelPartType::nines;
				}
				else if (text == U"instance")
				{
					m_type = ModelPartType::instance;
				}
				else if (text == U"armature")
				{
					m_type = ModelPartType::armature;
				}
				else if (text == U"effect")
				{
					m_type = ModelPartType::effect;
				}
				else if (text == U"mesh")
				{
					m_type = ModelPartType::mesh;
				}
				else if (text == U"movenode")
				{
					m_type = ModelPartType::movenode;
				}
				else if (text == U"constraint")
				{
					m_type = ModelPartType::constraint;
				}
				else if (text == U"mask")
				{
					m_type = ModelPartType::mask;
				}
				else if (text == U"joint")
				{
					m_type = ModelPartType::joint;
				}
				else if (text == U"bonepoint")
				{
					m_type = ModelPartType::bonepoint;
				}
				else if (text == U"transform_constraint")
				{
					m_type = ModelPartType::transform_constraint;
				}
				else if (text == U"camera")
				{
					m_type = ModelPartType::camera;
				}
				else if (text == U"ssaudio")
				{
					m_type = ModelPartType::audio;
				}
			}
			else if (name == U"boundsType")
			{
				const auto& text = child.text();
				if (text == U"none")
				{
					m_boundsType = ModelPartBoundsType::none;
				}
				else if (text == U"quad")
				{
					m_boundsType = ModelPartBoundsType::quad;
				}
				else if (text == U"aabb")
				{
					m_boundsType = ModelPartBoundsType::aabb;
				}
				else if (text == U"circle")
				{
					m_boundsType = ModelPartBoundsType::circle;
				}
				else if (text == U"circle_smin")
				{
					m_boundsType = ModelPartBoundsType::circle_scale_min;
				}
				else if (text == U"circle_smax")
				{
					m_boundsType = ModelPartBoundsType::circle_scale_max;
				}
			}
			else if (name == U"inheritType")
			{
				const auto& text = child.text();
				if (text == U"parent")
				{
					m_inheritType = ModelPartInheritType::parant;
				}
				else if (text == U"self")
				{
					m_inheritType = ModelPartInheritType::self;
				}
			}
			else if (name == U"ineheritRates")
			{
				for (auto inheritRate = child.firstChild(); inheritRate; inheritRate = inheritRate.nextSibling())
				{
					for (auto kind : step(static_cast<int32>(ATTRIBUTE_KIND_NUM)))
					{
						if (inheritRate.name() == ATTRIBUTE_KIND_TAG_NAMES[kind])
						{
							m_inheritRates[kind] = ParseOr<float, float>(inheritRate.text(), 1.0f);
						}
					}
				}
			}
			else if (name == U"alphaBlendType")
			{
				for (auto i : step(std::size(DRAW_MODE_TAG_NAMES)))
				{
					if (child.text().lowercased() == DRAW_MODE_TAG_NAMES[i])
					{
						m_alphaBlendType = static_cast<DRAW_MODE>(i);
						break;
					}
				}
			}
			else if (name == U"boneLength")
			{
				m_boneLength = ParseOr<int32, int32>(child.text(), 0);
			}
			else if (name == U"bonePosition")
			{
				const auto& lines = child.text().split(U' ');
				if (not lines.isEmpty())
				{
					m_bonePosition.x = ParseOr<double, double>(lines[0], 0.0);
					m_bonePosition.y = ParseOr<double, double>(lines[1], 0.0);
				}
			}
			else if (name == U"boneRotation")
			{
				m_boneRotation = ParseOr<double, double>(child.text(), 0.0);
			}
			else if (name == U"maskInfluence")
			{
				m_isMaskInfluence = (ParseOr<int32, int32>(child.text(), 0) == 1);
			}
		}
		return true;
	}

	StringView SSModelPart::getName() const
	{
		return m_name;
	}

	int32 SSModelPart::getArrayIndex() const
	{
		return m_arrayIndex;
	}

	int32 SSModelPart::getParentIndex() const
	{
		return m_parentIndex;
	}

	ModelPartType SSModelPart::getType() const
	{
		return m_type;
	}

	ModelPartBoundsType SSModelPart::getBoundsType() const
	{
		return m_boundsType;
	}

	ModelPartInheritType SSModelPart::getInheritType() const
	{
		return m_inheritType;
	}

	float SSModelPart::getInheritRate(ATTRIBUTE_KIND kind) const
	{
		return m_inheritRates[kind];
	}

	DRAW_MODE SSModelPart::getAlphaBlendType() const
	{
		return m_alphaBlendType;
	}

	int32 SSModelPart::getBoneLength() const
	{
		return m_boneLength;
	}

	const Vec2& SSModelPart::getBonePosition() const
	{
		return m_bonePosition;
	}

	double SSModelPart::getBoneRotation() const
	{
		return m_boneRotation;
	}

	bool SSModelPart::IsMaskInfluence() const
	{
		return m_isMaskInfluence;
	}
}
