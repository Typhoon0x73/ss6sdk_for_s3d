#ifndef __SSPLAYER_ANIMEDECODE__
#define __SSPLAYER_ANIMEDECODE__

#include "../Loader/ssloader.h"


#include "ssplayer_types.h"
#include "ssplayer_cellmap.h"
#include "ssplayer_PartState.h"
#include "ssplayer_macro.h"
#include "ssplayer_mesh.h"

#include <memory>

//きれいな頂点変形に対応する場合は1にする。
//４ポリゴンで変形します。
//0の場合はZ型の２ポリゴンで変形します。
#define SPRITESTUDIO6SDK_USE_TRIANGLE_FIN (1)

namespace spritestudio6
{


	//パーツとアニメを関連付ける
	typedef std::pair<SsPart*,SsPartAnime*>	SsPartAndAnime;


	//パーツのソート順
	class SsPartStateLess
	{
	public:
		bool operator()(const SsPartState* lhs, const SsPartState* rhs) const
		{
			if (lhs->prio == rhs->prio)
				return lhs->index < rhs->index;
			return lhs->prio < rhs->prio;
		}
	};



	class SsAnimeDecoder
	{
	public:

	private:

		///プロジェクト情報
		SsProject* project;
		
		///パーツ情報とパーツアニメーションを結びつけアレイにしたもの
		std::vector<SsPartAndAnime>		partAnime;
		std::vector<SsPartAndAnime>		setupPartAnime;		///セットアップデータ

		///パーツ名からアニメ情報をとるために使うもし、そういった用途が無い場合はローカル変数でも機能する
		std::map<SsString,SsPartAnime*> partAnimeDic;
		std::map<SsString, SsPartAnime*>setupPartAnimeDic;	///セットアップデータ

		std::unique_ptr<SsCellMapList>	curCellMapManager;		///アニメに関連付けられているセルマップ

		std::unique_ptr<std::vector<SsPartState>>	partState;	///パーツの現在の状態が格納されています。

		std::list<SsPartState*>			sortList;			///ソート状態
		std::list<SsPartState*>			partStatesMask_;	///マスクテンポラリ
		std::vector<SsPartState*>		maskIndexList;


		int				seedOffset;							//エフェクトのシードへ影響
		float			nowPlatTime;
		float			nowPlatTimeOld;						//前のフレームで再生した時間
		float			frameDelta;
		int				curAnimeStartFrame;
		int				curAnimeEndFrame;
		int				curAnimeTotalFrame;
		int				curAnimeFPS;
		SsAnimation*	curAnimation;
		bool			instancePartsHide;
		bool			maskFuncFlag;						//マスク機能（初期化、描画）を有効にするか？インスタンスパーツ内のマスク対応
		bool			maskParentSetting;					//親のマスク対象
		SsSequence*		curSequence;

		size_t			stateNum;

		std::unique_ptr<SsMeshAnimator>	meshAnimator;
		SsModel*		myModel;

	private:
		void	updateState( int nowTime , SsPart* part , SsPartAnime* part_anime , SsPartState* state );
		void	updateInstance( int nowTime , SsPart* part , SsPartAnime* part_anime , SsPartState* state );
		void	updateEffect( float frameDelta , int nowTime , SsPart* part , SsPartAnime* part_anime , SsPartState* state );


		void	updateMatrix(SsPart* part , SsPartAnime* anime , SsPartState* state);

		void	updateVertices(SsPart* part , SsPartAnime* anime , SsPartState* state);

		int		CalcAnimeLabel2Frame(const SsString& str, int offset, SsAnimation* Animation  );
		int		findAnimetionLabel(const SsString& str, SsAnimation* Animation);
		bool	getFirstCell(SsPart* part, SsCellValue& out);


	public:
		SsAnimeDecoder();
		virtual ~SsAnimeDecoder()
		{
			//念のため解放（スマートポインタなので実用上問題ないはずだが明示性として）
			curCellMapManager.reset();
			partState.reset();
			meshAnimator.reset();
		}

		virtual void	update( float frameDelta = 1.0f );
		virtual void	draw();

		void	setAnimation( SsModel*	model , SsAnimation* anime , SsCellMapList* cellmap , SsProject* sspj=0 );
	//	void	setAnimation(SsModel*	model, SsAnimation* anime, SsAnimePack *animepack , SsCellMapList* cellmap, SsProject* sspj = 0);
		void	setSequence( SsSequence* sequence , SsProject* sspj=0 );

		void	setPlayFrame( float time ) { nowPlatTime = time; }
		int		getAnimeStartFrame() { return curAnimeStartFrame; }
		int		getAnimeEndFrame() { return curAnimeEndFrame; }
		int		getAnimeTotalFrame() { return curAnimeTotalFrame; }
		int		getAnimeFPS() {	return curAnimeFPS; }		

		int		getSequenceItemCount() { return (int)(curSequence->list.size()); }
		SsSequenceItem*	getSequenceItem( int iIndex ) { return curSequence->list[iIndex]; }

		size_t	getStateNum() { return stateNum; }
		std::vector<SsPartState>& getPartState() { return *(partState.get()); }
		SsModel*	getMyModel(){return myModel;}

		std::list<SsPartState*>&		getPartSortList(){return sortList;}
		std::vector<SsPartAndAnime>&	getPartAnime(){ return	partAnime; }
	
		template<typename mytype> int	SsGetKeyValue( SsPart* part, int time , SsAttribute* attr , mytype&  value );
		template<typename mytype> void	SsInterpolationValue( int time , const SsKeyframe* leftkey , const SsKeyframe* rightkey , mytype& v );
		void	SsInterpolationValue( int time , const SsKeyframe* leftkey , const SsKeyframe* rightkey , SsCellValue& v );
		void	SsInterpolationValue( int time , const SsKeyframe* leftkey , const SsKeyframe* rightkey , SsPartsColorAnime& v );
		void	SsInterpolationValue( int time , const SsKeyframe* leftkey , const SsKeyframe* rightkey , SsColorAnime& v );
		void	SsInterpolationValue( int time , const SsKeyframe* leftkey , const SsKeyframe* rightkey , SsShaderAnime& v );
		void	SsInterpolationValue( int time , const SsKeyframe* leftkey , const SsKeyframe* rightkey , SsVertexAnime& v );
		void	SsInterpolationValue( int time , const SsKeyframe* leftkey , const SsKeyframe* rightkey , SsSignalAttr& v );
		void	SsInterpolationValue( int time , const SsKeyframe* leftkey , const SsKeyframe* rightkey , SsInstanceAttr& v );
		void	SsInterpolationValue( int time , const SsKeyframe* leftkey , const SsKeyframe* rightkey , SsEffectAttr& v );
		void	SsInterpolationValue( int time , const SsKeyframe* leftkey , const SsKeyframe* rightkey , SsDeformAttr& v );


		void	setInstancePartsHide(bool hide){
			instancePartsHide = hide;
		}
		void	restart();
		void	reset();

		void	setSeedOffset(int a ){ seedOffset = a; }
		int		getSeedOffset(){ return seedOffset; }

		void setMaskFuncFlag(bool flg);									//マスク用ステンシルバッファの初期化を行うか
		void setMaskParentSetting(bool flg);							//親のマスク対象を設定する
		bool getMaskParentSetting(void) { return maskParentSetting; };	//設定された親のマスク対象を取得する

	};


}	// namespace spritestudio6

#endif
