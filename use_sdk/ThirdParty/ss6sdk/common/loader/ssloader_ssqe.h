#ifndef __SSLOADER_SSQE__
#define __SSLOADER_SSQE__

#include "sstypes.h"
#include "ssarchiver.h"

#define SPRITESTUDIO6_SSQEVERSION "1.00.00"

namespace spritestudio6
{

class SsSequence;





class SsSequenceItem
{
public:
	SsString				refAnimePack;	/// 参照アニメパック名
	SsString				refAnime;		/// 参照アニメ名
	int						repeatCount;	/// 繰り返し再生回数

public:
	SsSequenceItem(){}
	virtual ~SsSequenceItem()
	{
	}

	///シリアライズのための宣言です。
	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		SPRITESTUDIO6SDK_SSAR_DECLARE( refAnimePack );
		SPRITESTUDIO6SDK_SSAR_DECLARE( refAnime );
		SPRITESTUDIO6SDK_SSAR_DECLARE( repeatCount );
	}
};

class SsSequence
{
public:
	SsString	name;							/// シーケンスの名称
	int								index;		/// このシーケンスがもつ固有の番号
	SsSequenceType::_enum			type;		/// このシーケンスのタイプ
	std::vector<SsSequenceItem*>	list;		/// このシーケンスがもつアイテムのリスト

public:
	SsSequence(){}
	virtual ~SsSequence()
	{
		for ( std::vector<SsSequenceItem*>::iterator itr = list.begin() ; 
			itr != list.end() ; itr ++ ) delete (*itr);		
	}

	///シリアライズのための宣言です。
	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		SPRITESTUDIO6SDK_SSAR_DECLARE( name );
		SPRITESTUDIO6SDK_SSAR_DECLARE( index );
		SPRITESTUDIO6SDK_SSAR_DECLARE_ENUM( type );
		SPRITESTUDIO6SDK_SSAR_DECLARE_LISTEX( list , "value" );
	}
};

/**
*@class SsSequencePack
*@brief アニメーションを組み合わせた構造とその構造を使用するシーケンスを格納するデータです。
アニメーションの組み合わせ構造をSsSequenceで定義しています。
*/
class SsSequencePack
{
public:
	SsString					version;
	SsString					name;			//!< シーケンスパック名称
	std::vector<SsSequence*>	sequenceList;	//!< 格納されている子シーケンスのリスト

	SsSequencePack(){}
	virtual ~SsSequencePack()
	{
		for ( std::vector<SsSequence*>::iterator itr = sequenceList.begin() ; 
			itr != sequenceList.end() ; itr ++ ) delete (*itr);		
	}

	///シリアライズのための宣言です。
	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		SPRITESTUDIO6SDK_SSAR_DECLARE_ATTRIBUTE(version);
		SPRITESTUDIO6SDK_SSAR_DECLARE( name );
		SPRITESTUDIO6SDK_SSAR_DECLARE_LISTEX( sequenceList , "sequence" );


	}

	//シーケンス名からシーケンスを取得する
	SsSequence*	findSequence(SsString& name);

};

/*
* @class ssloader_ssqe
* @brief ssqeファイルをロードするためのクラスです。
*/
class	ssloader_ssqe
{
public:
	ssloader_ssqe(){}
	virtual ~ssloader_ssqe(){}

	///ssqeファイル名を指定しロードが成功したらそのSsSequencePackのポインタを返します。
	static SsSequencePack*	Load(const std::string& filename );

};





}	// namespace spritestudio6

#endif
