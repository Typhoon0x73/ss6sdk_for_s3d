#include "ssloader_sspj.h"
#include "ssloader_ssae.h"
#include "ssloader_ssce.h"
#include "ssloader_ssee.h"
#include "ssstring_uty.h"

//#include "../Helper/DebugPrint.h"

namespace spritestudio6
{


SsEffectFile*	ssloader_ssee::Load(const std::string& filename )
{

	SsString _basepath = "";

	
	SsEffectFile* effectFile = new SsEffectFile();

	libXML::XMLDocument xml;
	if ( libXML::XML_SUCCESS == xml.LoadFile( filename.c_str() ) )
	{
		SsXmlIArchiver ar( xml.GetDocument() , "SpriteStudioEffect" );
		effectFile->__Serialize( &ar );
	}else{
		delete effectFile;
		effectFile = 0;
	}

	return effectFile;

}


void	ssloader_ssee::loadPostProcessing( SsEffectFile* file , SsProject* pj )
{

	for ( size_t i = 0 ; i < file->effectData.nodeList.size(); i++)
	{

		SsEffectBehavior* beh = &file->effectData.nodeList[i]->behavior;
		beh->refCell = 0;
		if ( beh->CellMapName == "" )continue;
		SsCellMap* map =pj->findCellMap( beh->CellMapName );

		if ( map )
		{
			for ( size_t n = 0 ; n < map->cells.size() ; n++ )
			{
				if ( map->cells[n]->name == beh->CellName )
				{
					beh->refCell = map->cells[n];
				}
			}
			if ( beh->refCell == 0 )
			{
				//DEBUG_PRINTF("not fount cell : %s , %s", beh->CellMapName.c_str(), beh->CellName.c_str());
			}
		}
	}



}

}	// namespace spritestudio6
