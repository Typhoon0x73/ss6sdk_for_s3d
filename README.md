# ss6sdk_for_s3d
このプロジェクトはOpenSiv3Dでスプライトスタジオのデータを使えるようにしたいプロジェクトです。  
現在、プライベートリポジトリにて作業継続中です！！  

# 開発環境
OS  : Windows 10 pro  
Lib : [OpenSiv3D v0.6.6](https://github.com/Siv3D/OpenSiv3D)  
IDE : Microsoft Visual Studio Community 2022 (64 ビット) Version 17.4.2  

# ThirdParty
### SpriteStudio6-SDK
※必要に応じて一部のソースを改変しています。  
| 項目 | 詳細 |
----|----
| ライセンスの場所 | ./ss6sdk_for_s3d/ThirdParty/ss6sdk/LICENSE |
| おいてあるパス | ./ss6sdk_for_s3d/ThirdParty/ss6sdk/* |
| リポジトリのURL | https://github.com/SpriteStudio/SpriteStudio6-SDK |
| 利用しているファイルのコミット | f6eaa3fbfc8298d8445b229840592226110c0c8c |  
  
### tinyxml2
OpenSiv3DのLibに含まれてますが、ヘッダを外に置いてないので、  
OpenSiv3Dのリポジトリからコピーし`./ss6sdk_for_s3d/ThirdParty/tinyxml2/tinyxml2.h`へ配置しています。  
  
### スプライトスタジオのサンプルデータ
`./ss6sdk_for_s3d/App/ss6sample/*`に配置してあります。  
※※※ !!! データごとにライセンスがあります。各フォルダ内のreadmeを必ず参照してください !!! ※※※  
※※※ !!! データごとにライセンスがあります。各フォルダ内のreadmeを必ず参照してください !!! ※※※  
※※※ !!! データごとにライセンスがあります。各フォルダ内のreadmeを必ず参照してください !!! ※※※  
  
ほかにも追加で必要な場合  
[こちら](https://www.webtech.co.jp/help/ja/spritestudio/download/sample/)のサイトから必要なものをダウンロードし利用してください。  

# 各テストの実行方法
1. ビルド構成を`USE_TEST_MAIN`へ切り替えます。  
2. `./ss6sdk_for_s3d/TestMain/__TestDefine.h`から実行したいテストのマクロ定義を一つだけ有効にします。  
※他は無効にしてください。  
3. 各テストファイルの先頭にコメントで使用方法が記載されているので、確認して実行します。  
以上  

構成コピー元はDebugで、プリプロセッサ定義に(USE_TEST_MAIN)を追加しただけです。  
定義名から実行するテストファイルを推測できます。  
テスト詳細は該当の各`./ss6sdk_for_s3d/TestMain/*.cpp`ファイルを確認ください。  
  
# 各サンプルの実行方法
1. ビルド構成を`USE_SAMPLE_MAIN`へ切り替えます。  
2. `./ss6sdk_for_s3d/SampleMain/__SampleDefine.h`から実行したいサンプルのマクロ定義を一つだけ有効にします。  
※他は無効にしてください。  
3. 各サンプルファイルの先頭にコメントで使用方法が記載されているので、確認して実行します。  
以上  

構成コピー元はDebugで、プリプロセッサ定義に(USE_SAMPLE_MAIN)を追加しただけです。  
定義名から実行するサンプルファイルを推測できます。  
サンプルの詳細は該当の各`./ss6sdk_for_s3d/SampleMain/*.cpp`ファイルを確認ください。  