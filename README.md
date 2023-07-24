# ss6sdk_for_s3d
このプロジェクトはOpenSiv3Dでスプライトスタジオのデータを使えるようにしたいプロジェクトです。  

# 開発環境
OS  : Windows 10 pro  
Lib : [OpenSiv3D v0.6.6](https://github.com/Siv3D/OpenSiv3D)  
IDE : Microsoft Visual Studio Community 2022 (64 ビット) Version 17.4.2  

# ThirdParty
### SpriteStudio6-SDK
※必要に応じて一部のソースを改変しています。  
| 項目                           | 詳細                                              |
| ------------------------------ | ------------------------------------------------- |
| ライセンスの場所               | ./use_sdk/ThirdParty/ss6sdk/LICENSE        |
| おいてあるパス                 | ./use_sdk/ThirdParty/ss6sdk/*              |
| リポジトリのURL                | https://github.com/SpriteStudio/SpriteStudio6-SDK |
| 利用しているブランチ           | develop                                           |
| 利用しているファイルのコミット | da5035866805eb48576b8464cbe49fc2d27684a0          |
  
### tinyxml2
OpenSiv3DのLibに含まれてますが、ヘッダを外に置いてないので、  
OpenSiv3Dのリポジトリからコピーし`./use_sdk/ThirdParty/tinyxml2/tinyxml2.h`へ配置しています。  
  
### スプライトスタジオのサンプルデータ
`./ss6sdk_for_s3d/App/ss6sample/*`に配置してあります。  

データごとにライセンスがあります。  
各フォルダ内のreadmeを必ず参照してください。  
  
ほかにも追加で必要な場合  
[こちら](https://www.webtech.co.jp/help/ja/spritestudio/download/sample/)のサイトから必要なものをダウンロードし利用してください。  

# use_sdkプロジェクト

## 各テストの実行方法
1. ビルド構成を`USE_TEST_MAIN`へ切り替えます。  
2. `./use_sdk/TestMain/__TestDefine.h`から実行したいテストのマクロ定義を一つだけ有効にします。  
※他は無効にしてください。  
3. 各テストファイルの先頭にコメントで使用方法が記載されているので、確認して実行します。  
以上  

構成コピー元はDebugで、プリプロセッサ定義に(USE_TEST_MAIN)を追加しただけです。  
定義名から実行するテストファイルを推測できます。  
テスト詳細は該当の各`./use_sdk/TestMain/*.cpp`ファイルを確認ください。  
  
## 各サンプルの実行方法
1. ビルド構成を`USE_SAMPLE_MAIN`へ切り替えます。  
2. `./use_sdk/SampleMain/__SampleDefine.h`から実行したいサンプルのマクロ定義を一つだけ有効にします。  
※他は無効にしてください。  
3. 各サンプルファイルの先頭にコメントで使用方法が記載されているので、確認して実行します。  
以上  

構成コピー元はDebugで、プリプロセッサ定義に(USE_SAMPLE_MAIN)を追加しただけです。  
定義名から実行するサンプルファイルを推測できます。  
サンプルの詳細は該当の各`./use_sdk/SampleMain/*.cpp`ファイルを確認ください。  

## 対応項目  
 - Parts  
   - [ ] null  
   - [ ] normal  
   - [ ] shape  
   - [ ] text  
   - [ ] nines  
   - [ ] instance  
   - [ ] armature  
   - [ ] effect  
   - [ ] mesh  
   - [ ] movenode  
   - [ ] constraint  
   - [ ] mask  
   - [ ] joint  
   - [ ] bonejoint  
   - [ ] transform_constraint  
   - [ ] camera  
   - [ ] audio  
 - Attribute
   - [x] cell  
   - [x] pos_x  
   - [x] pos_y  
   - [x] pos_z  
   - [ ] rot_x  
   - [ ] rot_y  
   - [x] rot_z  
   - [ ] scale_x  
   - [ ] scale_y  
   - [ ] local_scale_x  
   - [ ] local_scale_y  
   - [x] alpha  
   - [x] local_alpha  
   - [x] prio  
   - [ ] flip_h  
   - [ ] flip_v  
   - [ ] hide  
   - [ ] parts_color  
   - [ ] vertex_color  
   - [ ] shader  
   - [ ] vertex_trans  
   - [ ] pivot_x  
   - [ ] pivot_y  
   - [ ] anchor_x  
   - [ ] anchor_y  
   - [ ] size_x  
   - [ ] size_y  
   - [ ] image_flip_h  
   - [ ] image_flip_v  
   - [ ] uv_trans_x  
   - [ ] uv_trans_y  
   - [ ] uv_rotate_z  
   - [ ] uv_scale_x  
   - [ ] uv_scale_y  
   - [ ] bounds_r  
   - [ ] mask  
   - [ ] user  
   - [ ] signal  
   - [ ] instance_param  
   - [ ] effect_param  
   - [ ] deform_param  
   - [ ] audio_param  
   - [ ] texture_change  
 - DrawMode
   - [ ] mix  
   - [ ] mul  
   - [ ] add  
   - [ ] sub  
   - [ ] mul_alpha  
   - [ ] screen  
   - [ ] exclusion  
   - [ ] invert  
 - InterpolationType
   - [ ] none  
   - [ ] linear  
   - [ ] hermite  
   - [ ] bezier  
   - [ ] acceleration  
   - [ ] deceleration  
   - [ ] ease_in  
   - [ ] ease_out  
   - [ ] ease_in_out  
   - [ ] ease_exponential_in  
   - [ ] ease_exponential_out  
   - [ ] ease_exponential_in_out  
   - [ ] ease_sine_in  
   - [ ] ease_sine_out  
   - [ ] ease_sine_in_out  
   - [ ] ease_elastic_in  
   - [ ] ease_elastic_out  
   - [ ] ease_elastic_in_out  
   - [ ] ease_bounce_in  
   - [ ] ease_bounce_out  
   - [ ] ease_bounce_in_out  
   - [ ] ease_back_in  
   - [ ] ease_back_out  
   - [ ] ease_back_in_out  