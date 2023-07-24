
#include "SSAttributeKeyValue.hpp"

namespace sssdk
{
	SSAttributeKeyValue::SSAttributeKeyValue()
		: m_keyFrame{ 0 }
		, m_ipType{ InterpolationType::invalid }
		, m_value{}
		, m_easeRate{ 0.0f }
	{
	}

	SSAttributeKeyValue::SSAttributeKeyValue(ATTRIBUTE_KIND kind, const XMLElement& element)
		: SSAttributeKeyValue()
	{
		load(kind, element);
	}

	SSAttributeKeyValue::~SSAttributeKeyValue()
	{
	}

	bool SSAttributeKeyValue::load(ATTRIBUTE_KIND kind, const XMLElement& element)
	{
		if (element.name() != U"key")
		{
			return false;
		}
		for (const auto& attribute : element.attributes())
		{
			if (attribute.first == U"time")
			{
				m_keyFrame = ParseOr<int32, int32>(attribute.second, 0);
			}
			else if (attribute.first == U"ipType")
			{
				for (auto type : step(std::size(INTERPOLATION_TYPE_NAMES)))
				{
					if (attribute.second == INTERPOLATION_TYPE_NAMES[type])
					{
						m_ipType = static_cast<InterpolationType>(type);
					}
				}
			}
			else if (attribute.first == U"easingRate")
			{
				m_easeRate = ParseOr<float, float>(attribute.second, 0.0f);
			}
		}
		for (auto child = element.firstChild(); child; child = child.nextSibling())
		{
			const auto& name = child.name();
			if (name == U"value")
			{
				m_value.set(kind, child);
			}
			else if (name == U"curve")
			{
				auto lines = child.text().split(U' ');
				if (lines.size() >= 4)
				{
					m_curve.startTime = ParseOr<float, float>(lines[0], 0.0f);
					m_curve.startValue = ParseOr<float, float>(lines[1], 0.0f);
					m_curve.endTime = ParseOr<float, float>(lines[2], 0.0f);
					m_curve.endValue = ParseOr<float, float>(lines[3], 0.0f);
				}
			}
		}
		return true;
	}

	int32 SSAttributeKeyValue::getKeyFrame() const
	{
		return m_keyFrame;
	}

	InterpolationType SSAttributeKeyValue::getIpType() const
	{
		return m_ipType;
	}

	const SSValue& SSAttributeKeyValue::getValue() const
	{
		return m_value;
	}

	const SSCurve& SSAttributeKeyValue::getCurve() const
	{
		return m_curve;
	}

	float SSAttributeKeyValue::getEaseRate() const
	{
		return m_easeRate;
	}
}
