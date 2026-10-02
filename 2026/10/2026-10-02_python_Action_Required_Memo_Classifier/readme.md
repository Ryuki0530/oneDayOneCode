# Jevで「対応が必要なメモ」を判定する

使用言語: Python 3.10以上
目安: 15〜25分
ファイル名: main.py

## 目的
メモの文章から「これから対応する必要があるか」をJevで判断し、
返された確率をPythonで処理してください。
入力はテキストのみです。

## 準備
VSCodeのPowerShellで次を実行してください。
導入済みの場合はインストール不要です。

python -m pip install typesafe-sdk
$env:TYPESAFE_API_KEY = "取得済みのAPIキー"

この環境変数は現在のPowerShellで有効です。
同じターミナルから python main.py を実行してください。

## 入力データ
次の4件をコード内のリストに入れてください。

1. 明日までに参加申込フォームを提出してください。
2. 参加申込フォームは提出済みです。追加対応は不要です。
3. 来週の打ち合わせまでに資料を確認しておいてください。
4. 今日の打ち合わせでは、新しい開発環境が紹介されました。

## 要件
1. 各メモについて、Noulを使い
   「このメモは、読み手に未完了の対応を求めているか」
   を質問してください。
   期限の記載だけではなく、対応済みかどうかも判断させてください。

2. 返された値を次の基準で分類してください。
   0.7以上       : ACTION
   0.3より大きく0.7未満 : REVIEW
   0.3以下       : NO_ACTION

3. 各メモの番号、分類、確率を表示してください。
   確率は小数点以下3桁にしてください。

4. 最後にACTION、REVIEW、NO_ACTIONの件数を表示してください。

## SDKの使い方
読み込むもの:
from typesafe_sdk import Noul, TypeSafeClient

クライアント:
with TypeSafeClient() as client:

リクエスト:
client.system_one(
    state=メモの文字列,
    questions={
        "needs_action": Noul(instructions=判定の質問)
    }
)

結果の取得:
response.nouls["needs_action"].noul

noulは0〜1の値です。
対応の重要度ではなく、質問への答えが「はい」である確率です。

## 出力形式の例
以下の確率は説明用の仮の値です。

1: ACTION (0.950)
2: NO_ACTION (0.050)
3: ACTION (0.900)
4: NO_ACTION (0.100)

ACTION: 2
REVIEW: 0
NO_ACTION: 2

## 完了条件
・4件すべてについてAPIの結果を表示できること。
・分類と件数が、実際に返された確率と一致すること。
・境界値0.3がNO_ACTION、0.7がACTIONになること。

AIの判定は例と一致するとは限りません。
異なる場合は、その結果と質問文を見比べてください。