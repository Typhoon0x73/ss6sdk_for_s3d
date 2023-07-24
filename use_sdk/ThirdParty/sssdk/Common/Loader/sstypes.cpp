#include "sstypes.h"

namespace spritestudio6
{

//---------------------------------------------------------------
//相互変換 SsPartType
SsString	__EnumToString_( SsPartType::_enum n )
{
	if ( SsPartType::invalid == n ) return "invalid";
	if ( SsPartType::null == n) return "null";
	if ( SsPartType::normal == n) return "normal";
	if ( SsPartType::text == n) return "text";
	if ( SsPartType::instance == n) return "instance";
	if ( SsPartType::effect == n) return "effect";


	if (SsPartType::armature == n) return "armature";
	if (SsPartType::mesh == n) return "mesh";
	if (SsPartType::movenode == n) return "movenode";
	if (SsPartType::constraint == n) return "constraint";
	if (SsPartType::mask == n) return "mask";
	if (SsPartType::joint == n) return "joint";
	if (SsPartType::bonepoint == n) return "bonepoint";


	return "invalid";	
}

void 	__StringToEnum_( SsString n , SsPartType::_enum& out)
{
	out =  SsPartType::invalid;
	if ( n == "invalid") out = SsPartType::invalid;
	if ( n == "null") out = SsPartType::null;
	if ( n == "normal") out = SsPartType::normal;
	if ( n == "text") out = SsPartType::text;
	if ( n == "instance") out = SsPartType::instance;
	if ( n == "effect") out = SsPartType::effect;

	if (n == "armature") out = SsPartType::armature;
	if (n == "mesh") out = SsPartType::mesh;
	if (n == "movenode") out = SsPartType::movenode;
	if (n == "constraint") out = SsPartType::constraint;
	if (n == "mask") {
		out = SsPartType::mask;
	}
	if (n == "joint") out = SsPartType::joint;
	if (n == "bonepoint") out = SsPartType::bonepoint;

}

//---------------------------------------------------------------
//相互変換 SsPartsSortMode
SsString	__EnumToString_( SsPartsSortMode::_enum n )
{
	if ( SsPartsSortMode::invalid == n) return "invalid";
	if ( SsPartsSortMode::prio == n) return "prio";
	if ( SsPartsSortMode::z == n) return "z";
	return "invalid";	
}

void	__StringToEnum_( SsString n , SsPartsSortMode::_enum &out )
{
	out = SsPartsSortMode::invalid;
	if ( n == "invalid") out = SsPartsSortMode::invalid;
	if ( n == "prio") out = SsPartsSortMode::prio;
	if ( n == "z") out = SsPartsSortMode::z;
}

//---------------------------------------------------------------
//相互変換 SsBoundsType
SsString	__EnumToString_( SsBoundsType::_enum n )
{
	if ( SsBoundsType::invalid == n) return "invalid";
	if ( SsBoundsType::none == n) return "none";
	if ( SsBoundsType::quad == n) return "quad";
	if ( SsBoundsType::aabb == n) return "aabb";
	if ( SsBoundsType::circle == n) return "circle";
	if ( SsBoundsType::circle_smin == n) return "circle_smin";
	if ( SsBoundsType::circle_smax == n) return "circle_smax";
	return "invalid";	
}

void	__StringToEnum_( SsString n , SsBoundsType::_enum &out )
{
	out = SsBoundsType::invalid;
	if ( n == "invalid") out = SsBoundsType::invalid;
	if ( n == "none") out = SsBoundsType::none;
	if ( n == "quad") out = SsBoundsType::quad;
	if ( n == "aabb") out = SsBoundsType::aabb;
	if ( n == "circle") out = SsBoundsType::circle;
	if ( n == "circle_smin") out = SsBoundsType::circle_smin;
	if ( n == "circle_smax") out = SsBoundsType::circle_smax;
}


//---------------------------------------------------------------
//相互変換 SsBoundsType
SsString	__EnumToString_( SsInheritType::_enum n )
{
	if ( SsInheritType::invalid == n) return "invalid";
	if ( SsInheritType::parent == n) return "parent";
	if ( SsInheritType::self == n) return "self";
	return "invalid";	
}

void	__StringToEnum_( SsString n , SsInheritType::_enum &out )
{
	out = SsInheritType::invalid;
	if ( n == "invalid") out = SsInheritType::invalid;
	if ( n == "parent") out = SsInheritType::parent;
	if ( n == "self") out = SsInheritType::self;
}


//---------------------------------------------------------------
//相互変換 SsBlendType
SsString	__EnumToString_( SsBlendType::_enum n )
{
	if ( SsBlendType::invalid == n) return "invalid";
	if ( SsBlendType::mix == n) return "mix";
	if ( SsBlendType::mul == n) return "mul";
	if ( SsBlendType::add == n) return "add";
	if ( SsBlendType::sub == n) return "sub";
	if ( SsBlendType::mulalpha == n) return "mulalpha";
	if ( SsBlendType::screen == n) return "screen";
	if ( SsBlendType::exclusion == n) return "exclusion";
	if ( SsBlendType::invert == n) return "invert";

	return "invalid";	
}

void	__StringToEnum_( SsString n , SsBlendType::_enum &out )
{
	out = SsBlendType::invalid;
	if ( n == "invalid") out = SsBlendType::invalid;
	if ( n == "mix") out = SsBlendType::mix;
	if ( n == "mul") out = SsBlendType::mul;
	if ( n == "add") out = SsBlendType::add;
	if ( n == "sub") out = SsBlendType::sub;
	if ( n == "mulalpha") out = SsBlendType::mulalpha;
	if ( n == "screen") out = SsBlendType::screen;
	if ( n == "exclusion") out = SsBlendType::exclusion;
	if ( n == "invert") out = SsBlendType::invert;
}




//---------------------------------------------------------------
//相互変換 SsInterpolationType
SsString	__EnumToString_( SsInterpolationType::_enum n )
{
	if ( SsInterpolationType::invalid == n)		return "invalid";
	if ( SsInterpolationType::none == n)		return "none";
	if ( SsInterpolationType::linear == n)		return "linear";
	if ( SsInterpolationType::hermite == n)		return "hermite";
	if ( SsInterpolationType::bezier == n)		return "bezier";
	if ( SsInterpolationType::acceleration == n) return "acceleration";
	if ( SsInterpolationType::deceleration == n) return "deceleration";

	return "none";	
}

void	__StringToEnum_( SsString n , SsInterpolationType::_enum &out )
{
	out = SsInterpolationType::none;
	if ( n == "invalid") out = SsInterpolationType::invalid;
	if ( n == "none") out = SsInterpolationType::none;
	if ( n == "linear") out = SsInterpolationType::linear;
	if ( n == "hermite") out = SsInterpolationType::hermite;
	if ( n == "bezier") out = SsInterpolationType::bezier;
	if ( n == "acceleration") out = SsInterpolationType::acceleration;
	if ( n == "deceleration") out = SsInterpolationType::deceleration;
}

//---------------------------------------------------------------
//相互変換 SsTexWrapMode
SsString	__EnumToString_( SsTexWrapMode::_enum n )
{
	if ( SsTexWrapMode::invalid == n)		return "invalid";
	if ( SsTexWrapMode::clamp == n)		return "clamp";
	if ( SsTexWrapMode::repeat == n)		return "repeat";
	if ( SsTexWrapMode::mirror == n)		return "mirror";

	return "invalid";	
}

void	__StringToEnum_( SsString n , SsTexWrapMode::_enum &out )
{
	out = SsTexWrapMode::invalid;
	if ( n == "invalid") out = SsTexWrapMode::invalid;
	if ( n == "clamp") out = SsTexWrapMode::clamp;
	if ( n == "repeat") out = SsTexWrapMode::repeat;
	if ( n == "mirror") out = SsTexWrapMode::mirror;

}

//---------------------------------------------------------------
//相互変換 SsTexFilterMode
SsString	__EnumToString_( SsTexFilterMode::_enum n )
{
	if ( SsTexFilterMode::invalid == n)		return "invalid";
	if ( SsTexFilterMode::nearlest == n)		return "nearlest";
	if ( SsTexFilterMode::linear == n)		return "linear";

	return "invalid";	
}

void	__StringToEnum_( SsString n , SsTexFilterMode::_enum &out )
{
	out = SsTexFilterMode::invalid;
	if ( n == "invalid") out = SsTexFilterMode::invalid;
	if ( n == "nearlest") out = SsTexFilterMode::nearlest;
	if ( n == "linear") out = SsTexFilterMode::linear;

}

//---------------------------------------------------------------
//相互変換 SsTexFilterMode
SsString	__EnumToString_( SsColorBlendTarget::_enum n )
{
	if ( SsColorBlendTarget::invalid == n)		return "invalid";
	if ( SsColorBlendTarget::whole == n)		return "whole";
	if ( SsColorBlendTarget::vertex == n)		return "vertex";

	return "invalid";	
}

void	__StringToEnum_( SsString n , SsColorBlendTarget::_enum &out )
{
	out = SsColorBlendTarget::invalid;
	if ( n == "invalid") out = SsColorBlendTarget::invalid;
	if ( n == "whole") out = SsColorBlendTarget::whole;
	if ( n == "vertex") out = SsColorBlendTarget::vertex;

}

//---------------------------------------------------------------
//相互変換 SsAttributeKind
SsString	__EnumToString_( SsAttributeKind::_enum n )
{
	if ( SsAttributeKind::invalid == n)		return "invalid";
	if ( SsAttributeKind::cell == n)		return "CELL";
	if ( SsAttributeKind::posx == n)		return "POSX";
	if ( SsAttributeKind::posy == n)		return "POSY";
	if ( SsAttributeKind::posz == n)		return "POSZ";
	if ( SsAttributeKind::rotx == n)		return "ROTX";
	if ( SsAttributeKind::roty == n)		return "ROTY";
	if ( SsAttributeKind::rotz == n)		return "ROTZ";
	if ( SsAttributeKind::sclx == n)		return "SCLX";
	if ( SsAttributeKind::scly == n)		return "SCLY";
	if ( SsAttributeKind::losclx == n)		return "LSCX";
	if ( SsAttributeKind::loscly == n)		return "LSCY";
	if ( SsAttributeKind::alpha == n)		return "ALPH";
	if ( SsAttributeKind::loalpha == n)		return "LALP";
	if ( SsAttributeKind::prio == n)		return "PRIO";
	if ( SsAttributeKind::fliph == n)		return "FLPH";
	if ( SsAttributeKind::flipv == n)		return "FLPV";
	if ( SsAttributeKind::hide == n)		return "HIDE";
	if ( SsAttributeKind::partsColor == n)	return "PCOL";
	if ( SsAttributeKind::color == n)		return "VCOL";
	if ( SsAttributeKind::shader == n)		return "SHDR";
	if ( SsAttributeKind::vertex == n)		return "VERT";
	if ( SsAttributeKind::pivotx == n)		return "PVTX";
	if ( SsAttributeKind::pivoty == n)		return "PVTY";
	if ( SsAttributeKind::anchorx == n)		return "ANCX";
	if ( SsAttributeKind::anchory == n)		return "ANCY";
	if ( SsAttributeKind::sizex == n)		return "SIZX";
	if ( SsAttributeKind::sizey == n)		return "SIZY";
	if ( SsAttributeKind::imgfliph == n)	return "IFLH";
	if ( SsAttributeKind::imgflipv == n)	return "IFLV";
	if ( SsAttributeKind::uvtx == n)		return "UVTX";
	if ( SsAttributeKind::uvty == n)		return "UVTY";
	if ( SsAttributeKind::uvrz == n)		return "UVRZ";
	if ( SsAttributeKind::uvsx == n)		return "UVSX";
	if ( SsAttributeKind::uvsy == n)		return "UVSY";
	if ( SsAttributeKind::mask == n)		return "MASK";
	if ( SsAttributeKind::boundr == n)		return "BNDR";
	if ( SsAttributeKind::user == n)		return "USER";
	if ( SsAttributeKind::signal == n)		return "SIGN";
	if ( SsAttributeKind::instance == n)    return "IPRM";
	if ( SsAttributeKind::mask == n)	    return "MASK";
	if ( SsAttributeKind::deform == n)	    return "DEFM";



	return "invalid";	
}

void	__StringToEnum_( SsString n , SsAttributeKind::_enum &out )
{

	out = SsAttributeKind::invalid;
	if ( n == "invalid") out = SsAttributeKind::invalid;
	if ( n == "CELL") out = SsAttributeKind::cell;
	if ( n == "POSX") out = SsAttributeKind::posx;
	if ( n == "POSY") out = SsAttributeKind::posy;
	if ( n == "POSZ") out = SsAttributeKind::posz;
	if ( n == "ROTX") out = SsAttributeKind::rotx;
	if ( n == "ROTY") out = SsAttributeKind::roty;
	if ( n == "ROTZ") out = SsAttributeKind::rotz;
	if ( n == "SCLX") out = SsAttributeKind::sclx;
	if ( n == "SCLY") out = SsAttributeKind::scly;
	if ( n == "LSCX") out = SsAttributeKind::losclx;
	if ( n == "LSCY") out = SsAttributeKind::loscly;
	if ( n == "ALPH") out = SsAttributeKind::alpha;
	if ( n == "LALP") out = SsAttributeKind::loalpha;
	if ( n == "PRIO") out = SsAttributeKind::prio;
	if ( n == "FLPH") out = SsAttributeKind::fliph;
	if ( n == "FLPV") out = SsAttributeKind::flipv;
	if ( n == "HIDE") out = SsAttributeKind::hide;
	if ( n == "PCOL") out = SsAttributeKind::partsColor;
	if ( n == "VCOL") out = SsAttributeKind::color;
	if ( n == "SHDR") out = SsAttributeKind::shader;
	if ( n == "VERT") out = SsAttributeKind::vertex;
	if ( n == "PVTX") out = SsAttributeKind::pivotx;
	if ( n == "PVTY") out = SsAttributeKind::pivoty;
	if ( n == "ANCX") out = SsAttributeKind::anchorx;
	if ( n == "ANCY") out = SsAttributeKind::anchory;
	if ( n == "SIZX") out = SsAttributeKind::sizex;
	if ( n == "SIZY") out = SsAttributeKind::sizey;
	if ( n == "IFLH") out = SsAttributeKind::imgfliph;
	if ( n == "IFLV") out = SsAttributeKind::imgflipv;
	if ( n == "UVTX") out = SsAttributeKind::uvtx;
	if ( n == "UVTY") out = SsAttributeKind::uvty;
	if ( n == "UVRZ") out = SsAttributeKind::uvrz;
	if ( n == "UVSX") out = SsAttributeKind::uvsx;
	if ( n == "UVSY") out = SsAttributeKind::uvsy;
	if ( n == "BNDR") out = SsAttributeKind::boundr;
	if ( n == "USER") out = SsAttributeKind::user;
	if ( n == "SIGN") out = SsAttributeKind::signal;
	if ( n == "IPRM") out = SsAttributeKind::instance;
	if ( n == "EFCT") out = SsAttributeKind::effect;
	if ( n == "MASK") out = SsAttributeKind::mask;
	if ( n == "DEFM") out = SsAttributeKind::deform;


}


SPRITESTUDIO6SDK_DECLARE_ENUM_STRING_DEF( SsEffectNodeType );

//---------------------------------------------------------------
//相互変換 SsPartType
SsString	__EnumToString_( SsEffectNodeType::_enum n )
{
	if ( n == SsEffectNodeType::invalid ) return "invalid";
	if ( n == SsEffectNodeType::root ) return "Root";
	if ( n == SsEffectNodeType::emmiter ) return "Emmiter";
	if ( n == SsEffectNodeType::particle ) return "Prticle";

	return "invalid";	
}

void 	__StringToEnum_( SsString n , SsEffectNodeType::_enum& out)
{
	out =  SsEffectNodeType::invalid;
	if ( n == "invalid") out = SsEffectNodeType::invalid;
	if ( n == "Root") out = SsEffectNodeType::root;
	if ( n == "Emmiter") out = SsEffectNodeType::emmiter;
	if ( n == "Particle") out = SsEffectNodeType::particle;
}


SPRITESTUDIO6SDK_DECLARE_ENUM_STRING_DEF( SsRenderBlendType );

//---------------------------------------------------------------
//相互変換 SsPartType
SsString	__EnumToString_( SsRenderBlendType::_enum n )
{
	if ( n == SsRenderBlendType::invalid ) return "invalid";
	if ( n == SsRenderBlendType::Mix ) return "Mix";
	if ( n == SsRenderBlendType::Add ) return "Add";

	return "invalid";	
}

void 	__StringToEnum_( SsString n , SsRenderBlendType::_enum& out)
{
	out =  SsRenderBlendType::invalid;
	if ( n == "invalid") out = SsRenderBlendType::invalid;
	if ( n == "Mix") out = SsRenderBlendType::Mix;
	if ( n == "Add") out = SsRenderBlendType::Add;
}

//---------------------------------------------------------------
//相互変換 SsMeshDivType1
SsString	__EnumToString_(SsMeshDivType::_enum n)
{ 
	if (n == SsMeshDivType::unknown) return "unknown";
	if (n == SsMeshDivType::polyline_base) return "polyline_base";
	if (n == SsMeshDivType::boxdiv) return "boxdiv";

	return "unknown";
}

void 	__StringToEnum_(SsString n, SsMeshDivType::_enum& out)
{
	out = SsMeshDivType::unknown;
	if (n == "unknown") out = SsMeshDivType::unknown;
	if (n == "polyline_base") out = SsMeshDivType::polyline_base;
	if (n == "boxdiv") out = SsMeshDivType::boxdiv;
}

//---------------------------------------------------------------
//相互変換 SsIkRotationArrow
SsString	__EnumToString_(SsIkRotationArrow::_enum n)
{
	if (n == SsIkRotationArrow::arrowfree) return "arrowfree";
	if (n == SsIkRotationArrow::clockwise) return "clockwise";
	if (n == SsIkRotationArrow::anticlockwise) return "anticlockwise";

	return "unknown";
}

void 	__StringToEnum_(SsString n, SsIkRotationArrow::_enum& out)
{
	out = SsIkRotationArrow::arrowfree;
	if (n == "arrowfree") out = SsIkRotationArrow::arrowfree;
	if (n == "clockwise") out = SsIkRotationArrow::clockwise;
	if (n == "anticlockwise") out = SsIkRotationArrow::anticlockwise;
}

//---------------------------------------------------------------
//相互変換 SsSequenceType
SsString	__EnumToString_(SsSequenceType::_enum n)
{
	if (n == SsSequenceType::last) return "LAST";
	if (n == SsSequenceType::keep) return "KEEP";
	if (n == SsSequenceType::top) return "TOP";

	return "invalid";
}

void 	__StringToEnum_(SsString n, SsSequenceType::_enum& out)
{
	out = SsSequenceType::invalid;
	if (n == "LAST") out = SsSequenceType::last;
	if (n == "KEEP") out = SsSequenceType::keep;
	if (n == "TOP") out = SsSequenceType::top;
}

//相互変換 SsSignalParamType
SsString	__EnumToString_(SsSignalParamType::_enum n)
{
	if (n == SsSignalParamType::index) return "index";
	if (n == SsSignalParamType::integer) return "integer";
	if (n == SsSignalParamType::floating) return "floating";

	return "none";
}

void 	__StringToEnum_(SsString n, SsSignalParamType::_enum& out)
{
	out = SsSignalParamType::none;
	if (n == "index") out = SsSignalParamType::index;
	if (n == "integer") out = SsSignalParamType::integer;
	if (n == "floating") out = SsSignalParamType::floating;
}


}	// namespace spritestudio6
