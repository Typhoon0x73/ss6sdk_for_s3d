# if !defined(USE_TEST_MAIN)
# if !defined(USE_SAMPLE_MAIN)

# include <Siv3D.hpp> // OpenSiv3D v0.6.10
# include "ThirdParty/sssdk/Common/Loader/ssloader_sspj.h"
# include "ThirdParty/sssdk/Common/Animator/ssplayer_animedecode.h"
# include "ThirdParty/sssdk/Common/Animator/ssplayer_render.h"
# include "SsDrawer/SsDrawerS3D.h"

//# define USE_TESTDATA
# define USE_WINDOWSIZE_CHANGE

namespace
{
	#ifdef USE_TESTDATA
	static const spritestudio6::SsString SSPJ_PATH = "ss6sample/TestData/allAttributeV7/allAttributeV7.sspj";
	static const spritestudio6::SsString ANIM_PACK = "alpha";
	static const spritestudio6::SsString ANIM_NAME = "anime_1";
	#else // USE_TESTDATA
	static const spritestudio6::SsString SSPJ_PATH = "ss6sample/Ringo/Ringo.sspj";
	static const spritestudio6::SsString ANIM_PACK = "Ringo";
	static const spritestudio6::SsString ANIM_NAME = "dance";
	#endif // USE_TESTDATA 
}

void Main()
{
	Console.open();

	spritestudio6::SsCurrentRenderer::SetCurrentRender(new SsDrawerS3D());

	std::unique_ptr<spritestudio6::SsProject> pSsProject = nullptr;
	pSsProject.reset(spritestudio6::ssloader_sspj::Load(SSPJ_PATH));
	if (pSsProject == nullptr)
	{
		return;
	}

	spritestudio6::SsString animPackName = ANIM_PACK;
	auto* animPack = pSsProject->findAnimationPack(animPackName);
	if (animPack == nullptr)
	{
		return;
	}

	spritestudio6::SsString animName = ANIM_NAME;
	auto* anim = animPack->findAnimation(animName);
	if (anim == nullptr)
	{
		return;
	}

	#ifdef USE_WINDOWSIZE_CHANGE
	Window::Resize((int32)anim->settings.canvasSize.x, (int32)anim->settings.canvasSize.y);
	#endif // USE_WINDOWSIZE_CHANGE

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

	int32 frameCount = 0;

	Camera2D camera;
	while (System::Update())
	{
		camera.update();
		{
			const Transformer2D trans{ Mat3x2::Translate(transPos) * Mat3x2::Translate(Float2{ camera.getCenter().x, camera.getCenter().y })};
			frameCount++;
			decoder.setPlayFrame(frameCount);
			decoder.update(1.0f);
			decoder.draw();
		}
		camera.draw();

		// センタリングチェック用 十字
		{
			Line{ 0.0, Scene::Height() * 0.5, Scene::Width(), Scene::Height() * 0.5 }.draw(ColorF{ Palette::Lightpink, 0.4 });
			Line{ Scene::Width() * 0.5, 0.0, Scene::Width() * 0.5, Scene::Height() }.draw(ColorF{ Palette::Lightpink, 0.4 });
		}
	}
}

# endif // !defined(USE_SAMPLE_MAIN)
# endif // !defined(USE_TEST_MAIN)
