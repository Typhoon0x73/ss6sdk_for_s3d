#ifndef __SSLOADER_SSPJ__
#define __SSLOADER_SSPJ__

#pragma warning(disable : 4819) //

#include "sstypes.h"
#include "ssarchiver.h"
#include "ssstring_uty.h"

#include "ssloader_ssae.h"
#include "ssloader_ssce.h"
#include "ssloader_ssee.h"
#include "ssloader_ssqe.h"

#include <memory>
#include <utility>

#define SPRITESTUDIO6_SSPJVERSION "2.00.00"

namespace spritestudio6
{

/// プロジェクトファイルの設定が記載されているデータです。
/// 以下のタグはエディタ編集用のデータなので読み飛ばします。
//	編集時設定のためよまない
//	<ssaxImport>						//!< .ssax インポート設定
//	<copyWhenImportImageIsOutside>	//!< プロジェクトフォルダ外に画像ファイルがある場合にインポート先にコピーする。
//	<exportAnimeFileFormat>			//!< エクスポート時のアニメーションファイルのフォーマット 
//	<exportCellMapFileFormat>			//!< エクスポート時のセルマップファイルのフォーマット
//	<exportCellMap>					//!< セルマップをエクスポートする
//	<copyImageWhenExportCellmap>		//!< セルマップのエクスポート時に画像ファイルをエクスポート先にコピーする
//	<ssaxExport>						//!< .ssax エクスポート設定
//	<player>					//!< 再生対象のプレイヤー。これにより使える機能に制限をかける。
//	<strictVer4>				//!< Ver4互換
//	<availableAttributes>		//!< 使用するアトリビュート
//	<defaultSetAttributes>	//!< 新規キー作成でデフォルトでキーが打たれるアトリビュート
class	SsProjectSetting
{
public:
	//他のメンバーもあるが、必要最低限のものを読み込む

	//アニメーションファイルの相対指定等
	SsString					animeBaseDirectory;			//!< アニメーションデータの読み書き基準ディレクトリ。
	SsString					cellMapBaseDirectory;		//!< セルマップデータの読み書き基準ディレクトリ。
	SsString					imageBaseDirectory;			//!< 画像データの読み込み元先基準ディレクトリ。
	SsString					effectBaseDirectory;


	SsString					exportBaseDirectory;		//!< エクスポート先の基準ディレクトリ。
	bool						queryExportBaseDirectory;	//!< エクスポート先の基準ディレクトリ指定をさせるか？ 
	SsTexWrapMode::_enum		wrapMode;					//!< テクスチャのラップモード
	SsTexFilterMode::_enum		filterMode;					//!< テクスチャのフィルタモード

	int							vertexAnimeFloat;			//!< 頂点変形アトリビュートの少数対応

	SsProjectSetting() 
	{
		vertexAnimeFloat = false;							//!< SpriteStudio6.1以前のデータの場合は存在しないのでOFFを初期値とする
	}
	
	///シリアライズのための宣言です。
	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		SPRITESTUDIO6SDK_SSAR_DECLARE(animeBaseDirectory);
		SPRITESTUDIO6SDK_SSAR_DECLARE(cellMapBaseDirectory);
		SPRITESTUDIO6SDK_SSAR_DECLARE(imageBaseDirectory);
		SPRITESTUDIO6SDK_SSAR_DECLARE(effectBaseDirectory);

		SPRITESTUDIO6SDK_SSAR_DECLARE(exportBaseDirectory);
		SPRITESTUDIO6SDK_SSAR_DECLARE(queryExportBaseDirectory);
		SPRITESTUDIO6SDK_SSAR_DECLARE_ENUM( wrapMode );
		SPRITESTUDIO6SDK_SSAR_DECLARE_ENUM( filterMode );
		SPRITESTUDIO6SDK_SSAR_DECLARE_ENUM(vertexAnimeFloat);

	}
};


typedef std::vector<std::unique_ptr<SsAnimePack>> SsAnimePackList;
typedef SsAnimePackList::iterator SsAnimePackListItr;

typedef std::vector<std::unique_ptr<SsCellMap>> SsSsCellMapList;
typedef SsSsCellMapList::iterator SsSsCellMapListItr;


typedef std::vector<std::unique_ptr<SsEffectFile>> SsEffectFileList;
typedef SsEffectFileList::iterator SsEffectFileListItr;

typedef std::vector<std::unique_ptr<SsSequencePack>> SsSequencePackList;
typedef SsSequencePackList::iterator SsSequencePackListItr;

/// XMLドキュメントとなっているsspjファイルのデータ保持を提供するクラスです。
///以下はエディタ情報のため読み飛ばします。
/// animeSettings   デフォルトのアニメーション設定 
/// texPackSettings デフォルトのテクスチャパッキング設定
class SsProject
{
public:

	SsString				version;
	SsProjectSetting		settings;			//!< プロジェクト設定

	std::vector<SsString>	cellmapNames;		//!< セルマップファイルのリスト
	std::vector<SsString>	animepackNames;		//!< アニメファイルのリスト
	std::vector<SsString>	effectFileNames;	//!< エフェクトファイルのリスト
	std::vector<SsString>	textureList;		//!< セルマップから取得したテクスチャのリスト
	std::vector<SsString>	sequencepackNames;	//!< シーケンスファイルのリスト

	SsAnimePackList		animeList;		//!< アニメーションのリスト	
	SsSsCellMapList		cellmapList;	//!< セルマップリスト
	SsEffectFileList	effectfileList;	//!< エフェクトのリスト
	SsSequencePackList	sequenceList;	//!< シーケンスのリスト	

	//ロード時に作成されるワーク
	SsString	m_proj_filepath;	///プロジェクトファイルのパス


	SsProject(){}
	virtual ~SsProject();

	///プロジェクトの設定情報の取得
	const SsProjectSetting&	getProjectSetting(){ return settings;} 

	///アニメパックの数量を取得する
	const size_t getAnimePackNum(){ return animepackNames.size(); }

	///セルマップデータの数量を取得
	const size_t getCellMapNum(){ return cellmapNames.size(); }

	///セルマップデータの数量を取得
	const size_t getEffectFileNum(){ return effectFileNames.size(); }

	///シーケンスパックの数量を取得する
	const size_t getSequencePackNum(){ return sequencepackNames.size(); }


	///アニメパックデータのコンテナを取得する
	SsAnimePackList&	getAnimePackList(){ return animeList;}

	///セルマップデータのコンテナを取得する
	SsSsCellMapList&	getCellMapList(){ return cellmapList;}

	//エフェクトファイルのリスト
	SsEffectFileList&	getEffectFileList(){ return effectfileList;}

	///シーケンスパックデータのコンテナを取得する
	SsSequencePackList&	getSequencePackList(){ return sequenceList;}

	///アニメパックデータの各情報を取得する
	SsAnimePack*	getAnimePack(int index){ return animeList[index].get();}

	///セルマップデータの各情報を取得する
	SsCellMap*	getCellMap(int index){ return cellmapList[index].get();}

	//エフェクトファイルの各情報を取得する
	SsEffectFile*	getEffectFile(int index){ return effectfileList[index].get();}

	///シーケンスパックデータの各情報を取得する
	SsSequencePack*	getSequencePack(int index){ return sequenceList[index].get();}


	//アニメパック名とアニメ名からアニメーションを取得する
	SsAnimation*		findAnimation( SsString& animePackName , SsString& AnimeName );

	SsAnimePack*		findAnimationPack( SsString& animePackName );

	//エフェクトの名前からエフェクトを取得する
	SsEffectFile*		findEffect( SsString& effectName );


	//シーケンスパック名とシーケンス名からシーケンスを取得する
	SsSequence*			findSequence( SsString& sequencePackName , SsString& SequenceName );

	SsSequencePack*		findSequencePack( SsString& sequencePackName );

	

	SsCellMap* findCellMap( SsString& str );


	///シリアライズのための宣言です。
	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		SPRITESTUDIO6SDK_SSAR_DECLARE_ATTRIBUTE(version);
		SPRITESTUDIO6SDK_SSAR_STRUCT_DECLARE( settings );
		SPRITESTUDIO6SDK_SSAR_DECLARE( cellmapNames );
		SPRITESTUDIO6SDK_SSAR_DECLARE( animepackNames );
		SPRITESTUDIO6SDK_SSAR_DECLARE( effectFileNames );
		SPRITESTUDIO6SDK_SSAR_DECLARE( sequencepackNames );

	}

public:

	///自身のファイルパスを設定する
	void	setFilepath( const SsString& path ) { m_proj_filepath = path; }

	///自身の読み込み元のファイルパスを取得する
	const	SsString&	getFilepath(){ return m_proj_filepath; }

	///ssceデータの読み込み元の基準パスを取得する。
	SsString	getSsceBasepath();

	///ssaeデータの読み込み元の基準パスを取得する。
	SsString	getSsaeBasepath();

	SsString	getSseeBasepath();

	SsString	getImageBasepath();

	///ssqeデータの読み込み元の基準パスを取得する。
	SsString	getSsqeBasepath();


	///AnimePack(ssae)のファイル名をパス付きで取得する
	SsString	getAnimePackFilePath( size_t index ) { 
		if ( animepackNames.size() <= index ) return "";
		return getSsaeBasepath() + animepackNames[index];
	}


	///CellMap(ssce)のファイル名をパス付きで取得する
	SsString	getCellMapFilePath( size_t index ) { 
		if ( cellmapNames.size() <= index ) return "";
        SsString str = getSsceBasepath();


		std::string f = cellmapNames[index];
		
		std::string ret = getFullPath( str , path2dir( cellmapNames[index] ) );
		ret = ret + path2file( cellmapNames[index] );
		ret = nomarizeFilename(ret);

        //str = str + cellmapNames[index];
		//return str ;
		return ret;
	}

	SsString	getEffectFilePath( size_t index ) { 
		if ( effectFileNames.size() <= index ) return "";
		return getSseeBasepath() + effectFileNames[index];
	}

	SsString	getCelMapFileOriginalPath( size_t index ) { 
		if ( cellmapNames.size() <= index ) return "";
		return cellmapNames[index] ;
	}

	///SequencePack(ssqe)のファイル名をパス付きで取得する
	SsString	getSequencePackFilePath( size_t index ) { 
		if ( sequencepackNames.size() <= index ) return "";
		return getSsqeBasepath() + sequencepackNames[index];
	}

};

///sspjのローダークラスです。
class ssloader_sspj
{
private:

public:
	ssloader_sspj(){}
	virtual ~ssloader_sspj(){}
	static SsProject*	Load(const std::string& filename );

};


}	// namespace spritestudio6

#endif
