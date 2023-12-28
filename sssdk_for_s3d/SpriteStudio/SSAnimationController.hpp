#pragma once
#ifndef SS_ANIMATION_CONTROLLER_H_
#define SS_ANIMATION_CONTROLLER_H_

#include <Siv3D.hpp>

namespace sssdk
{
	class SSProject;
	class SSAnimeSettings;
	class SSDrawPart;
	class SSModel;
	class SSAnimationPack;
	class SSAnimation;
	class SSAnimationPart;
	class SSMeshAnimator;

	class SSAnimationController
	{
	public:

		SSAnimationController();
		virtual ~SSAnimationController();

		bool build(const SSProject* proj, StringView pack, StringView anim);

		void update(double deltaTime);
		void draw(const Vec2& pos) const;
		void drawAt(const Vec2& pos) const;

		void setTime(double t);
		void setLoop(bool isLoop);

		double getTime() const;
		bool isLoop() const;
		bool isAnimationEnd() const;

		SSDrawPart* getDrawPart(size_t index);
		const SSDrawPart* getDrawPart(size_t index) const;
		size_t getDrawPartNum() const;

		const SSModel* getModel() const;

	private:

		void createDrawParts();
		void linkPart();
		void sortDrawParts();

	private:

		int32 calcFrame();
		void updatePart(int32 frame);

		const SSProject* m_pProject;
		const SSAnimationPack* m_pAnimationPack;
		const SSAnimation* m_pAnimation;

		HashTable<String, const SSAnimationPart*> m_drawPartTable;
		HashTable<String, const SSAnimationPart*> m_setupDrawPartTable;

		double m_tick;
		bool m_isLoop;
		bool m_isAnimationEnd;
		const SSAnimeSettings* m_pAnimeSettings;
		Array<std::unique_ptr<SSDrawPart>> m_drawParts;
		std::unique_ptr<SSMeshAnimator> m_pMeshAnimator;
	};
}

#endif // !SS_ANIMATION_CONTROLLER_H_
