
#include "SSProject.hpp"
#include "SSCellmap.hpp"

namespace sssdk
{
	SSCellmap::SSCellmap(SSProject* project)
		: m_pProject{ project }
		, m_textureKey{ U"" }
		, m_version{ U"" }
		, m_name{ U"" }
		, m_imagePath{ U"" }
		, m_pixelSize{ 0, 0 }
		, m_wrapMode{ TextureAddressMode::Clamp }
		, m_filterMode{ TextureFilter::Linear }
	{
	}

	SSCellmap::SSCellmap(SSProject* project, FilePathView path)
		: SSCellmap(project)
	{
		load(path);
	}

	SSCellmap::~SSCellmap()
	{
	}

	bool SSCellmap::load(FilePathView path)
	{
		XMLReader reader{ path };
		if (not reader)
		{
			return false;
		}
		for (const auto& attribute : reader.attributes())
		{
			if (attribute.first == U"version")
			{
				m_version = attribute.second;
			}
		}
		for (auto child = reader.firstChild(); child; child = child.nextSibling())
		{
			const auto& name = child.name();
			if (name == U"name")
			{
				m_name = child.text();
			}
			else if (name == U"imagePath")
			{
				m_imagePath = child.text();
			}
			else if (name == U"pixelSize")
			{
				const auto& lines = child.text().split(U' ');
				m_pixelSize.x = ParseOr<int32>(lines[0], 0);
				m_pixelSize.y = ParseOr<int32>(lines[1], 0);
			}
			else if (name == U"wrapMode")
			{
				const auto& text = child.text();
				if (text == U"repeat")
				{
					m_wrapMode = TextureAddressMode::Repeat;
				}
				else if (text == U"mirror")
				{
					m_wrapMode = TextureAddressMode::Mirror;
				}
				else if (text == U"clamp")
				{
					m_wrapMode = TextureAddressMode::Clamp;
				}
				else if (text == U"border")
				{
					m_wrapMode = TextureAddressMode::Border;
				}
			}
			else if (name == U"filterMode")
			{
				const auto& text = child.text();
				if (text == U"nearest")
				{
					m_filterMode = TextureFilter::Nearest;
				}
				else if (text == U"linear")
				{
					m_filterMode = TextureFilter::Linear;
				}
			}
			else if (name == U"cells")
			{
				for (auto cell = child.firstChild(); cell; cell = cell.nextSibling())
				{
					m_cells.emplace_back(cell);
				}
			}
		}
		const auto& imageDir = m_pProject->getImageFileDirectory();
		const auto& texturePath = FileSystem::PathAppend(imageDir, m_imagePath);
		m_textureKey = m_pProject->getName() + U".sspj_" + m_name + U".ssce_" + m_imagePath;
		if (not TextureAsset::Register(m_textureKey, texturePath))
		{
			return false;
		}
		return true;
	}

	StringView SSCellmap::getTextureKey() const
	{
		return m_textureKey;
	}

	StringView SSCellmap::getVersion() const
	{
		return m_version;
	}

	StringView SSCellmap::getName() const
	{
		return m_name;
	}

	FilePathView SSCellmap::getImagePath() const
	{
		return m_imagePath;
	}

	const Size& SSCellmap::getPixelSize() const
	{
		return m_pixelSize;
	}

	TextureAddressMode SSCellmap::getWrapMode() const
	{
		return m_wrapMode;
	}

	TextureFilter SSCellmap::getFilterMode() const
	{
		return m_filterMode;
	}

	const Array<SSCell>& SSCellmap::getCells() const
	{
		return m_cells;
	}

	const SSCell* const SSCellmap::getCell(StringView name) const
	{
		for (const auto& cell : m_cells)
		{
			if (cell.getName() == name)
			{
				return &cell;
			}
		}
		return nullptr;
	}
}
