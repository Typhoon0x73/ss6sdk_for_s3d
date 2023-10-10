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

	spritestudio6::SsString animName = "dance";
	auto* anim = animPack->findAnimation(animName);
	if (anim == nullptr)
	{
		return;
	}

	// デコーダー内でスマートポインタ管理される
	spritestudio6::SsCellMapList* pCellmapList = new spritestudio6::SsCellMapList();
	if (not(pCellmapList->preloadTexture(pSsProject.get())))
	{
		return;
	}
	pCellmapList->set(pSsProject.get(), animPack);

	spritestudio6::SsAnimeDecoder decoder;
	decoder.setAnimation(&animPack->Model, anim, pCellmapList, pSsProject.get());

	while (System::Update())
	{
		{
			const Transformer2D trans{ Mat3x2::Translate(Float2{ 200, 600 }) };
			decoder.update();
			decoder.draw();
		}
	}
}

# endif // !defined(USE_SAMPLE_MAIN)
# endif // !defined(USE_TEST_MAIN)
