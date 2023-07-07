#pragma once
#ifndef SS_MODEL_H_
#define SS_MODEL_H_

#include <Siv3D.hpp>
#include "SSModelPart.hpp"
#include "SSBoneItem.hpp"
#include "SSMeshBind.hpp"

namespace sssdk
{
	class SSModel
	{
	public:

		explicit SSModel();
		explicit SSModel(const XMLElement& element);
		virtual ~SSModel();

		bool load(const XMLElement& element);

		SIV3D_NODISCARD_CXX20 const Array<SSModelPart>& getParts() const;
		SIV3D_NODISCARD_CXX20 const SSModelPart* const getPart(int32 index) const;
		SIV3D_NODISCARD_CXX20 const SSModelPart* const getPart(StringView name) const;
		SIV3D_NODISCARD_CXX20 const Array<SSBoneItem>& getBones() const;
		SIV3D_NODISCARD_CXX20 const Array<SSMeshBind>& getMeshBinds() const;

	private:

		Array<SSModelPart> m_parts;
		Array<SSBoneItem> m_bones;
		Array<SSMeshBind> m_meshBinds;
	};
}

#endif // !SS_MODEL_H_
