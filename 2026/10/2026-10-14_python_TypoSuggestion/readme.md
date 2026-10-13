# Typo Suggestion

使用言語: Python
目安: 15〜20分

## 目的
入力した単語が登録されていない場合に、似ている単語を候補として表示するプログラムを作成してください。

## 実装要件
1. 次の登録済み単語を使用してください。
   ["start", "stop", "status", "restart", "reset"]

2. suggest_command(word) 関数を作成してください。
   入力の前後の空白を除去し、小文字に変換して判定します。

3. 登録済み単語と一致した場合は「OK: 単語」を返してください。
   空文字の場合は「EMPTY」を返してください。

4. 一致しない場合は、difflib.get_close_matches を使って
   類似度0.6以上の候補を最大2件取得してください。
   候補があれば「候補: 単語1, 単語2」、
   なければ「候補なし」を返してください。
   候補の順番はAPIが返した順番を使用します。

## 入力例
次の文字列を順番に関数へ渡してください。

" START "
"stats"
"xyz"
"   "

## 期待出力
OK: start
候補: status, start
候補なし
EMPTY

## APIの使い方
difflibは標準ライブラリなので、追加インストールは不要です。

from difflib import get_close_matches

get_close_matches(word, possibilities, n=2, cutoff=0.6)

- word: 比較したい文字列
- possibilities: 候補となる文字列のリスト
- n: 取得する候補の最大件数
- cutoff: 候補として採用する類似度の下限
- 戻り値: 類似度が高い順の文字列リスト

## 完了条件
入力例の4件で期待出力と一致し、別の入力でも候補を表示できれば完了です。