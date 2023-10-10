# if !defined(USE_TEST_MAIN)
# if !defined(USE_SAMPLE_MAIN)

# include <Siv3D.hpp> // OpenSiv3D v0.6.10
# include "ThirdParty/sssdk/Common/Loader/ssloader_sspj.h"
# include "ThirdParty/sssdk/Common/Animator/ssplayer_animedecode.h"
# include "ThirdParty/sssdk/Common/Animator/ssplayer_render.h"
# include "SsDrawer/SsDrawerS3D.h"

void Main()
{
	Console.open();

	spritestudio6::SsCurrentRenderer::SetCurrentRender(new SsDrawerS3D());

	std::unique_ptr<spritestudio6::SsProject> pSsProject = nullptr;
	pSsProject.reset(spritestudio6::ssloader_sspj::Load("ss6sample/Ringo/Ringo.sspj"));
	if (pSsProject == nullptr)
	{
		return;
	}

	spritestudio6::SsString animPackName = "Ringo";
	auto* animPack = pSsProject->findAnimationPack(animPackName);
	if (animPack == nullptr)
	{
		return;
	}

	spritestudio6::SsString animName = "attack1";
	auto* anim = animPack->findAnimation(animName);
	if (anim == nullptr)
	{
		return;
	}

	spritestudio6::SsAnimeDecoder decoder;
	spritestudio6::SsCellMapList cellmapList;
	cellmapList.set(pSsProject.get(), animPack);
	decoder.setAnimation(&animPack->Model, anim, &cellmapList, pSsProject.get());

	while (System::Update())
	{
		decoder.update();
		decoder.draw();
	}
}

# endif // !defined(USE_SAMPLE_MAIN)
# endif // !defined(USE_TEST_MAIN)
