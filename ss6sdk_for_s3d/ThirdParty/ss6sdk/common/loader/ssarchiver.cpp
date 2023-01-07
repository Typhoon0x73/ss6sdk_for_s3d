#include "ssarchiver.h"
#include "ssstring_uty.h"
#include "sscharconverter.h"

namespace spritestudio6
{

bool	SsXmlIArchiver::dc_attr( const char* name , SsString& member )
{
	SPRITESTUDIO6SDK_AR_SELF_CHECK();
	const char* v = 0;

	v = getxml()->Attribute( name  );
	if ( v != 0 ) member= v;


	return true;
}
bool	SsXmlIArchiver::dc_attr( const char* name , int& member )
{
	SPRITESTUDIO6SDK_AR_SELF_CHECK();

	const char* v = getxml()->Attribute( name  );

	member = atoi(v);

	return true;
}

bool	SsXmlIArchiver::dc( const char* name , int& member )
{

	SPRITESTUDIO6SDK_AR_SELF_CHECK();
	SsString str;
	dc( name , str );
	member = atoi( str.c_str() );
	if (str == "")
	{
		return false;
	}

	return true;
}

bool	SsXmlIArchiver::dc( const char* name , float& member )
{
	SPRITESTUDIO6SDK_AR_SELF_CHECK();
	SsString str;
	dc( name , str );
	member = (float)atof( str.c_str() );
	if (str == "")
	{
		return false;
	}

	return true;
}

bool	SsXmlIArchiver::dc( const char* name , SsString& member )
{
	SPRITESTUDIO6SDK_AR_SELF_CHECK();
	libXML::XMLElement* e = getxml()->FirstChildElement( name );
	if ( e )
	{
		if ( e->GetText() )
		{
			member = e->GetText();
		}else{
			member ="";
		}
		return true;
	}
	return false;
}

bool	SsXmlIArchiver::dc( const char* name , bool& member )
{
	SPRITESTUDIO6SDK_AR_SELF_CHECK();
	member = false;
	libXML::XMLElement* e = getxml()->FirstChildElement( name );
	if ( e )
	{
		int ret = GetTextToInt( e , 0 );
		if ( ret == 1)member = true;
		return true;
	}

	return false;
}


bool	SsXmlIArchiver::dc(const char* name, std::vector<SsPoint2>& list)
{
	SPRITESTUDIO6SDK_AR_SELF_CHECK();
	list.clear();
	libXML::XMLElement* e = getxml()->FirstChildElement(name);
	if (e == 0)return false;
	e = e->FirstChildElement("value");
	while (e)
	{
		const char* txt = e->GetText();
		SsVector2 vec;
		StringToPoint2(txt, vec);
		list.push_back(vec);
		e = e->NextSiblingElement();
	}

	return true;

}

bool	SsXmlIArchiver::dc(const char* name, std::vector<SsTriangle>& list)
{
	SPRITESTUDIO6SDK_AR_SELF_CHECK();
	list.clear();
	libXML::XMLElement* e = getxml()->FirstChildElement(name);
	if (e == 0)return false;
	e = e->FirstChildElement("value");
	while (e)
	{
		const char* txt = e->GetText();
		SsTriangle tri;
		StringToTriangle(txt, tri);
		list.push_back(tri);
		e = e->NextSiblingElement();
	}

	return true;

}

bool	SsXmlIArchiver::dc( const char* name , std::vector<SsString>& list )
{
	SPRITESTUDIO6SDK_AR_SELF_CHECK();
	list.clear();
	libXML::XMLElement* e = getxml()->FirstChildElement( name );
	if (e==0)return false;
	e = e->FirstChildElement( "value" );
	while( e )
	{
		const char* txt = e->GetText();
		std::string txtStr = std::string(txt);

		list.push_back( txtStr );
		e = e->NextSiblingElement();
	}

	return true;
}

bool	SsXmlIArchiver::dc( const char* name , SsPoint2& member )
{
	SPRITESTUDIO6SDK_AR_SELF_CHECK();

	libXML::XMLElement* e = getxml()->FirstChildElement( name );

	if ( e )
	{
		std::string str = e->GetText();
		return StringToPoint2( str , member );

	}
	return false;

}

bool	SsXmlIArchiver::dc( const char* name , SsCurve& member )
{
	SPRITESTUDIO6SDK_AR_SELF_CHECK();

	libXML::XMLElement* e = getxml()->FirstChildElement( name );

	if ( e )
	{
		std::vector<SsString>	str_list;
		split_string( e->GetText() , ' ' , str_list );
		if ( str_list.size() < 4 )
		{
			return false;
		}else{
			member.startTime = (float)atof( str_list[0].c_str() );
			member.startValue = (float)atof( str_list[1].c_str() );
			member.endTime = (float)atof( str_list[2].c_str() );
			member.endValue = (float)atof( str_list[3].c_str() );		

			return true;
		}
	}
	return false;
}

bool	SsXmlIArchiver::dc(const char* name, SsTriangle& member)
{
	SPRITESTUDIO6SDK_AR_SELF_CHECK();

	libXML::XMLElement* e = getxml()->FirstChildElement(name);

	if (e)
	{

		StringToTriangle(e->GetText(), member);
/*
		std::vector<SsString>	str_list;
		split_string(e->GetText(), ' ', str_list);
		if (str_list.size() < 4)
		{
			return false;
		}
		else {
			member.idxPo1 = (int)atoi(str_list[0].c_str());
			member.idxPo2 = (int)atoi(str_list[1].c_str());
			member.idxPo3 = (int)atoi(str_list[2].c_str());

			return true;
		}
*/
	}
	return false;
}
/*
bool	SsXmlIArchiver::dc(const char* name, SsBoneBind& member)
{
	SPRITESTUDIO6SDK_AR_SELF_CHECK();

	libXML::XMLElement* e = getxml()->FirstChildElement(name);

	if (e)
	{
		std::vector<SsString>	str_list;
		split_string(e->GetText(), ' ', str_list);
		if (str_list.size() < 4)
		{
			return false;
		}
		else 
		{
			member.boneIndex = (float)atof(str_list[0].c_str());
			member.blend = (float)atof(str_list[1].c_str());

			return true;
		}
	}
	return false;
}
*/


bool	SsXmlIArchiver::dc(const char* name, std::map<SsString, int>& _map)
{
	_map.clear();

	libXML::XMLElement* e = getxml()->FirstChildElement(name);
	if (e == 0)return false;
	e = e->FirstChildElement("item");

	while (e)
	{
		std::string key = e->Attribute("key");
		std::string val = e->GetText();

		_map[key] = atoi(val.c_str());

		e = e->NextSiblingElement();
	}

	return false;
}

bool	StringToTriangle(const std::string& str, SsTriangle& tri)
{

	std::vector<SsString>	str_list;
	split_string(str, ' ', str_list);
	if (str_list.size() < 3)
	{
		return false;
	}
	else {
		tri.idxPo1 = (int)atoi(str_list[0].c_str());
		tri.idxPo2 = (int)atoi(str_list[1].c_str());
		tri.idxPo3 = (int)atoi(str_list[2].c_str());

		return true;
	}
	return true;
}


bool	StringToPoint2( const std::string& str , SsPoint2& point )
{
	std::vector<SsString>	str_list;
	split_string( str , ' ' , str_list );
	if ( str_list.size() < 2 )
	{
		point.x = 0;
		point.y = 0;
		return false;
	}else{
		point.x = (float)atof( str_list[0].c_str() );
		point.y = (float)atof( str_list[1].c_str() );
	}

	return true;
}


bool	StringToIRect( const std::string& str , SsIRect& rect )
{
	std::vector<SsString>	str_list;
	split_string( str , ' ' , str_list );
	if ( str_list.size() < 4 )
	{
		rect.x = 0;
		rect.y = 0;
		rect.w = 0;
		rect.h = 0;
		return false;
	}else{
		rect.x = (int)atof( str_list[0].c_str() );
		rect.y = (int)atof( str_list[1].c_str() );
		rect.w = (int)atof( str_list[2].c_str() );
		rect.h = (int)atof( str_list[3].c_str() );
	}

	return true;
}

void	SsArchiverInit()
{
}

}	// namespace spritestudio6
