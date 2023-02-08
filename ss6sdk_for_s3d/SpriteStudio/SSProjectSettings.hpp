
# pragma once
# include <Siv3D.hpp>
# include "SSCommon.hpp"

namespace s3d
{
	class SSProjectSettings
	{
	public:

		SSProjectSettings() = default;

		~SSProjectSettings() = default;

		bool load(const XMLElement& settings, bool createEditorParam);

	private:

		FilePath           m_animeBaseDirectory      { U"" };   //!< .ssaeを保存するフォルダへの相対パス
		FilePath           m_cellMapBaseDirectory    { U"" };   //!< .ssceを保存するフォルダへの相対パス
		FilePath           m_imageBaseDirectory      { U"" };   //!< 画像を保存するフォルダへの相対パス
		FilePath           m_effectBaseDirectory     { U"" };   //!< .sseeを保存するフォルダへの相対パス
		FilePath           m_exportBaseDirectory     { U"" };   //!< 出力先フォルダへの相対パス。デフォルトはExport
		bool               m_queryExportBaseDirectory{ false }; //!< エクスポート先の基準ディレクトリ指定をさせるか？
		TextureAddressMode m_wrapMode                {};        //!< サンプラーステートに利用するラップモード
		TextureFilter      m_filterMode              {};        //!< サンプラーステートに利用するフィルターモード
		bool               m_vertexAnimeFloat        { false }; //!< 頂点変形アトリビュートの少数対応

		struct EditorParam
		{
			unknown32           m_copyWhenImportImageIsOutside { 0 };     //!< 画像参照が外部にある場合のコピーフラグ？
			String              m_exportAnimeFileFormat        { U"" };   //!< アニメーションの出力フォーマット
			String              m_exportCellMapFileFormat      { U"" };   //!< セルマップの出力フォーマット
			unknown32           m_exportCellMap                { 0 };     //!< セルマップをエクスポートするフラグ
			unknown32           m_copyImageWhenExportCellmap   { 0 };     //!< セルマップを出力する際、画像をコピーするフラグ？
			Array<unknownStr>   m_ssConverterOptions           {};        //!< ？
			String              m_player                       { U"" };   //!< 再生対象のプレイヤー。
			unknown32           m_strictVer4                   { 0 };     //!< Ver4互換
			unknown32           m_dontUseMatrixForTransform    { 0 };     //!< 変換に行列を使用しないフラグ？
			unknown32           m_rootPartFunctionAsVer4       { 0 };     //!< Ver4互換
			unknown32           m_interpolateColorBlendAsVer4  { 0 };     //!< Ver4互換
			unknown32           m_interpolateVertexOffsetAsVer4{ 0 };     //!< Ver4互換
			unknown32           m_restrictXYAsInteger          { 0 };     //!< ？
			unknown32           m_inheritRatesNoKeySave        { 0 };     //!< ？
			Array<String>       m_availableInterpolationTypes  {};        //!< 利用可能な補間の種類一覧
			Array<String>       m_availableAttributes          {};        //!< 利用可能なアトリビュート一覧
			Array<String>       m_availableFeatures            {};        //!< 利用可能な造形一覧
			Array<String>       m_defaultSetAttributes         {};        //!< 基本設定されるアトリビュート一覧
			String              m_interpolationType            {};        //!< 補間の種類。基本設定？
			SSRenderingSettings m_renderingSettings            {};        //!< レンダリング設定？
			SSEffectSettings    m_effectSettings               {};        //!< エフェクト設定？
			Array<String>       m_cellTags                     {};        //!< セル用タグ一覧？
			int32               m_useDecimalDigit              { 0 };     //!< 使用する10進用桁数
			bool                m_opacifyOutsideCanvasFrame    { false }; //!< キャンバスの外側を不透明にするフラグ
			unknown32           m_convertImageToPMA            { 0 };     //!< ？
			unknown32           m_blendImageAsPMA              { 0 };     //!< ？
			unknown32           m_unpremultiplyAlpha           { 0 };     //!< ？
		};
		std::unique_ptr<EditorParam> m_pEditorParam{ nullptr }; //!< エディタ用パラメータ
	};
}
