#ifndef __ISSEFFECTRENDER__
#define __ISSEFFECTRENDER__

namespace spritestudio6
{

class SsPartState;
class SsEffectModel;

class ISsEffectRenderer
{
public:
	bool				usePreMultiTexture;

	//再生制御用
	bool			m_isPlay;
	bool			m_isPause;
	bool			m_isLoop;

public:
	ISsEffectRenderer(){}
	virtual ~ISsEffectRenderer(){}

	virtual void	update(float delta){}
	virtual void	draw(){}
	virtual void    reload(){}
	virtual void	setSeed( u32 seed ){}


	virtual void	setEffectData(SsEffectModel* data){}
	virtual void	setParentAnimeState( SsPartState* state ){}

	virtual void	setFrame( float frame ){}
	virtual float	getFrame(){return 0.0f;}

	virtual int		getCurrentFPS(){ return 30; }
	virtual void 	debugDraw(){}

	virtual void    play(){ m_isPause = false;m_isPlay=true; }
	virtual void	stop(){ m_isPlay = false;}
	virtual void    pause(){m_isPause = true;m_isPlay=false;}
	virtual void	setLoop(bool flag){ m_isLoop = flag; }
	virtual bool	isplay(){return m_isPlay;}
	virtual bool	ispause(){return m_isPause;}
	virtual bool	isloop(){return m_isLoop;}

};

}	// namespace spritestudio6

#endif
