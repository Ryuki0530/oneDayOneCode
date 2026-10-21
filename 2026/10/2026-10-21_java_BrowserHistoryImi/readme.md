# Mini Browser History

使用言語: Java
目安: 15〜20分

## 課題
ブラウザの「ページを開く」「戻る」「進む」を再現する
BrowserHistoryクラスを作成してください。

## 実装要件
1. 初期ページは "home" とします。
2. visit(String page) で新しいページへ移動します。
   「戻る」で移動した後にvisitした場合、先の履歴を削除します。
3. back() と forward() で履歴を1つ移動します。
   移動できない場合は現在のページに留まります。
4. current() で現在のページ名を返します。
   ページ名は空でない文字列とし、同じページの連続訪問も履歴に残します。

## 動作例
次の順番で操作し、各操作後にcurrent()を表示してください。

操作:
初期状態
visit("news")
visit("article")
back()
visit("weather")
forward()
back()
back()
back()
forward()

期待出力:
home
news
article
news
weather
weather
news
home
home
news

## ヒント
ArrayList<String>で履歴を保持し、int型の変数で
現在の位置を管理すると実装しやすくなります。

## 完了条件
Main.javaの1ファイルで実行でき、
上記の期待出力と一致することを確認してください。