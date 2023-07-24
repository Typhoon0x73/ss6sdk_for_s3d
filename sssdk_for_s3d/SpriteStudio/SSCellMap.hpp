#pragma once
#ifndef SS_CELLMAP_H_
#define SS_CELLMAP_H_

#include <Siv3D.hpp>
#include "SSCell.hpp"

namespace sssdk
{
	class SSProject;
	class SSCellmap
	{
	public:

		explicit SSCellmap(SSProject* project);
		explicit SSCellmap(SSProject* project, FilePathView path);
		virtual ~SSCellmap();

		bool load(FilePathView path);

		SIV3D_NODISCARD_CXX20 StringView getTextureKey() const;
		SIV3D_NODISCARD_CXX20 StringView getVersion() const;

		SIV3D_NODISCARD_CXX20 StringView getName() const;

		SIV3D_NODISCARD_CXX20 FilePathView getImagePath() const;
		SIV3D_NODISCARD_CXX20 const Size& getPixelSize() const;

		SIV3D_NODISCARD_CXX20 TextureAddressMode getWrapMode() const;
		SIV3D_NODISCARD_CXX20 TextureFilter getFilterMode() const;

		SIV3D_NODISCARD_CXX20 const Array<SSCell>& getCells() const;
		SIV3D_NODISCARD_CXX20 const SSCell* const getCell(StringView name) const;

	private:

		SSProject* m_pProject;
		String m_textureKey;

		String m_version;

		String m_name;

		FilePath m_imagePath;
		Size m_pixelSize;

		TextureAddressMode m_wrapMode;
		TextureFilter m_filterMode;

		Array<SSCell> m_cells;
	};
}

#endif // !SS_CELLMAP_H_
