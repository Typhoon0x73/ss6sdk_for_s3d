#include "../Loader/ssloader.h"
#include "ssplayer_animedecode.h"
#include "ssplayer_PartState.h"

namespace spritestudio6
{


SsPartState::SsPartState() :
	refAnime(),
	index(-1),
	parent(nullptr),
	noCells(false),
	alphaBlendType(SsBlendType::invalid),
	refEffect(),
	meshPart()
{
	init();
	effectValue.attrInitialized = false;
}

SsPartState::~SsPartState(){
	destroy();
}


void	SsPartState::destroy()
{
	if ( refAnime ) refAnime.reset();
	if ( refEffect ) refEffect.reset();
	if ( meshPart ) meshPart.reset();
}

void	SsPartState::init()
{
	memset( vertices , 0 , sizeof( vertices ) );
	memset( colors , 0 , sizeof( colors ) );
	memset( uvs , 0 , sizeof( uvs ) );
	memset( matrix , 0 , sizeof( matrix ) );
	//cell = 0;
	position = SsVector3( 0.0f , 0.0f, 0.0f );
	rotation = SsVector3( 0.0f , 0.0f , 0.0f );
	scale = SsVector2( 1.0f , 1.0f );
	localscale = SsVector2(1.0f, 1.0f);

	alpha = 1.0f;
	localalpha = 1.0f;
	prio = 0;
	hFlip = false;
	vFlip = false;
	hide = false;

	pivotOffset = SsVector2(0, 0);
	anchor = SsVector2( 0 , 0 );
	size = SsVector2( 1 , 1 );

	imageFlipH = false;
	imageFlipV = false;
	uvTranslate = SsVector2(0, 0);
	uvRotation = 0;
	uvScale = SsVector2( 1 , 1 );

	boundingRadius = 0;

	is_parts_color = false;
	is_color_blend = false;
	is_shader = false;
	is_vertex_transform = false;
	inheritRates = 0; 

	is_localAlpha = false;
	is_defrom = false;
	
	effectValue.independent = false;
	effectValue.loopflag = 0;
	effectValue.speed = 1.0f;
	effectValue.startTime = 0;
	effectValue.curKeyframe = 0;

	effectseed = 0;
	effectTime = 0;

	instanceValue.infinity = false;
	instanceValue.reverse = false;
	instanceValue.pingpong = false;
	instanceValue.independent = false;
	instanceValue.loopflag = 0;
	instanceValue.loopNum = 1;
	instanceValue.startLabel = "_start";
	instanceValue.startOffset = 0;
	instanceValue.endLabel = "_end";
	instanceValue.endOffset = 0;
	instanceValue.curKeyframe = 0;
	instanceValue.speed = 1.0f;
	instanceValue.startFrame = 0;
	instanceValue.endFrame = 0;
//	instanceValue.liveFrame = 0.0f;	//加算値なので初期化してはいけない
	
	masklimen = 0;
//	partType = SsPartType::normal;
//	maskInfluence = false;

}


void	SsPartState::reset()
{
	effectValue.independent = false;
	effectValue.attrInitialized = false;
	effectValue.speed = 1.0f;
	effectValue.startTime = 0;

}

}	// namespace spritestudio6
