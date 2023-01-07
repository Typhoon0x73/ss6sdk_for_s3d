#include "ssloader.h"
#include "sstypes.h"
#include "ssattribute.h"
//#include "../Helper/DebugPrint.h"

namespace spritestudio6
{


const SsKeyframe*	SsAttribute::firstKey()
{
	if ( key.empty() )
		return 0;
		
	return key_dic.begin()->second;

}

///時間から左側のキーを取得
const SsKeyframe*	SsAttribute::findLeftKey( int time )
{

	static SsKeyframe ret;
	if ( key.empty() )
		return 0;
    
	AttributeKeyDic::const_iterator it = key_dic.lower_bound(time);
        
    if ( it == key_dic.end())
    {
		it = --it;
        SsKeyframe* keyframe = it->second;
        return keyframe;
    }
    
		
	if ( it->first == time ) return it->second;
	if ( key_dic.begin() == it )
	{
		if ( it->first > time ) return 0;
	}else{
		it = --it;
	}

	if ( it->first > time ) return 0;

	return it->second;
}

//時間から右側のキーを取得する
const SsKeyframe*	SsAttribute::findRightKey( int time )
{

	static SsKeyframe ret;
	if ( key.empty() )
		return 0;

	AttributeKeyDic::const_iterator it = key_dic.upper_bound(time);
	if (it == key_dic.end())
		return 0;
	return it->second;

}


//頂点カラーアニメデータの取得
void	GetSsPartsColorValue( const SsKeyframe* key , SsPartsColorAnime& v )
{
	SsColorBlendTarget::_enum target;
	__StringToEnum_( key->value["target"].get<SsString>() , target );
		SsBlendType::_enum blendtype;
	__StringToEnum_( key->value["blendType"].get<SsString>() , blendtype);

	v.blendType = blendtype;
	v.target = target;

	if ( target == SsColorBlendTarget::vertex )
	{
		SsHash lt = key->value["LT"].get<SsHash>();
		SsHash rt = key->value["RT"].get<SsHash>();
		SsHash lb = key->value["LB"].get<SsHash>();
		SsHash rb = key->value["RB"].get<SsHash>();

		ConvertStringToSsColor( lt["rgba"].get<SsString>() , v.colors[0].rgba);
		v.colors[0].rate = lt["rate"].get<float>();

		ConvertStringToSsColor( rt["rgba"].get<SsString>() , v.colors[1].rgba);
		v.colors[1].rate = rt["rate"].get<float>();

		ConvertStringToSsColor( lb["rgba"].get<SsString>() , v.colors[2].rgba);
		v.colors[2].rate = lb["rate"].get<float>();

		ConvertStringToSsColor( rb["rgba"].get<SsString>() , v.colors[3].rgba);
		v.colors[3].rate = rb["rate"].get<float>();

	}else{
		SsHash color = key->value["color"].get<SsHash>();

		ConvertStringToSsColor( color["rgba"].get<SsString>() , v.color.rgba);
		v.color.rate = color["rate"].get<float>();
	}

}

//頂点カラーアニメデータの取得
void	GetSsColorValue( const SsKeyframe* key , SsColorAnime& v )
{
	SsColorBlendTarget::_enum target;
	__StringToEnum_( key->value["target"].get<SsString>() , target );
		SsBlendType::_enum blendtype;
	__StringToEnum_( key->value["blendType"].get<SsString>() , blendtype);

	v.blendType = blendtype;
	v.target = target;

	if ( target == SsColorBlendTarget::vertex )
	{
		SsHash lt = key->value["LT"].get<SsHash>();
		SsHash rt = key->value["RT"].get<SsHash>();
		SsHash lb = key->value["LB"].get<SsHash>();
		SsHash rb = key->value["RB"].get<SsHash>();

		ConvertStringToSsColor( lt["rgba"].get<SsString>() , v.colors[0].rgba);
		v.colors[0].rate = lt["rate"].get<float>();

		ConvertStringToSsColor( rt["rgba"].get<SsString>() , v.colors[1].rgba);
		v.colors[1].rate = rt["rate"].get<float>();

		ConvertStringToSsColor( lb["rgba"].get<SsString>() , v.colors[2].rgba);
		v.colors[2].rate = lb["rate"].get<float>();

		ConvertStringToSsColor( rb["rgba"].get<SsString>() , v.colors[3].rgba);
		v.colors[3].rate = rb["rate"].get<float>();

	}else{
		SsHash color = key->value["color"].get<SsHash>();

		ConvertStringToSsColor( color["rgba"].get<SsString>() , v.color.rgba);
		v.color.rate = color["rate"].get<float>();
	}

}

//シェーダーアニメデータの取得
void	GetSsShaderValue( const SsKeyframe* key , SsShaderAnime& v )
{
	v.id = key->value["id"].get<SsString>();
	v.param[0] = key->value["param0"].get<float>();
	v.param[1] = key->value["param1"].get<float>();
	v.param[2] = key->value["param2"].get<float>();
	v.param[3] = key->value["param3"].get<float>();

}

void	GetSsVertexAnime( const SsKeyframe* key , SsVertexAnime& v )
{
	const SsString& sLT = key->value["LT"].get<SsString>();
	const SsString& sRT = key->value["RT"].get<SsString>();
	const SsString& sLB = key->value["LB"].get<SsString>();
	const SsString& sRB = key->value["RB"].get<SsString>();
	
	StringToPoint2( sLT , v.offsets[0] );
	StringToPoint2( sRT , v.offsets[1] );
	StringToPoint2( sLB , v.offsets[2] );
	StringToPoint2( sRB , v.offsets[3] );

}

void	GetSsRefCell( const SsKeyframe* key , SsRefCell& v )
{
	int id = key->value["mapId"].get<int>();
	SsString name = key->value["name"].get<SsString>();

	v.mapid = id;
	v.name = name;
}


void	GetSsUserDataAnime( const SsKeyframe* key , SsUserDataAnime& v )
{
	v.integer = 0;
	v.point.x = v.point.y = 0;
	v.rect.x = v.rect.y = v.rect.w = v.rect.h = 0; 
	v.string = "";
	v.useInteger = key->value.IsExistHashkey("integer");
	v.usePoint = key->value.IsExistHashkey("point");
	v.useRect = key->value.IsExistHashkey("rect");
	v.useString = key->value.IsExistHashkey("string");

	if ( v.useInteger )
	{
		v.integer = key->value["integer"].get<int>();
	}

	if ( v.usePoint )
	{
		const SsString& str = key->value["point"].get<SsString>();
		StringToPoint2( str , v.point );
	}
	
	if ( v.useRect )
	{
		const SsString& str = key->value["rect"].get<SsString>();
		StringToIRect( str , v.rect );
	}

	if ( v.useString )
	{
		const SsString& str = key->value["string"].get<SsString>();
		v.string = str;
	}

}

void	GetSsSignalAnime( const SsKeyframe* key , SsSignalAttr& v )
{
	const SsHash& commands = key->value["commands"].get<SsHash>();

	v.commands.clear();

	for ( SsHash::const_iterator itr = commands.begin(); itr != commands.end(); itr++ ) {
		SsSignalCommand command;
		const SsValue& val = itr->second;
		const SsHash& params = val["params"].get<SsHash>();

		command.active = val["active"].get<bool>();
		command.commandId = val["commandId"].get<SsString>();
		command.note = val["note"].get<SsString>();

		command.params.clear();

		for ( SsHash::const_iterator itr2 = params.begin(); itr2 != params.end(); itr2++ ) {
			SsSignalParam param;
			const SsValue& val2 = itr2->second;
			SsSignalParamType::_enum type;
//			SsSignalParamValue value;

			param.paramId = val2["paramId"].get<SsString>();

			__StringToEnum_( val2["type"].get<SsString>() , type );

			param.type = type;

			switch ( type ) {
			case SsSignalParamType::none :
				param.value.i = 0;
				break;
			case SsSignalParamType::index :
				param.value.i = val2["value"].get<int>();
				break;
			case SsSignalParamType::integer :
				param.value.i = val2["value"].get<int>();
				break;
			case SsSignalParamType::floating :
				param.value.f = val2["value"].get<float>();
				break;
			default:
				param.value.i = 0;
				break;
			}

			command.params.push_back( param );
		}

		v.commands.push_back( command );
	}

}


void	GetSsEffectParamAnime( const SsKeyframe* key , SsEffectAttr& v )
{

	v.startTime = key->value["startTime"].get<int>();
	v.speed = key->value["speed"].get<float>();
	v.independent = key->value["independent"].get<bool>();
	v.curKeyframe = key->time;

	int iflags = 0;
	if (v.independent)
	{
		iflags = iflags | EFFECT_LOOP_FLAG_INFINITY;
	}
	v.loopflag = iflags;

}

void	GetSsInstparamAnime( const SsKeyframe* key , SsInstanceAttr& v )
{
	const SsString& sstartLabel = key->value["startLabel"].get<SsString>();
	const int& sstartOffset = key->value["startOffset"].get<int>();
	const SsString& sendLabel = key->value["endLabel"].get<SsString>();
	const int& sendOffset = key->value["endOffset"].get<int>();

	const float& sspeed = key->value["speed"].get<float>();

	const int& sloopNum = key->value["loopNum"].get<int>();
	const bool& sinfinity = key->value["infinity"].get<bool>();
	const bool& sreverse = key->value["reverse"].get<bool>();
	const bool& spingpong = key->value["pingpong"].get<bool>();
	const bool& sindependent = key->value["independent"].get<bool>();


	v.startLabel = sstartLabel;
	v.startOffset = sstartOffset;
	v.endLabel = sendLabel;
	v.endOffset = sendOffset;
	v.speed = sspeed;

	v.loopNum = sloopNum;
	v.infinity = sinfinity;
	v.reverse = sreverse;
	v.pingpong = spingpong;
	v.independent = sindependent;
	v.curKeyframe = key->time;

	int iflags = 0;
	if (sinfinity)
	{
		iflags = iflags | INSTANCE_LOOP_FLAG_INFINITY;
	}
	if (sreverse)
	{
		iflags = iflags | INSTANCE_LOOP_FLAG_REVERSE;
	}
	if (spingpong)
	{
		iflags = iflags | INSTANCE_LOOP_FLAG_PINGPONG;
	}
	if (sindependent)
	{
		iflags = iflags | INSTANCE_LOOP_FLAG_INDEPENDENT;
	}
	v.loopflag = iflags;

}

//デフォームアニメデータの取得
void	GetSsDeformAnime(const SsKeyframe* key, SsDeformAttr& v)
{
	const int& svsize = key->value["vsize"].get<int>();
	const SsString& sVchg = key->value["vchg"].get<SsString>();

	v.verticeChgList.clear();

	std::vector<SsString>	str_list;
	split_string(sVchg, ' ', str_list);
	if (str_list.size() < 1)
	{
	}
	else
	{
		//移動していいない点は出力されていない
		int datasize = (int)atoi(str_list[0].c_str());		//データ数
		int cnt = 0;
		for (int i = 0; i < svsize; i++)
		{
			SsVector2 param(0, 0);
			if (cnt < datasize)
			{
				int idx = (int)atoi(str_list[1 + (cnt * 3)].c_str());		//index
				float x = (float)atof(str_list[2 + (cnt * 3)].c_str());	//x
				float y = (float)atof(str_list[3 + (cnt * 3)].c_str());	//y


				if (i == idx)
				{
					param.x = x;
					param.y = y;
					cnt++;
				}
			}
			v.verticeChgList.push_back(param);
		}
		if (cnt != datasize)
		{
			//データがおかしいのでは？
			//DEBUG_PRINTF("Deform Attr Data Error");
		}
	}
}


}	// namespace spritestudio6
