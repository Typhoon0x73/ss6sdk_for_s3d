# include <Siv3D.hpp> // OpenSiv3D v0.6.10
# include "SpriteStudio/SSProject.hpp"
# include "SpriteStudio/SSAnimationController.hpp"

void Main()
{
	Window::Resize(Size{ 1280, 720 });

	FilePath sspjFile = U"ss6sample/Ringo/Ringo.sspj";
	sssdk::SSProject ssProject;
	sssdk::SSAnimationController animController;
	if (not ssProject.load(sspjFile))
	{
		return;
	}
	if (not animController.build(&ssProject, U"Ringo", U"attack1"))
	{
		return;
	}
	animController.setLoop(true);

	while (System::Update())
	{
		animController.update(Scene::DeltaTime());
		animController.draw(Vec2{ 0, 0 });
	}
}
