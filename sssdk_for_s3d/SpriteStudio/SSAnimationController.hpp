﻿#pragma once
#ifndef SS_ANIMATION_CONTROLLER_H_
#define SS_ANIMATION_CONTROLLER_H_

#include <Siv3D.hpp>

namespace sssdk
{
	class SSProject;
	class SSAnimeSettings;
	class SSDrawPart;

	class SSAnimationController
	{
	public:

		SSAnimationController();
		virtual ~SSAnimationController();

		bool build(SSProject* proj, StringView pack, StringView anim);

		void update(double deltaTime);
		void draw(const Vec2& pos) const;
		void drawAt(const Vec2& pos) const;

		void setTime(double t);
		void setLoop(bool isLoop);

		double getTime() const;
		bool isLoop() const;
		bool isAnimationEnd() const;

	private:

		int32 calcFrame();
		void updatePart(int32 frame);

		double m_tick;
		bool m_isLoop;
		bool m_isAnimationEnd;
		const SSAnimeSettings* m_pAnimeSettings;
		Array<std::unique_ptr<SSDrawPart>> m_drawParts;
	};
}

#endif // !SS_ANIMATION_CONTROLLER_H_