
# pragma once
# include <Siv3D.hpp>

namespace s3d
{

	class SSBackgroundSettings
	{
	public:

		SSBackgroundSettings() = default;

		~SSBackgroundSettings() = default;

	private:

		FilePath m_imagePath  { U"" };
		int32    m_imageDisp  { 0 };
		Point    m_imageOffset{ 0, 0 };
		Size     m_imageCanvas{ 0, 0 };
		Float2   m_imagePivot { 0.0f, 0.0f };
	};
}
