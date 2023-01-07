#include "ssloader_sspj.h"
#include "ssloader_ssae.h"
#include "ssloader_ssce.h"
#include "ssloader_ssee.h"
#include "ssloader_ssqe.h"

#include "ssstring_uty.h"
//#include "../Helper/DebugPrint.h"
#include "sscharconverter.h"

namespace spritestudio6
{


SsString	SsProject::getSsceBasepath(){ 
	return getFullPath( m_proj_filepath , settings.cellMapBaseDirectory );
}


SsString	SsProject::getSsaeBasepath()
{ 
	return getFullPath( m_proj_filepath , settings.animeBaseDirectory );
}


SsString	SsProject::getSseeBasepath()
{ 
	return getFullPath( m_proj_filepath , settings.effectBaseDirectory);
}


SsString	SsProject::getSsqeBasepath()
{ 
	return getFullPath( m_proj_filepath , settings.animeBaseDirectory );
}




SsString	SsProject::getImageBasepath()
{
	return getFullPath( m_proj_filepath , settings.imageBaseDirectory );
}

SsProject::~SsProject()
{
	for ( SsAnimePackListItr itr = animeList.begin() ;
		itr != animeList.end() ; itr ++ ) itr->reset();
	animeList.clear();

	for ( SsSsCellMapList::iterator itr = cellmapList.begin() ;
		itr != cellmapList.end() ; itr ++ ) itr->reset();
	cellmapList.clear();

	for ( SsEffectFileList::iterator itr = effectfileList.begin() ;
		itr != effectfileList.end() ; itr ++ ) itr->reset();
	effectfileList.clear();

	for ( SsSequencePackList::iterator itr = sequenceList.begin() ;
		itr != sequenceList.end() ; itr ++ ) itr->reset();
	sequenceList.clear();

	cellmapNames.clear();
	animepackNames.clear();
	effectFileNames.clear();
	textureList.clear();
	sequencepackNames.clear();
}


SsAnimePack*		SsProject::findAnimationPack( SsString& animePackName )
{
	SsAnimePack* anime;
	for ( SsAnimePackListItr itr = animeList.begin()
		; itr != animeList.end() ; ++itr )
	{
		anime = itr->get();
		if ( anime->name == animePackName )
		{
			return anime;
		}
	}

	return 0;
}



SsAnimation*	SsProject::findAnimation( SsString& animePackName , SsString& AnimeName )
{
	SsAnimePack* p = findAnimationPack( animePackName );
	if ( p )
	{
		return p->findAnimation(AnimeName);
	}

	return 0;
}

SsEffectFile*		SsProject::findEffect( SsString& effectName )
{
	SsEffectFile* effect;
	for ( SsEffectFileListItr itr = effectfileList.begin()
		; itr != effectfileList.end() ; ++itr )
	{
		effect = itr->get();
		if ( effect->name == effectName )
		{
			return effect;
		}
	}

	return 0;
}

SsSequencePack*		SsProject::findSequencePack( SsString& sequencePackName )
{
	SsSequencePack* sequence;
	for ( SsSequencePackListItr itr = sequenceList.begin()
		; itr != sequenceList.end() ; ++itr )
	{
		sequence = itr->get();
		if ( sequence->name == sequencePackName )
		{
			return sequence;
		}
	}

	return 0;
}



SsSequence*		SsProject::findSequence( SsString& sequencePackName , SsString& SequenceName )
{
	SsSequencePack* p = findSequencePack( sequencePackName );
	if ( p )
	{
		return p->findSequence(SequenceName);
	}

	return 0;
}


SsProject*	ssloader_sspj::Load(const std::string& filename )
{
	libXML::XMLDocument xml;
	std::string filenameSSPJ = SsCharConverter::convert_path_string( filename );

	if ( libXML::XML_SUCCESS == xml.LoadFile( filenameSSPJ.c_str() ) )
	{
		SsXmlIArchiver ar( xml.GetDocument() , "SpriteStudioProject" );

		SsProject* proj = new SsProject();
		proj->__Serialize( &ar );
//		std::string project_filepath = SsCharConverter::convert_path_string( path2dir( filename ) );
		std::string project_filepath = path2dir( filename );
		proj->setFilepath( project_filepath );

		if ( checkFileVersion(proj->version, SPRITESTUDIO6_SSPJVERSION) == false )
		{
			//DEBUG_PRINTF("Project load error : %s", project_filepath.c_str());
			//DEBUG_PRINTF("sspj old version");
			delete proj;
			return 0;
		}

		//アニメーションリストを元に読み込みます。
		for ( size_t i = 0 ;i < proj->getAnimePackNum() ; i++ )
		{
			SsString ssaepath = SsCharConverter::convert_path_string(proj->getAnimePackFilePath(i));
			SsAnimePack* anime = ssloader_ssae::Load( ssaepath );
			if ( ( anime ) && ( checkFileVersion(anime->version, SPRITESTUDIO6_SSAEVERSION) == true ) ) 
			{
				proj->animeList.push_back( std::move( std::unique_ptr<SsAnimePack>( anime ) ) );
			}else{
				//エラー
				//DEBUG_PRINTF( "Animation load error : %s" , ssaepath.c_str() );
				//DEBUG_PRINTF( "ssae old version" );

				if ( anime ) delete anime;
				delete proj;
				return 0;
			}
		}

		std::map<std::string, std::string> textures;

		//セルマップリストを元に読み込みます。
		for ( size_t i = 0 ;i < proj->getCellMapNum() ; i++ )
		{
			SsString sscepath = SsCharConverter::convert_path_string(proj->getCellMapFilePath(i));
			SsCellMap* cell = ssloader_ssce::Load( sscepath );
			cell->loadFilepath = proj->getCelMapFileOriginalPath(i);
			proj->cellmapList.push_back( std::move( std::unique_ptr<SsCellMap>( cell ) ) );

			//セルマップリストからテクスチャを取得
			textures[cell->imagePath] = sscepath;
/*
			if ( ( cell ) && (checkFileVersion(cell->version, SPRITESTUDIO6_SSCEVERSION) == true) )
			{
				cell->loadFilepath = proj->getCelMapFileOriginalPath(i);
				proj->cellmapList.push_back( cell );
			}else{
				//エラー
				DEBUG_PRINTF( "Cellmap load error : %s" , sscepath.c_str() );
				DEBUG_PRINTF( "ssce old version" );
				delete proj;
				return 0;
			}
*/
		}

		//テクスチャリストを保存
		proj->textureList.clear();
		for (auto i = textures.begin(); i != textures.end(); i++)
		{
			proj->textureList.push_back(i->first);
		}

		//エフェクトリストを元に読み込みます。
		for ( size_t i = 0 ;i < proj->getEffectFileNum() ; i++ )
		{
			SsString sscepath = SsCharConverter::convert_path_string(proj->getEffectFilePath(i));
			SsEffectFile* efile = ssloader_ssee::Load( sscepath );
			ssloader_ssee::loadPostProcessing( efile, proj );
			proj->effectfileList.push_back( std::move( std::unique_ptr<SsEffectFile>( efile ) ) );
/*
			if ( ( efile ) && ( checkFileVersion(efile->version, SPRITESTUDIO6_SSCEVERSION) == true ) )
			{
				//efile->loadFilepath = proj->getCelMapFileOriginalPath(i);
				proj->effectfileList.push_back( efile );
				ssloader_ssee::loadPostProcessing( efile , proj );
			}else{
				//エラー
				DEBUG_PRINTF( "effect load error : %s" , sscepath.c_str() );
				DEBUG_PRINTF( "ssee old version" );
				delete proj;
				return 0;
			}
*/

		}

		//シーケンスリストを元に読み込みます。
		for ( size_t i = 0 ;i < proj->getSequencePackNum() ; i++ )
		{
			SsString ssqepath = SsCharConverter::convert_path_string(proj->getSequencePackFilePath(i));
			SsSequencePack* sequence = ssloader_ssqe::Load( ssqepath );
			if ( ( sequence ) && ( checkFileVersion( sequence->version, SPRITESTUDIO6_SSQEVERSION) == true ) ) 
			{
				proj->sequenceList.push_back( std::move( std::unique_ptr<SsSequencePack>( sequence ) ) );
			}else{
				//エラー
				//DEBUG_PRINTF( "Sequence load error : %s" , ssqepath.c_str() );
				//DEBUG_PRINTF( "ssqe old version" );

				if ( sequence ) delete sequence;
				delete proj;
				return 0;
			}
		}

		return proj;
	}	

	return 0;
}

SsCellMap* SsProject::findCellMap( SsString& str )
{
	for ( SsSsCellMapListItr itr = cellmapList.begin() ; 
		itr != cellmapList.end() ; itr ++) 
	{
//		SsString _name = (*itr)->name;
		//_name+=".ssce";
		//sspjの参照名とXMLが無いnameタグが一致していないケースがあったのでファイル名で取得
///		SsString _name = (*itr)->fname;
		SsCellMap* cellmap = itr->get();
		SsString _name = cellmap->loadFilepath;
		if ( _name == str )
		{
			return cellmap;
		}
	}
	return 0;
}


}	// namespace spritestudio6
