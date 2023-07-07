#ifndef __SSPLAYER_RENDER__
#define __SSPLAYER_RENDER__


#include "../loader/sstypes.h"


namespace spritestudio6
{

struct SsPartState;
struct SsCellValue;

class ISsRenderer
{
public:
	virtual ~ISsRenderer() = default;
	virtual void	initialize() = 0;
	virtual void	renderSetup() = 0;
	virtual void	renderPart(SsPartState* state) = 0;
	virtual void	execMask(SsPartState* state) = 0;
	virtual void	clearMask() = 0;

	virtual void	renderSpriteSimple( float matrix[16], 
										int width, int height, 
										SsVector2& pivot , 
										SsVector2 uv1, SsVector2 uv2, 
										const SsFColor& color ) = 0;

	virtual void	SetAlphaBlendMode(SsBlendType::_enum type)=0;
	virtual void	SetTexture( SsCellValue* cell )=0;
	virtual void	enableMask(bool flag) = 0;

};

class	SsCurrentRenderer
{
private:
	static ISsRenderer*	m_currentrender;
public:
	SsCurrentRenderer(){}
	SsCurrentRenderer(ISsRenderer* render)
	{
		SetCurrentRender(render);
	}
	virtual ~SsCurrentRenderer()
	{
		if ( m_currentrender ) delete m_currentrender;
	}
	static void	SetCurrentRender( ISsRenderer* render )
	{ 
		render->initialize();
		m_currentrender = render; 
	}
	static ISsRenderer*	getRender(){ return m_currentrender; } 



};


}	//	namespace spritestudio6

#endif
