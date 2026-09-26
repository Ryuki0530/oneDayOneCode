# OneDayOneCode: DogDetectorWithJev

## 概要

Webカメラの映像を利用して、現在カメラに犬が写っているか、
写っている場合は茶色の犬か黒色の犬かをJevに判断させる
Pythonプログラムを作成してください。

今回の目的は、Jevの特徴である「Decision Model」を利用して、

    カメラからの観測
        ↓
    Jevによる判断
        ↓
    型の決まった結果
        ↓
    Pythonプログラムの処理

という流れを体験することです。


# 判定する状態

JevのChoiceを利用して、必ず次の3種類のいずれかとして
状態を取得してください。

    NO_DOG
    BROWN_DOG
    BLACK_DOG

それぞれの意味は次の通りです。

    NO_DOG
        カメラに犬が写っていない

    BROWN_DOG
        茶色の犬が写っている

    BLACK_DOG
        黒色の犬が写っている


# 環境

OS:
    Windows 11

エディタ:
    VSCode

言語:
    Python

使用するもの:
    Webカメラ
    Jev
    OpenCV


# Jevの導入

## 1. Pythonの確認

VSCodeでターミナルを開き、以下を実行してください。

    python --version


## 2. プロジェクトディレクトリの作成

    mkdir DogDetectorWithJev
    cd DogDetectorWithJev


## 3. Python仮想環境の作成

    python -m venv .venv


PowerShellの場合は以下で有効化します。

    .\.venv\Scripts\Activate.ps1


## 4. 必要なライブラリのインストール

Webカメラを使用するためにOpenCVをインストールします。

また、JevをPythonから利用するためのSDKもインストールしてください。

公式ドキュメントを確認し、現在推奨されているJev SDKの
インストール方法を使用してください。


## 5. Jevを利用できるようにする

TypeSafe AIのJevを利用するためのアカウント設定を行い、
必要なAPIキーを取得してください。

APIキーはPythonコードへ直接記述せず、
環境変数などを利用して管理してください。

Jevを初めて使用するため、この段階で一度公式ドキュメントの
Quick Startも確認してください。


# 課題

Webカメラを起動し、カメラ映像を取得してください。

カメラから得られた情報をJevへ渡し、

    NO_DOG
    BROWN_DOG
    BLACK_DOG

の3つから現在の状態を判断させてください。


## Step 1: カメラを起動する

OpenCVを利用してWebカメラへ接続してください。

プログラム実行中はカメラ映像をウィンドウへ表示します。


## Step 2: カメラから画像を取得する

カメラから現在のフレームを取得してください。

取得した画像を、Jevが判断するための観測情報として利用します。


## Step 3: Jevへ状態を問い合わせる

Jevへ現在の観測情報を渡します。

JevのChoiceを利用して、選択可能な結果を

    NO_DOG
    BROWN_DOG
    BLACK_DOG

の3種類に限定してください。


## Step 4: 判定結果を取得する

Jevから返されたChoiceを取得してください。

自由な文章を生成させ、その文章をPython側で解析して

    "brown"が含まれていたらBROWN_DOG

のように判定する方法は禁止します。

Jevの型付きDecisionを利用してください。


## Step 5: 判定結果を表示する

Jevから取得した状態をコンソールへ表示してください。

例:

    Decision: BROWN_DOG

または、

    Decision: BLACK_DOG

犬が存在しない場合は、

    Decision: NO_DOG

と表示してください。


# 動作確認

最低でも次の3パターンを試してください。

パターン1:

    犬がカメラに写っていない

期待結果:

    NO_DOG


パターン2:

    茶色の犬をカメラに写す

期待結果:

    BROWN_DOG


パターン3:

    黒色の犬をカメラに写す

期待結果:

    BLACK_DOG


# 完成条件

以下をすべて満たせば完成です。

- Webカメラの映像を取得できる
- Jevへ観測情報を渡せる
- JevのChoiceを使用している
- ChoiceがNO_DOG / BROWN_DOG / BLACK_DOGに制限されている
- Jevから型付きの判定結果を取得できる
- 判定結果をPythonから利用できる


# 今回のポイント

通常の生成AIへ、

    What is in this image?

と質問して文章を生成させることが目的ではありません。

今回作りたいのは、

    Webcam
       |
       v
    Observation
       |
       v
      Jev
       |
       +---- NO_DOG
       |
       +---- BROWN_DOG
       |
       +---- BLACK_DOG

という小さな認識・判断システムです。

Jevが返す結果を最初から3種類に限定することで、
AIの出力をそのままプログラムの状態として利用できることを
確認してください。