# Today's C++ Challenge - Unique Word Counter

## Problem
標準入力から複数行の文字列を受け取り、**単語ごとの出現回数**を数えてください。

ただし、以下のルールに従います。

- 単語は **空白区切り** で判定する
- 大文字と小文字は区別しない  
  - 例: `Apple` と `apple` は同じ単語
- 記号はそのまま無視してよいことにします  
  - 今回は「英字のみが入力される」と考えてOKです

最後に、単語と出現回数を **辞書順** で出力してください。

## Input
複数行の文字列が入力されます。  
EOF まで読み込んでください。

## Output
各単語について、以下の形式で辞書順に出力してください。

word count

## Example

### Input
apple orange Apple
banana apple
orange banana banana

### Output
apple 3
banana 3
orange 2

## Hint
- `std::map`
- `std::string`
- `std::transform`
- `tolower`
- `while (std::cin >> s)`

## Goal
10〜20分くらいで終わる想定です。  
余裕があれば、出現回数の多い順に並べる版にも挑戦してみてください。