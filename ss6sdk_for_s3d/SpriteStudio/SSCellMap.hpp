
# pragma once
# include <Siv3D.hpp>
# include "SSCommon.hpp"
# include "SSCellInfo.hpp"

namespace s3d
{
	class SSCellMap
	{
	public:

		SSCellMap() = default;

		~SSCellMap() = default;

		explicit SSCellMap(FilePathView path, SSCreateEditorParam createEditorParam);

		bool load(FilePathView path, SSCreateEditorParam createEditorParam);

		bool isCreateEditorParam() const;

	private:

		void parseCells(const XMLElement& element);

		Array<SSCellInfo> m_cells{};

		struct EditorParam
		{

		};
		std::unique_ptr<EditorParam> m_pEditorParam{ nullptr };
	};
}
