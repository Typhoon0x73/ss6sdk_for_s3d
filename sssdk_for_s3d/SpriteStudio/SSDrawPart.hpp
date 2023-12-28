#pragma once
#ifndef SS_DRAW_PART_H_
#define SS_DRAW_PART_H_

#include <Siv3D.hpp>
#include "SSDefine.hpp"
#include "SSAnimationPart.hpp"

namespace sssdk
{
	class SSModelPart;
	class SSAnimationPart;
	class SSDrawCellPart;
	class SSDrawPart
	{
	public:

		static bool CompareSortFuncByPrio(const std::unique_ptr<SSDrawPart>& v1, const std::unique_ptr<SSDrawPart>& v2);
		static bool CompareSortFuncByZ(const std::unique_ptr<SSDrawPart>& v1, const std::unique_ptr<SSDrawPart>& v2);

		explicit SSDrawPart(const SSAnimationPart* setup);
		virtual ~SSDrawPart();

		virtual void update(int32 frame);
		virtual void draw(const Vec2& canvasOffset) const;

		void setParent(SSDrawPart* const parent);
		void setModelPart(const SSModelPart* const model);
		void setAnimPart(const SSAnimationPart* part);
		void setSetupPart(const SSAnimationPart* setup);
		void setHideParam();
		SIV3D_NODISCARD_CXX20 SSDrawPart* getParent() const;
		SIV3D_NODISCARD_CXX20 const SSAnimationPart* getSetupPart() const;
		SIV3D_NODISCARD_CXX20 const SSAnimationPart* getAnimPart() const;

		const Mat4x4& getWorldMatrix() const;

	protected:

		virtual void updateMatrix();

		Float3 getRotation() const;
		float getAlpha() const;

	private:

		void setValue(int32 frame, ATTRIBUTE_KIND kind, const SSAttributeKeyValue* left, const SSAttributeKeyValue* right);

	protected:

		SSDrawPart* m_pParentPart;
		const SSModelPart* m_pModelPart;
		const SSAnimationPart* m_pSetupPart;
		const SSAnimationPart* m_pAnimPart;

		SSCellValue          m_cell;          // CELL
		Float3               m_position;      // POSX POSY POSZ
		Float3               m_rotation;      // ROTX ROTY ROTZ
		Float2               m_scale;         // SCLX SCLY
		Float2               m_localScale;    // LSCX LSCY
		float                m_alpha;         // ALPH
		float                m_localAlpha;    // LALP
		int32                m_priority;      // PRIO
		bool                 m_isFlipH;       // FLPH
		bool                 m_isFlipV;       // FLPV
		bool                 m_isHide;        // HIDE
		SSPartColorValue     m_partColor;     // PCOL
		// VCOL
		// SHDR
		SSVertexValue        m_vertex;        // VERT
		Float2               m_pivot;         // PVTX PVTY
		// ANCX ANCY
		Float2               m_size;          // SIZX SIZY
		bool                 m_isImageFlipH;  // IFLH
		bool                 m_isImageFlipV;  // IFLV
		Float2               m_uvTranslate;   // UVTX UVTY
		float                m_uvRotation;    // UVRZ
		Float2               m_uvScale;       // UVSX UVSY
		float                m_boundsRadius;  // BNDR
		int32                m_mask;          // MASK
		// USER
		// SIGN
		SSInstanceParamValue m_instanceParam; // IPRM
		SSEffectValue        m_effect;        // EFCT
		SSDeformVertexValue  m_deform;        // DEFM
		SSAudioValue         m_audio;         // ADIO
		SSTextureChangeValue m_textureChange; // TCHG

		Mat4x4 m_worldMatrix; // 描画用マトリクス

		bool m_isFindSetupHideKey; // setupにHIDEのキーがあるか
		bool m_isFindKeySizeX; // サイズXのキーがあるか
		bool m_isFindKeySizeY; // サイズYのキーがあるか
		bool m_isUsedDeform; // デフォームを使用しているか
	};
}

#endif // !SS_DRAW_PART_H_
