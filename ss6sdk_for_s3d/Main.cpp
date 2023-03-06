# if !defined(USE_TEST_MAIN)
# if !defined(USE_SAMPLE_MAIN)

# include <Siv3D.hpp> // OpenSiv3D v0.6.6
# include "SpriteStudio/SSProjectDecoder.hpp"

void Main()
{
	FilePath sspjFile = U"ss6sample/Ringo/Ringo.sspj";
	SSProjectDecoder ssDecoder;
	if (not ssDecoder.load(sspjFile, SSCreateEditorParam::NoCreate))
	{
		return;
	}

	while (System::Update())
	{
	}
}

# endif // !defined(USE_SAMPLE_MAIN)
# endif // !defined(USE_TEST_MAIN)
