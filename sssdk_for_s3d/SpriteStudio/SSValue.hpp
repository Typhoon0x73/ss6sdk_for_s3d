#pragma once
#ifndef SS_VALUE_H_
#define SS_VALUE_H_

#include <Siv3D.hpp>
#include "SSDefine.hpp"

namespace sssdk
{
	class SSValue;
	using SSValueVariant = std::variant<
		std::monostate,
		String,
		int32,
		float,
		bool,
		HashTable<String, SSValue>,
		Array<SSValue>,
		SSCellValue,
		SSPartColorValue,
		SSVertexValue,
		SSInstanceParamValue,
		SSEffectValue,
		SSDeformVertexValue,
		SSAudioValue,
		SSTextureChangeValue
	>;

	class SSValue
	{
	public:

		enum TYPE
		{
			TYPE_UNKOWN,
			TYPE_STRING,
			TYPE_INT,
			TYPE_FLOAT,
			TYPE_BOOL,
			TYPE_HASH,
			TYPE_ARRAY,
			TYPE_CELL,
			TYPE_PCOL,
			TYPE_VERT,
			TYPE_IPRM,
			TYPE_EFCT,
			TYPE_DEFM,
			TYPE_ADIO,
			TYPE_TCHG,
		};

		explicit SSValue();
		explicit SSValue(ATTRIBUTE_KIND kind, const XMLElement& element);
		explicit SSValue(TYPE type, const XMLElement& element);
		virtual ~SSValue();

		void set(ATTRIBUTE_KIND kind, const XMLElement& element);
		void set(TYPE type, const XMLElement& element);

		TYPE getType() const;

		template< typename T > inline const T* getValue() const
		{
			if (std::holds_alternative<T>(m_value))
			{
				return &(std::get<T>(m_value));
			}
			return nullptr;
		}

	private:

		SSValueVariant m_value;
	};
}

#endif // !SS_VALUE_H_
