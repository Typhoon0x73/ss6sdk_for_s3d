#ifndef __SSPLAYER_CELLMAP__
#define __SSPLAYER_CELLMAP__

#include <Siv3D.hpp>
#include "../Loader/sstypes.h"

#include <memory>

namespace spritestudio6
{

class SsAnimeDecoder;
class SsCelMapLinker;

///パーツが使用するセルの情報
struct SsCellValue
{
	SsCell*						cell;		///参照しているセル
	Texture						texture;	///テクスチャ
	SsVector2					uvs[5];		///使用するUV座標
	SsTexWrapMode::_enum		wrapMode;	///< テクスチャのラップモード
	SsTexFilterMode::_enum		filterMode;	///< テクスチャのフィルタモード

	SsCellValue() :  
		cell(0) ,  
		texture()
		{}
};

class SsCelMapLinker
{
public:
	SsCellMap*			cellMap;
	Texture				tex;

	std::map<SsString,SsCell*>	CellDic;

public:
	SsCelMapLinker()
		: cellMap(0) , tex(0)
	{}

	SsCelMapLinker(SsCellMap* cellmap, SsString filePath);

	virtual ~SsCelMapLinker()
	{
		CellDic.clear();
	}

	SsCell*	findCell( const SsString& name ){ return CellDic[name]; }
	
};

//プロジェクト全体で保持しているセルマップ
//現在はprojectのセルマップの列挙とssaeの列挙は同一
class	SsCellMapList
{
private:
	//同名セルマップは上書き
	std::map<SsString, std::unique_ptr<SsCelMapLinker>>	CellMapDic;
	typedef std::map<SsString,std::unique_ptr<SsCelMapLinker>>::iterator CellMapDicItr;

	std::vector<std::unique_ptr<SsCelMapLinker>>	CellMapList;//添え字参照用

	SsString	CellMapPath;

private:
	void	addIndex(SsCellMap* cellmap);
	void	addMap(SsCellMap* cellmap);

public:
	SsCellMapList(){}
	virtual ~SsCellMapList()
	{
		for ( CellMapDicItr itr = CellMapDic.begin() ; itr != CellMapDic.end() ; itr ++)
		{
			itr->second.reset();
		}

		for ( size_t i = 0 ; i < CellMapList.size(); i++ )
		{
			CellMapList[i].reset();
		}
		CellMapList.clear();
		CellMapDic.clear();
	}

	void	clear();
	size_t	size(){ return CellMapList.size(); }

	void	setCellMapPath(  const SsString& filepath );

	//projectとanimepackからアニメーションの再生に必要なセルマップのリストを作成する
	//アニメパックのセルリストに登載されている順にセルマップを読み込みインデックス化する
	//SsProjectを介してセルを検索しているのはセルがそこにアレイで確保されているから
	//もし既に読み込み済みだったりする場合は、アニメパックのセルＩＤ順にセルマップを登録すればいい
	void	set(SsProject* proj , SsAnimePack* animepack );

	SsCelMapLinker*	getCellMapLink( const SsString& name );
	SsCelMapLinker*	getCellMapLink( int index )
	{	
		if (CellMapList.size() <= index) return 0;
		return CellMapList[index].get();
	}
	
	bool preloadTexture(SsProject* proj);
	bool unloadTexture(SsProject* proj = 0);

};



//void getCellValue( int cellMapid , SsString& cellName , SsCellValue& v );
void getCellValue( SsCellMapList* cellList, int cellMapid , SsString& cellName , SsCellValue& v );
void getCellValue( SsCellMapList* cellList, SsString& cellMapName , SsString& cellName , SsCellValue& v );

void calcUvs( SsCellValue* cellv );


}	// namespace spritestudio6

#endif
