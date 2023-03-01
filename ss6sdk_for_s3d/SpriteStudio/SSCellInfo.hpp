
# pragma once
# include <Siv3D.hpp>

namespace s3d
{
	class SSCellInfo
	{
	public:

		SSCellInfo() = default;

		~SSCellInfo() = default;

		explicit SSCellInfo(const XMLElement& cell, bool createEditorParam);

		bool load(const XMLElement& cell, bool createEditorParam);

	private:
	};
}
