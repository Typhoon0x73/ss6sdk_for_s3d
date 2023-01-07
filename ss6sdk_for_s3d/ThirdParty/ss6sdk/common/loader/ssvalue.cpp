#include "ssarchiver.h"
#include "ssstring_uty.h"
#include "ssvalue.h"
#include "sscharconverter.h"


namespace spritestudio6
{

//SsValue用のシリアライザ
void	SsValueSeriarizer( ISsXmlArchiver* ar , SsValue& v , const std::string key) 
{

	//インプット
	libXML::XMLElement* e = ar->getxml();
	if ( key != "" )
	{
		e = e->FirstChildElement( key.c_str() );
	}
	if ( e )
	{
		libXML::XMLElement* child = e->FirstChildElement();

		if ( child == 0 )
		{
			const char* str = e->GetText();

			if (str==0)
			{
				const char* err_log1 = e->Parent()->Value();
				SPRITESTUDIO6SDK_NOUSE_ARGUMENT(err_log1);

				if ( e->Parent()->ToElement() )
				{
					const char* err_log2 = e->Parent()->ToElement()->GetText();
					SPRITESTUDIO6SDK_NOUSE_ARGUMENT(err_log2);

					if ( e->Parent()->Parent()->ToElement()->FirstAttribute() )
					{
						const char* err_log3 = e->Parent()->Parent()->ToElement()->FirstAttribute()->Value();
						SPRITESTUDIO6SDK_NOUSE_ARGUMENT(err_log3);
					}
				}
				return ;
			}
			
			std::string enc_str = str;
			v = SsValueSeriarizer__MakeValue( enc_str.c_str() );
		}else{
			libXML::XMLElement* ce = child;

			SsHash hash;
			while(ce)
			{
				const char* str = ce->GetText();
				if ( str != 0 )
				{
					std::string enc_str = str;
					hash[ce->Name()] = SsValueSeriarizer__MakeValue( enc_str.c_str() );
					ce = (libXML::XMLElement*)ce->NextSibling();
				}else{
					//さらに子構造があるようだ
					//さらに子構造があるようだ
					//ce = 0 ;
					SsXmlIArchiver ar2(ce);

					SsValue tempv;
					SsValueSeriarizer( &ar2 , tempv , "");
					hash[ce->Name()] = tempv;					
					ce = (libXML::XMLElement*)ce->NextSibling();
				}
			}

			v = SsValue(hash);

		}

		return ;
	}
	//assert(e);

}

}	// namespace spritestudio6
