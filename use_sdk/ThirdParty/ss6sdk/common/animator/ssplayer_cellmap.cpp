#include <Siv3D.hpp>

#include <stdio.h>
#include <cstdlib>

#include "../Loader/ssloader.h"

#include "ssplayer_animedecode.h"
#include "ssplayer_matrix.h"
#include "ssplayer_render.h"
#include "../Loader/sscharconverter.h"


#include <memory>
#include <utility>


namespace spritestudio6
{

bool SsCellMapList::preloadTexture(SsProject* proj)
{

	for (auto i = proj->textureList.begin(); i != proj->textureList.end(); i++)
	{
		FilePath basePath = Unicode::FromUTF8(proj->getImageBasepath());
		String fname = FileSystem::PathAppend(basePath, Unicode::FromUTF8(*i));
		TextureAsset::Register(FileSystem::FileName(fname), fname);
		TextureAsset::Load(FileSystem::FileName(fname));
	}

	return true;
}

bool SsCellMapList::unloadTexture(SsProject* proj)
{
	for (auto i = proj->textureList.begin(); i != proj->textureList.end(); i++)
	{
		String fname = Unicode::FromUTF8(*i);
		TextureAsset::Unregister(FileSystem::FileName(fname));
	}

	return true;
}


SsCelMapLinker::SsCelMapLinker(SsCellMap* cellmap, SsString filePath)
{

	cellMap = cellmap;
	size_t num = cellMap->cells.size();
	for (size_t i = 0; i < num; i++)
	{
		CellDic[cellMap->cells[i]->name] = cellMap->cells[i];
	}

	FilePath fullpath = FileSystem::FullPath(Unicode::FromUTF8(filePath));
	fullpath = FileSystem::PathAppend(fullpath, Unicode::FromUTF8(cellmap->imagePath));

//	DEBUG_PRINTF("TextureFile Load %s \n", fullpath.c_str());
//	tex = SSTextureFactory::loadTexture(fullpath.c_str());
	Logger << U"TextureFile Load {}"_fmt(fullpath);
	tex = TextureAsset(FileSystem::FileName(fullpath));

}



void	SsCellMapList::clear()
{
	if (CellMapDic.size() > 0)
	{
		for (CellMapDicItr itr = CellMapDic.begin(); itr != CellMapDic.end();)
		{
			if (itr->second)
			{
				itr->second.reset();
				continue;
			}
			itr++;
		}
	}

	CellMapDic.clear();
	CellMapList.clear();
}


void	SsCellMapList::setCellMapPath(  const SsString& filepath )
{
	CellMapPath = filepath;
}

void	SsCellMapList::set(SsProject* proj , SsAnimePack* animepack )
{
	clear();
	setCellMapPath( proj->getImageBasepath() );

	for ( size_t i = 0 ; i < animepack->cellmapNames.size() ; i++ )
	{
		SsCellMap* cell = proj->findCellMap( animepack->cellmapNames[i] );
		if ( cell==0 )
		{
			Logger << U" Not found cellmap = {}"_fmt(Unicode::FromUTF8(animepack->cellmapNames[i]));
		}else{
			addIndex( cell );
		}
	}

	for ( size_t i = 0 ; i < proj->cellmapNames.size() ; i++ )
	{
		SsCellMap* cell = proj->findCellMap( proj->cellmapNames[i] );
		if ( cell==0 )
		{
			Logger << U" Not found cellmap = {}"_fmt(Unicode::FromUTF8(animepack->cellmapNames[i]));
		}else{
			addMap( cell );
		}
	}


}
void	SsCellMapList::addMap(SsCellMap* cellmap)
{
	CellMapDic[ cellmap->name+".ssce" ].reset( new SsCelMapLinker(cellmap , this->CellMapPath ) );
}

void	SsCellMapList::addIndex(SsCellMap* cellmap)
{
	std::unique_ptr<SsCelMapLinker> cellmapLinker( new SsCelMapLinker(cellmap , this->CellMapPath ) );
	CellMapList.push_back( std::move( cellmapLinker ) );
}

SsCelMapLinker*	SsCellMapList::getCellMapLink( const SsString& name )
{
	CellMapDicItr itr = CellMapDic.find(name);
	if ( itr != CellMapDic.end() )
	{
		return itr->second.get();
	}else{
		for ( itr=CellMapDic.begin() ; itr != CellMapDic.end() ; itr++)
		{
			if ( (itr->second.get())->cellMap->loadFilepath == name )
			{
				return itr->second.get();
			}
		}
	}

	return 0;
}


void getCellValue( SsCelMapLinker* l, SsString& cellName , SsCellValue& v )
{
	v.cell = l->findCell( cellName );

	v.filterMode = l->cellMap->filterMode;
	v.wrapMode = l->cellMap->wrapMode;

	if ( l->tex )
	{
		v.texture = l->tex;
	}
	else
	{
		v.texture.release();
	}

	calcUvs( &v );
}

void getCellValue( SsCellMapList* cellList, SsString& cellMapName , SsString& cellName , SsCellValue& v )
{
	SsCelMapLinker* l = cellList->getCellMapLink( cellMapName );
	getCellValue( l , cellName , v );


}

void getCellValue( SsCellMapList* cellList, int cellMapid , SsString& cellName , SsCellValue& v )
{
	SsCelMapLinker* l = cellList->getCellMapLink( cellMapid );
	if (l)
	{
		getCellValue(l, cellName, v);
	}


}

void calcUvs( SsCellValue* cellv )
{
	//SsCellMap* map = cellv->cellmapl->cellMap;
	SsCell* cell = cellv->cell;
	if ( cellv->texture.isEmpty() ) return ;

//	if ( cell == 0 || map == 0)
	if ( cell == 0 )
//	if ( ( cell == 0 ) || ( cellv->texture == 0 ) )	//koizumi change
	{
		cellv->uvs[0].x = cellv->uvs[0].y = 0;
		cellv->uvs[1].x = cellv->uvs[1].y = 0;
		cellv->uvs[2].x = cellv->uvs[2].y = 0;
		cellv->uvs[3].x = cellv->uvs[3].y = 0;
		return;
	}

	SsVector2 wh;
	wh.x = (float)cellv->texture.width();
	wh.y = (float)cellv->texture.height();

//	SsVector2 wh = map->pixelSize;
	// 右上に向かって＋になる
	float left = cell->pos.x / wh.x;
	float right = (cell->pos.x + cell->size.x) / wh.x;


	// LB->RB->LT->RT 順
	// 頂点をZ順にしている都合上UV値は上下逆転させている
	float top = cell->pos.y / wh.y;
	float bottom = ( cell->pos.y + cell->size.y) / wh.y;

	if (cell->rotated)
	{
		// 反時計回りに９０度回転されているため起こして描画されるようにしてやる。
		// 13
		// 02
		cellv->uvs[0].x = cellv->uvs[1].x = left;
		cellv->uvs[2].x = cellv->uvs[3].x = right;
		cellv->uvs[1].y = cellv->uvs[3].y = top;
		cellv->uvs[0].y = cellv->uvs[2].y = bottom;
	}
	else
	{
		// そのまま。頂点の順番は下記の通り
		// 01
		// 23
		cellv->uvs[0].x = cellv->uvs[2].x = left;
		cellv->uvs[1].x = cellv->uvs[3].x = right;
		cellv->uvs[0].y = cellv->uvs[1].y = top;
		cellv->uvs[2].y = cellv->uvs[3].y = bottom;
	}
}

}	// namespace spritestudio6

