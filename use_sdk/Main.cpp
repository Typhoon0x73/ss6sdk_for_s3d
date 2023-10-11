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

	Float2 transPos{ Float2::Zero() };
	transPos.x = anim->settings.canvasSize.x * (anim->settings.pivot.x + 0.5f);
	transPos.y = anim->settings.canvasSize.y * (anim->settings.pivot.y - 0.5f) * -1;

	Camera2D camera;
	while (System::Update())
	{
		camera.update();
		{
			const Transformer2D trans{ Mat3x2::Translate(transPos) * Mat3x2::Translate(Float2{ camera.getCenter().x, camera.getCenter().y })};
			decoder.update();
			decoder.draw();
		}
		camera.draw();
	}
}

# endif // !defined(USE_SAMPLE_MAIN)
# endif // !defined(USE_TEST_MAIN)
