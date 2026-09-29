# Action Triage

## 課題

作業報告を1行入力し、Jevを使って次の対応を判定する
Pythonプログラムを作成してください。

- CONTINUE：問題なく作業を続けられる
- CHECK：状況の確認が必要
- STOP：作業を止めて対処する必要がある

## 準備（VSCodeのPowerShell）

py -m pip install typesafe-sdk

APIキーを取得し、現在のターミナルの環境変数に設定します。

$env:TYPESAFE_API_KEY = "取得したAPIキー"

APIキーはPythonファイルに書かないでください。

## 仕様

1. input()で作業報告を受け取る。
2. 報告文をJevのstateに渡す。
3. Choiceを使ってCONTINUE・CHECK・STOPから1つ選ばせる。
4. 選ばれた対応と、各選択肢の確率を表示する。
5. 空入力ならAPIを呼ばずに終了する。

## 試す入力

- 「予定どおり動作し、異常は見られませんでした」
- 「動作はしていますが、いつもより振動が大きいです」
- 「モーターから煙が出て、動作が停止しました」
- 「少し変な気がしますが、詳しい状況は不明です」

4件目をどう扱うか考えながら、各選択肢の説明を調整してください。

## Jev呼び出しのヒント

from typesafe_sdk import Choice, TypeSafeClient

client = TypeSafeClient()

response = client.system_one(
    state=報告文,
    questions={
        "action": Choice(
            instructions="作業報告を読んで、次に取るべき対応を選んでください",
            criteria={
                "CONTINUE": "異常の記述がなく、作業を続けられる",
                "CHECK": "異常の疑いがあるか、情報が不足している",
                "STOP": "明確な危険や故障があり、直ちに停止が必要",
            },
        )
    },
)

answer = response.answers["action"]
print(answer.choice)
print(answer.probabilities)