# 今日のRust課題：Cargo.toml 駆動の「プラグイン式コマンド」CLI

## ゴール
`Cargo.toml` に **自作ツールの設定** を書き込み、それを **実行時に読み取って動作を変える** CLI を作ってください。  
つまり「ビルド時の依存管理ファイル」だった `Cargo.toml` を、あなたのアプリの **設定ファイル** としても使います。

---

## 作るもの：`plugctl`
`plugctl` は「サブコマンド(=プラグイン)」を `Cargo.toml` で定義できる CLI です。

### 例：Cargo.toml にこう書く
```toml
[package.metadata.plugctl]
default = "hello"

[[package.metadata.plugctl.plugins]]
name = "hello"
message = "Hello from Cargo.toml!"
repeat = 2

[[package.metadata.plugctl.plugins]]
name = "bye"
message = "Bye!"
repeat = 1
```

---

## 仕様
### 入力（コマンド）
- `plugctl list`
  - 定義されている plugin の `name` を一覧表示（1行1個）
- `plugctl run <name>`
  - 指定した plugin の `message` を `repeat` 回だけ表示
- `plugctl run`
  - `<name>` が省略された場合は `default` を使う
- `plugctl show <name>`
  - 指定 plugin の設定（name/message/repeat）を読みやすく表示（形式は自由）
- `plugctl validate`
  - 設定に問題があればエラー終了（終了コード 1）
  - 問題なければ `OK` と表示して終了（終了コード 0）

### 設定のバリデーションルール（validate で使う）
- `default` が設定されているなら、その名前の plugin が存在すること
- plugin `name` は重複禁止
- `repeat` は 1 以上
- plugin が 0 件の場合はエラー（このツールの存在意義が消えるので）

### Cargo.toml の場所
- 実行時のカレントディレクトリにある `Cargo.toml` を読む
- 読めない／パースできない場合はエラーメッセージを出して終了

---

## 制約（わざと現実寄りに）
- 設定は `Cargo.toml` の **`package.metadata.plugctl`** を使う（他は禁止）
- Rust 2021 edition
- 外部クレート使用OK（むしろ使っていい）
  - 例：`toml`, `serde`, `serde_derive`, `clap` など

---

## 期待するディレクトリ構成（例）
- `src/main.rs`
- `src/config.rs`（Cargo.toml 読み取り＆検証）
- `src/cli.rs`（引数処理）
- `Cargo.toml`（ここに `metadata` を書く）

※ファイル分割は任意だけど、最低でも「設定読み取り」と「CLI処理」は分けると気持ちいいです。

---

## 実装ヒント
### 1) Cargo.toml を読む
- `std::fs::read_to_string("Cargo.toml")`

### 2) TOML をパースして `serde` で構造体に落とす
- `toml::from_str::<Root>(&s)` みたいな流れ  
- `package.metadata.plugctl` に到達するために、上位の構造体を素直に定義するか、
  `toml::Value` で掘ってから `try_into()` するか、どちらでもOK

### 3) validate の実装
- `HashSet` で name 重複チェック
- default の存在確認
- repeat >= 1 チェック

---

## 追加チャレンジ（余裕があれば）
- `plugctl run --all` で全プラグインを順に実行
- `--cargo <path>` で任意の Cargo.toml を指定できるようにする
- `repeat` が大きいときに `--max-repeat` で安全弁を作る（暴走対策）

---

## 提出物
- 動くRustコード一式
- `Cargo.toml` に `package.metadata.plugctl` を書いた例
- 実行例（コマンドと出力を数個）

---

## ねらい（学べること）
- `Cargo.toml` の `package.metadata.*` は「自由に使える名前空間」なので、
  自作ツールの設定を埋め込むのにちょうどいい
- `serde` + `toml` の設定読み取り
- CLI設計（validate / show / list / run）とエラーハンドリング
