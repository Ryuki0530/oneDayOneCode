# 今日のC課題
## タイトル
Simple HTTP Query Server

## 概要
WSL 上で動作する、**簡易HTTPサーバ**を C で作成してください。  
ブラウザから GET リクエストでアクセスされたとき、URL に含まれるクエリ文字列を読み取り、その内容に応じたテキストを返します。

たとえば、次のようなアクセスに応答できるようにします。

- `http://localhost:8080/?name=Taro`
- `http://localhost:8080/?a=12&b=30`
- `http://localhost:8080/?mode=upper&text=hello`

---

## 問題
TCP ソケットを使って待ち受けを行うHTTPサーバを作成してください。  
受信した GET リクエストからパスとクエリ文字列を取り出し、以下のルールで応答してください。

### 1. `name` が指定されている場合
例:
`/?name=Taro`

応答本文:
`Hello, Taro!`

---

### 2. `a` と `b` が指定されている場合
例:
`/?a=12&b=30`

応答本文:
`sum=42`

※ `a` と `b` は整数とします。

---

### 3. `mode=upper` と `text` が指定されている場合
例:
`/?mode=upper&text=hello`

応答本文:
`HELLO`

英小文字を大文字に変換してください。

---

### 4. 上記のどれにも当てはまらない場合
応答本文:
`bad request`

---

## 必須仕様
- C 言語で実装すること
- Linux / WSL 上で動作すること
- ポート `8080` で待ち受けること
- `GET` リクエストのみ対応でよい
- 応答は `Content-Type: text/plain` とすること
- 1回のアクセスに応答したら接続を閉じてよい
- 複数同時接続対応は不要
- URLデコードは不要（まずはそのままでよい）

---

## 入出力形式
この課題は標準入力ではなく、**HTTP通信**で入出力を行います。

ブラウザや `curl` で確認してください。

---

## 実行例

### アクセス例1
`http://localhost:8080/?name=Taro`

### 応答例1
```text
Hello, Taro!
```

### アクセス例2
`http://localhost:8080/?a=10&b=25`

### 応答例2
```text
sum=35
```

### アクセス例3
`http://localhost:8080/?mode=upper&text=network`

### 応答例3
```text
NETWORK
```

### アクセス例4
`http://localhost:8080/`

### 応答例4
```text
bad request
```

---

## ヒント
- `socket`
- `bind`
- `listen`
- `accept`
- `recv`
- `send`
- `close`

HTTPリクエストの1行目はたとえば次のようになります。

```text
GET /?name=Taro HTTP/1.1
```

まずはこの1行目を取り出し、
- メソッド
- パス
- クエリ文字列

を分解すると進めやすいです。

---

## コンパイル例
```bash
gcc main.c -o server
```

---

## 実行例
```bash
./server
```

---

## 動作確認例
```bash
curl "http://localhost:8080/?name=Taro"
curl "http://localhost:8080/?a=12&b=30"
curl "http://localhost:8080/?mode=upper&text=hello"
```

---

## 追加課題
余裕があれば次も挑戦してください。

1. `mode=lower` を追加して小文字変換に対応する
2. `/health` にアクセスしたら `ok` を返す
3. `HTTP/1.1 200 OK` だけでなく、条件によって `400 Bad Request` を返す
4. URLデコード（ `%20` など ）に対応する
5. 複数リクエストを連続で処理できるようにする

---

## 学べること
- Cでのソケット通信の基礎
- HTTPリクエストの最小構成
- 文字列解析
- WSL上でのネットワークプログラミング
- ブラウザ / curl との接続確認