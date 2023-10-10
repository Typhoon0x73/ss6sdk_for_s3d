# if !defined(USE_TEST_MAIN)
# if !defined(USE_SAMPLE_MAIN)

# include <Siv3D.hpp> // OpenSiv3D v0.6.10
# include "ThirdParty/sssdk/Common/Loader/ssloader_sspj.h"

void Main()
{
	Console.open();

	std::unique_ptr<spritestudio6::SsProject> pSsProject = nullptr;
	pSsProject.reset(spritestudio6::ssloader_sspj::Load("ss6sample/Ringo/Ringo.sspj"));
	if (pSsProject == nullptr)
	{
		return;
	}

	while (System::Update())
	{
	}
}

# endif // !defined(USE_SAMPLE_MAIN)
# endif // !defined(USE_TEST_MAIN)
