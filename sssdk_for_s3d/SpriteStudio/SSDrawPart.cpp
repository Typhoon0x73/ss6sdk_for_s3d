
#include "SSDrawPart.hpp"
#include "SSModelPart.hpp"

namespace sssdk
{
	float SSAccelerating(float start, float end, float ratio)
	{
		float rate = ratio * ratio;
		return start + (rate * (end - start));
	}

	float SSDecelerating(float start, float end, float ratio)
	{
		float time = 1 - ratio;
		float rate = 1 - time * time;
		return start + (rate * (end - start));
	}

	/**
		エルミート
	*/
	float SSHermite(float start, float end, float ratio, const SSCurve& curve)
	{
		float t2 = ratio * ratio;
		float t3 = t2 * ratio;
		float result =
			(2 * t3 - 3 * t2 + 1) * start +
			(-2 * t3 + 3 * t2) * end +
			(t3 - 2 * t2 + ratio) * (curve.startValue - start) +
			(t3 - t2) * (curve.endValue - end);
		return result;
	}

	float SSBezier(float start, float end, float ratio, const SSCurve& curve, float startKeyFrame, float endKeyFrame)
	{
		//値が変化しない場合は左キーを補間値とする
		if ((start == end) && (curve.startValue == 0.0f) && (curve.endValue == 0.0f))
		{
			return start;
		}


		float currentPos = (endKeyFrame - startKeyFrame) * ratio + startKeyFrame;

		float ret = end;
		float currentCalc = 0.5f;
		float calcRange = 0.5f;

		float temp1;
		float temp2;
		float temp3;

		float currentX;

		for (int iLoop = 0; iLoop < 8; iLoop++)
		{// more count of loop, better precision increase
			temp1 = 1.0f - currentCalc;
			temp2 = temp1 * temp1;
			temp3 = temp2 * temp1;
			currentX = (temp3 * startKeyFrame) +
				(3 * temp2 * currentCalc * (curve.startTime + startKeyFrame)) +
				(3 * temp1 * currentCalc * currentCalc * (curve.endTime + endKeyFrame)) +
				(currentCalc * currentCalc * currentCalc * endKeyFrame);

			calcRange /= 2.0f;
			if (currentX > currentPos)
			{
				currentCalc -= calcRange;
			}
			else
			{
				currentCalc += calcRange;
			}
		}

		// finally calculate with current value
		temp1 = 1.0f - currentCalc;
		temp2 = temp1 * temp1;
		temp3 = temp2 * temp1;
		ret = (temp3 * start) +
			(3 * temp2 * currentCalc * (curve.startValue + start)) +
			(3 * temp1 * currentCalc * currentCalc * (curve.endValue + end)) +
			(currentCalc * currentCalc * currentCalc * end);

		return ret;
	}

	float SSInterpolate(InterpolationType ipType, float ratio, float start, float end, const SSCurve& curve, float startKeyFrame, float endKeyFrame)
	{
		double r = start;
		switch (ipType)
		{
		case sssdk::InterpolationType::linear:
			r = Math::Lerp(start, end, ratio);
			break;
		case sssdk::InterpolationType::hermite:
			r = SSHermite(start, end, ratio, curve);
			break;
		case sssdk::InterpolationType::bezier:
			r = SSBezier(start, end, ratio, curve, startKeyFrame, endKeyFrame);
			break;
		case sssdk::InterpolationType::acceleration:
			r = SSAccelerating(start, end, ratio);
			break;
		case sssdk::InterpolationType::deceleration:
			r = SSDecelerating(start, end, ratio);
			break;
		case sssdk::InterpolationType::easeIn:
			r = EaseIn(Easing::Linear, start, end, ratio);
			break;
		case sssdk::InterpolationType::easeOut:
			r = EaseOut(Easing::Linear, start, end, ratio);
			break;
		case sssdk::InterpolationType::easeInOut:
			r = EaseInOut(Easing::Linear, start, end, ratio);
			break;
		case sssdk::InterpolationType::easeExponentialIn:
			r = EaseIn(Easing::Expo, start, end, ratio);
			break;
		case sssdk::InterpolationType::easeExponentialOut:
			r = EaseOut(Easing::Expo, start, end, ratio);
			break;
		case sssdk::InterpolationType::easeExponentialInOut:
			r = EaseInOut(Easing::Expo, start, end, ratio);
			break;
		case sssdk::InterpolationType::easeSineIn:
			r = EaseIn(Easing::Sine, start, end, ratio);
			break;
		case sssdk::InterpolationType::easeSineOut:
			r = EaseOut(Easing::Sine, start, end, ratio);
			break;
		case sssdk::InterpolationType::easeSineInOut:
			r = EaseInOut(Easing::Sine, start, end, ratio);
			break;
		case sssdk::InterpolationType::easeElasticIn:
			r = EaseIn(Easing::Elastic, start, end, ratio);
			break;
		case sssdk::InterpolationType::easeElasticOut:
			r = EaseOut(Easing::Elastic, start, end, ratio);
			break;
		case sssdk::InterpolationType::easeElasticInOut:
			r = EaseInOut(Easing::Elastic, start, end, ratio);
			break;
		case sssdk::InterpolationType::easeBounceIn:
			r = EaseIn(Easing::Bounce, start, end, ratio);
			break;
		case sssdk::InterpolationType::easeBounceOut:
			r = EaseOut(Easing::Bounce, start, end, ratio);
			break;
		case sssdk::InterpolationType::easeBounceInOut:
			r = EaseInOut(Easing::Bounce, start, end, ratio);
			break;
		case sssdk::InterpolationType::easeBackIn:
			r = EaseIn(Easing::Back, start, end, ratio);
			break;
		case sssdk::InterpolationType::easeBackOut:
			r = EaseOut(Easing::Back, start, end, ratio);
			break;
		case sssdk::InterpolationType::easeBackInOut:
			r = EaseInOut(Easing::Back, start, end, ratio);
			break;

		case sssdk::InterpolationType::invalid:
		case sssdk::InterpolationType::none:
		default:
			break;
		}
		return static_cast<float>(r);
	}

	template<typename T>
	void SSInterpolationValue(int32 frame, const SSAttributeKeyValue* left, const SSAttributeKeyValue* right, T& out)
	{
		if (not right)
		{
			out = *(left->getValue().getValue<T>());
			return;
		}
		float v1 = static_cast<float>(*(left->getValue().getValue<T>()));
		float v2 = static_cast<float>(*(right->getValue().getValue<T>()));

		int32 leftKeyFrame = left->getKeyFrame();
		int32 rightKeyFrame = right->getKeyFrame();

		int32 range = rightKeyFrame - leftKeyFrame;
		float ratio = static_cast<float>(frame - leftKeyFrame) / range;

		InterpolationType ipType = left->getIpType();
		const SSCurve& curve = left->getCurve();
		out = static_cast<T>(SSInterpolate(ipType, ratio, v1, v2, curve, static_cast<float>(leftKeyFrame), static_cast<float>(rightKeyFrame)));
	}

	template<>
	void SSInterpolationValue(int32 frame, const SSAttributeKeyValue* left, const SSAttributeKeyValue* right, SSPartColorValue& out)
	{
		// TODO
	}

	template<>
	void SSInterpolationValue(int32 frame, const SSAttributeKeyValue* left, const SSAttributeKeyValue* right, SSVertexValue& out)
	{
		// TODO
	}

	template<>
	void SSInterpolationValue(int32 frame, const SSAttributeKeyValue* left, const SSAttributeKeyValue* right, SSInstanceParamValue& out)
	{
		// TODO
	}

	template<>
	void SSInterpolationValue(int32 frame, const SSAttributeKeyValue* left, const SSAttributeKeyValue* right, SSEffectValue& out)
	{
		// TODO
	}

	template<>
	void SSInterpolationValue(int32 frame, const SSAttributeKeyValue* left, const SSAttributeKeyValue* right, SSDeformVertexValue& out)
	{
		// TODO
	}

	template<>
	void SSInterpolationValue(int32 frame, const SSAttributeKeyValue* left, const SSAttributeKeyValue* right, SSAudioValue& out)
	{
		// TODO
	}

	template<>
	void SSInterpolationValue(int32 frame, const SSAttributeKeyValue* left, const SSAttributeKeyValue* right, SSTextureChangeValue& out)
	{
		// TODO
	}
}

namespace sssdk
{
	bool SSDrawPart::CompareSortFuncByPrio(const std::unique_ptr<SSDrawPart>& v1, const std::unique_ptr<SSDrawPart>& v2)
	{
		return v1->m_priority < v2->m_priority;
	}

	bool SSDrawPart::CompareSortFuncByZ(const std::unique_ptr<SSDrawPart>& v1, const std::unique_ptr<SSDrawPart>& v2)
	{
		return v1->m_position.z < v2->m_position.z;
	}

	SSDrawPart::SSDrawPart(const SSAnimationPart* anim)
		: m_pParentPart{ nullptr }
		, m_pModelPart{ nullptr }
		, m_pSetupPart{ nullptr }
		, m_pAnimPart{ nullptr }
		, m_cell{}
		, m_position{ Float3::Zero() }
		, m_rotation{ Float3::Zero() }
		, m_scale{ 1.0f, 1.0f }
		, m_localScale{ 1.0f, 1.0f }
		, m_alpha{ 1.0f }
		, m_localAlpha{ 1.0f }
		, m_priority{ 0 }
		, m_isFlipH{ false }
		, m_isFlipV{ false }
		, m_isHide{ false }
		, m_partColor{}
		, m_vertex{}
		, m_pivot{ Float2::Zero() }
		, m_size{ Float2::Zero() }
		, m_isImageFlipH{ false }
		, m_isImageFlipV{ false }
		, m_uvTranslate{ Float2::Zero() }
		, m_uvRotation{ 0.0f }
		, m_uvScale{ 1.0f, 1.0f }
		, m_boundsRadius{ 0.0f }
		, m_mask{ 0 }
		, m_instanceParam{}
		, m_effect{}
		, m_deform{}
		, m_audio{}
		, m_textureChange{}
		, m_worldMatrix{ Mat4x4::Identity() }
		, m_isFindSetupHideKey{ false }
		, m_isFindKeySizeX{ false }
		, m_isFindKeySizeY{ false }
		, m_isUsedDeform{ false }
	{
		setAnimPart(anim);
	}

	SSDrawPart::~SSDrawPart()
	{
	}

	void SSDrawPart::update(int32 frame)
	{
		{
			m_isFindKeySizeX = false;
			m_isFindKeySizeY = false;
		}

		if (m_pSetupPart)
		{
			for (const auto& attribute : m_pSetupPart->getAttributes())
			{
				const auto* leftKeyValue = attribute.getLeftKeyValue(frame);
				const auto* rightKeyValue = attribute.getRightKeyValue(frame);
				setValue(frame, attribute.getAttributeKind(), leftKeyValue, rightKeyValue);
			}
		}
		if (m_pAnimPart)
		{
			for (const auto& attribute : m_pAnimPart->getAttributes())
			{
				const auto* leftKeyValue = attribute.getLeftKeyValue(frame);
				const auto* rightKeyValue = attribute.getRightKeyValue(frame);
				setValue(frame, attribute.getAttributeKind(), leftKeyValue, rightKeyValue);
			}
		}
		updateMatrix();
	}

	void SSDrawPart::draw() const
	{
	}

	void SSDrawPart::setParent(SSDrawPart* const parent)
	{
		m_pParentPart = parent;
	}

	void SSDrawPart::setModelPart(const SSModelPart* const model)
	{
		m_pModelPart = model;
	}

	void SSDrawPart::setAnimPart(const SSAnimationPart* part)
	{
		m_pAnimPart = part;
		setHideParam();
	}

	void SSDrawPart::setSetupPart(const SSAnimationPart* setup)
	{
		m_pSetupPart = setup;
		setHideParam();
	}

	void SSDrawPart::setHideParam()
	{
		m_isFindSetupHideKey = false;
		if (m_pSetupPart)
		{
			for (const auto& attribute : m_pSetupPart->getAttributes())
			{
				if (attribute.getAttributeKind() == ATTRIBUTE_KIND_HIDE)
				{
					m_isFindSetupHideKey = true;
					return;
				}
			}
		}
		if (m_pAnimPart)
		{
			for (const auto& attribute : m_pAnimPart->getAttributes())
			{
				if (attribute.getAttributeKind() == ATTRIBUTE_KIND_HIDE)
				{
					return;
				}
			}
			// 非表示キーが見つからない場合は常に非表示。
			m_isHide = true;
		}
	}

	SSDrawPart* SSDrawPart::getParent() const
	{
		return m_pParentPart;
	}

	const SSAnimationPart* SSDrawPart::getSetupPart() const
	{
		return m_pSetupPart;
	}

	const SSAnimationPart* SSDrawPart::getAnimPart() const
	{
		return m_pAnimPart;
	}

	ModelPartType SSDrawPart::getModelPartType() const
	{
		if (m_pModelPart == nullptr)
		{
			return ModelPartType::null;
		}
		return m_pModelPart->getType();
	}

	void SSDrawPart::updateMatrix()
	{
		Mat4x4 rot = Mat4x4::RollPitchYaw(Math::ToRadians(m_rotation.x), Math::ToRadians(m_rotation.y), Math::ToRadians(-m_rotation.z));
		Mat4x4 scale = Mat4x4::Scale(Float3{ m_scale, 1.0f });
		Mat4x4 trans = Mat4x4::Translate(m_position.x, -m_position.y, m_position.z);

		m_worldMatrix = Mat4x4::Identity();
		m_worldMatrix *= scale;
		m_worldMatrix *= rot;
		m_worldMatrix *= trans;
		if (m_pParentPart)
		{
			m_worldMatrix *= m_pParentPart->m_worldMatrix;
		}
	}

	Float3 SSDrawPart::getRotation() const
	{
		if (m_pParentPart)
		{
			return m_pParentPart->getRotation() + m_rotation;
		}
		return m_rotation;
	}

	float SSDrawPart::getAlpha() const
	{
		if (m_pParentPart)
		{
			return m_pParentPart->getAlpha() * m_alpha;
		}
		return m_alpha;
	}

	const Mat4x4& SSDrawPart::getWorldMatrix() const
	{
		return m_worldMatrix;
	}

	void SSDrawPart::setValue(int32 frame, ATTRIBUTE_KIND kind, const SSAttributeKeyValue* left, const SSAttributeKeyValue* right)
	{
		if (not left)
		{
			if (kind == ATTRIBUTE_KIND_HIDE and right)
			{
				// 左がなく、右がある場合、先頭データがまだ後ろ。
				// 先頭の非表示キーより手前の場合は常に非表示にする。
				// セットアップによってm_isSetupHideKeyFindがあった場合は強制非表示にしない。
				if (not m_isFindSetupHideKey)
				{
					m_isHide = true;
				}
			}
			return;
		}
		switch (kind)
		{
		case ATTRIBUTE_KIND_CELL:
			m_cell.name = left->getValue().getValue<SSCellValue>()->name;
			m_cell.mapId = left->getValue().getValue<SSCellValue>()->mapId;
			break;
		case sssdk::ATTRIBUTE_KIND_POSX:
			SSInterpolationValue<float>(frame, left, right, m_position.x);
			break;
		case sssdk::ATTRIBUTE_KIND_POSY:
			SSInterpolationValue<float>(frame, left, right, m_position.y);
			break;
		case sssdk::ATTRIBUTE_KIND_POSZ:
			SSInterpolationValue<float>(frame, left, right, m_position.z);
			break;
		case sssdk::ATTRIBUTE_KIND_ROTX:
			SSInterpolationValue<float>(frame, left, right, m_rotation.x);
			break;
		case sssdk::ATTRIBUTE_KIND_ROTY:
			SSInterpolationValue<float>(frame, left, right, m_rotation.y);
			break;
		case sssdk::ATTRIBUTE_KIND_ROTZ:
			SSInterpolationValue<float>(frame, left, right, m_rotation.z);
			break;
		case sssdk::ATTRIBUTE_KIND_SCLX:
			SSInterpolationValue<float>(frame, left, right, m_scale.x);
			break;
		case sssdk::ATTRIBUTE_KIND_SCLY:
			SSInterpolationValue<float>(frame, left, right, m_scale.y);
			break;
		case sssdk::ATTRIBUTE_KIND_LSCX:
			SSInterpolationValue<float>(frame, left, right, m_localScale.x);
			break;
		case sssdk::ATTRIBUTE_KIND_LSCY:
			SSInterpolationValue<float>(frame, left, right, m_localScale.y);
			break;
		case sssdk::ATTRIBUTE_KIND_ALPH:
			SSInterpolationValue<float>(frame, left, right, m_alpha);
			break;
		case sssdk::ATTRIBUTE_KIND_LALP:
			SSInterpolationValue<float>(frame, left, right, m_localAlpha);
			break;
		case sssdk::ATTRIBUTE_KIND_PRIO:
			SSInterpolationValue<int32>(frame, left, right, m_priority);
			break;
		case sssdk::ATTRIBUTE_KIND_FLPH:
			SSInterpolationValue<bool>(frame, left, right, m_isFlipH);
			break;
		case sssdk::ATTRIBUTE_KIND_FLPV:
			SSInterpolationValue<bool>(frame, left, right, m_isFlipV);
			break;
		case sssdk::ATTRIBUTE_KIND_HIDE:
			SSInterpolationValue<bool>(frame, left, right, m_isHide);
			break;
		case sssdk::ATTRIBUTE_KIND_PCOL:
			SSInterpolationValue<SSPartColorValue>(frame, left, right, m_partColor);
			break;
		case sssdk::ATTRIBUTE_KIND_VCOL:
			// TODO
			break;
		case sssdk::ATTRIBUTE_KIND_SHDR:
			// TODO
			break;
		case sssdk::ATTRIBUTE_KIND_VERT:
			SSInterpolationValue<SSVertexValue>(frame, left, right, m_vertex);
			break;
		case sssdk::ATTRIBUTE_KIND_PVTX:
			SSInterpolationValue<float>(frame, left, right, m_pivot.x);
			break;
		case sssdk::ATTRIBUTE_KIND_PVTY:
			SSInterpolationValue<float>(frame, left, right, m_pivot.y);
			break;
		case sssdk::ATTRIBUTE_KIND_ANCX:
			// TODO
			break;
		case sssdk::ATTRIBUTE_KIND_ANCY:
			// TODO
			break;
		case sssdk::ATTRIBUTE_KIND_SIZX:
			SSInterpolationValue<float>(frame, left, right, m_size.x);
			m_isFindKeySizeX = true;
			break;
		case sssdk::ATTRIBUTE_KIND_SIZY:
			SSInterpolationValue<float>(frame, left, right, m_size.y);
			m_isFindKeySizeY = true;
			break;
		case sssdk::ATTRIBUTE_KIND_IFLH:
			SSInterpolationValue<bool>(frame, left, right, m_isImageFlipH);
			break;
		case sssdk::ATTRIBUTE_KIND_IFLV:
			SSInterpolationValue<bool>(frame, left, right, m_isImageFlipV);
			break;
		case sssdk::ATTRIBUTE_KIND_UVTX:
			SSInterpolationValue<float>(frame, left, right, m_uvTranslate.x);
			break;
		case sssdk::ATTRIBUTE_KIND_UVTY:
			SSInterpolationValue<float>(frame, left, right, m_uvTranslate.y);
			break;
		case sssdk::ATTRIBUTE_KIND_UVRZ:
			SSInterpolationValue<float>(frame, left, right, m_uvRotation);
			break;
		case sssdk::ATTRIBUTE_KIND_UVSX:
			SSInterpolationValue<float>(frame, left, right, m_uvScale.x);
			break;
		case sssdk::ATTRIBUTE_KIND_UVSY:
			SSInterpolationValue<float>(frame, left, right, m_uvScale.y);
			break;
		case sssdk::ATTRIBUTE_KIND_BNDR:
			SSInterpolationValue<float>(frame, left, right, m_boundsRadius);
			break;
		case sssdk::ATTRIBUTE_KIND_MASK:
			SSInterpolationValue<int32>(frame, left, right, m_mask);
			break;
		case sssdk::ATTRIBUTE_KIND_USER:
			// TODO
			break;
		case sssdk::ATTRIBUTE_KIND_SIGN:
			// TODO
			break;
		case sssdk::ATTRIBUTE_KIND_IPRM:
			SSInterpolationValue<SSInstanceParamValue>(frame, left, right, m_instanceParam);
			break;
		case sssdk::ATTRIBUTE_KIND_EFCT:
			SSInterpolationValue<SSEffectValue>(frame, left, right, m_effect);
			break;
		case sssdk::ATTRIBUTE_KIND_DEFM:
			m_isUsedDeform = true;
			SSInterpolationValue<SSDeformVertexValue>(frame, left, right, m_deform);
			break;
		case sssdk::ATTRIBUTE_KIND_ADIO:
			SSInterpolationValue<SSAudioValue>(frame, left, right, m_audio);
			break;
		case sssdk::ATTRIBUTE_KIND_TCHG:
			SSInterpolationValue<SSTextureChangeValue>(frame, left, right, m_textureChange);
			break;

		case sssdk::ATTRIBUTE_KIND_NUM:
		default:
			break;
		}
	}
}

