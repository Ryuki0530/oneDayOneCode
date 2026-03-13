# Deduplicated Event Counter

## 問題
イベントログが時刻順に与えられます。
各行は次の形式です。

    時刻 ユーザーID イベント名

ただし、同じユーザーが同じイベントを直前に受理されてから W 秒以内に再度送った場合、そのイベントは無視します。

受理されたイベントだけを対象にして、以下を出力してください。

- 受理されたイベント総数
- イベント名ごとの受理回数（辞書順）

## 入力
1行目に整数 W が与えられる。
2行目以降はイベントログで、各行は次の形式。

    timestamp user event

- timestamp は整数
- 入力は時刻昇順で与えられる
- 行数は EOF まで続く

## 出力
まず総数を次の形式で出力してください。

    TOTAL 総数

その後、イベント名ごとの件数をイベント名の辞書順で次の形式で出力してください。

    EVENT イベント名 回数

## ルール
ある (user, event) の組について、前回受理された時刻を last とすると、

- timestamp - last <= W なら無視
- それ以外なら受理

## 入力例
```
3
1 alice LOGIN
2 alice LOGIN
4 bob VIEW
5 alice LOGIN
7 alice VIEW
8 alice VIEW
```
## 出力例
```
TOTAL 4
EVENT LOGIN 2
EVENT VIEW 2
```

## 説明
- 2 alice LOGIN は、1 alice LOGIN から 3 秒以内なので無視
- 8 alice VIEW は、7 alice VIEW から 1 秒後なので無視

## ヒント
- (user, event) ごとの最後に受理した時刻を記録する
- strcmp で文字列比較
- 件数管理には配列でも構造体配列でもOK
- 入力は scanf で EOF まで読むと楽です

## 目安
- 想定時間: 15〜25分
- 目標: 構造体と文字列処理に慣れる