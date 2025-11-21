# 🐶 C++ 課題：TheDogAPI から犬画像を取得して「実際に表示せよ」

## 🎯 目的
- C++ から HTTP GET を行い API を利用する  
- JSON を解析し、画像 URL を取得する  
- 画像データをダウンロードする  
- Windows の標準画像ビューアで開く（自動表示）

## 📡 使用 API
TheDogAPI（無料・キー不要）  
```
https://api.thedogapi.com/v1/images/search
```

レスポンス例（1件）
```json
[
  {
    "id": "BJa4kxc4X",
    "url": "https://cdn2.thedogapi.com/images/BJa4kxc4X.jpg"
  }
]
```

## 📝 要求仕様
1. API へ GET を送り、最初の `url` を取得する  
2. その URL から画像データをダウンロードする  
3. 一時ファイル（例：`dog.jpg`）として保存する  
4. 保存した画像を OS のデフォルト画像ビューアで自動表示する  
   - Windows の場合：  
     ```
     ShellExecuteA(NULL, "open", "dog.jpg", NULL, NULL, SW_SHOW);
     ```
5. 失敗時は適切なエラー表示を行う  
6. コードは `main.cpp` 一つで構わない

## 🧰 使用ライブラリ
- HTTP通信：`libcurl`  
- JSON解析：`nlohmann/json`（header-only）

## 🖥️ 実行例
```
今日の犬画像 URL:
https://cdn2.thedogapi.com/images/H1NIzRj7n.jpg

→ dog.jpg を開きます…
```
結果として OS の画像ビューアが開き、犬の画像が出ること。

## 🐕 拡張課題（任意）
- 画像を 3 枚ダウンロードして連続表示  
- 保存先を引数で設定できるようにする  
- 犬種（breed）を指定して検索する CLI オプションを追加する

```
