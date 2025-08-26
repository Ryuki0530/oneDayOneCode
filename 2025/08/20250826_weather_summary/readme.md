# OneDayOneCode #2025-08-26（Java 単一ファイル）

## 課題  
Open-Meteo を使った「都市名 → 日次天気サマリ」CLI  
（APIキー不要・Gradle不使用・単一 .java）

---

## 目的

- APIキー不要の Open-Meteo を用いて、都市名から緯度経度を取得
- 日本時間の予報を取得し、日次で集計
- 所定フォーマットで出力・保存できる単一ファイルの Java CLI を作成
- 外部ライブラリ（Gson/Jackson など）やビルドツール（Gradle/Maven）は禁止
- `java.net.http.HttpClient`（Java 11+）と自作の軽量 JSON パーサで対応

---

## 仕様（コマンドライン引数）

```sh
java Main --city <CITY_NAME> [--days <N=3>] [--format <table|csv|json>] [--out <PATH>] [--debug]
```

- `--city`（必須）: 例 東京 / Nagoya / New York
- `--days`（任意）: 取得日数（1〜7 程度、既定は 3）
- `--format`（任意）: 出力形式（既定 table）
    - `table` … 人が読みやすい表
    - `csv` … ヘッダ付き CSV
    - `json` … 配列 JSON（UTF-8）
- `--out`（任意）: 指定パスに標準出力と同じ内容を保存（拡張子は csv または json を推奨）
- `--debug`（任意）: リクエスト URL、HTTP ステータス、リトライ回数、パース警告などを標準エラーに出力

---

## 終了コード

| コード | 内容                                      |
|-------|-------------------------------------------|
| 0     | 成功                                      |
| 1     | 引数不正・使い方誤り                      |
| 2     | ネットワーク/HTTP エラー（タイムアウト含む）|
| 3     | データ不整合（都市が見つからない等）      |

---

## 処理フロー

### 1. 都市名から座標を取得（Geocoding API）

- エンドポイント:  
  `https://geocoding-api.open-meteo.com/v1/search`
- 主なクエリ:
    - `name=<CITY_NAME>` … 都市名（日本語/英語どちらも可）
    - `language=ja` … 日本語応答を優先
    - `count=5` … 候補数
- 応答例:
    ```json
    {
      "results": [
        {
          "name": "Tokyo",
          "latitude": 35.6895,
          "longitude": 139.6917,
          "country_code": "JP",
          "timezone": "Asia/Tokyo"
        }
      ]
    }
    ```
- 必須抽出項目: `latitude`, `longitude`, `country_code`, `timezone`, `name`
- 0件なら終了コード 3。複数あっても最上位1件のみ採用。`--debug`時は候補一覧をstderrに表示。

---

### 2. 予報を取得（Forecast API）

- エンドポイント:  
  `https://api.open-meteo.com/v1/forecast`
- 主なクエリ:
    - `latitude=<LAT>` / `longitude=<LON>`
    - `hourly=temperature_2m,precipitation,wind_speed_10m`
    - `timezone=Asia/Tokyo`
    - `forecast_days=<N>`
- 応答例:
    ```json
    {
      "latitude": 35.7,
      "longitude": 139.7,
      "timezone": "Asia/Tokyo",
      "hourly": {
        "time": ["2025-08-26T00:00", ...],
        "temperature_2m": [27.3, ...],
        "precipitation": [0.0, ...],
        "wind_speed_10m": [18.2, ...]
      }
    }
    ```

---

### 3. 日次集計（Asia/Tokyo 基準）

- `hourly.time` の ISO 文字列（YYYY-MM-DDTHH:mm）を日付 YYYY-MM-DD に切り出し、同日の各値を集計
- 出力レコード（1日あたり1行）:
    - `date` … YYYY-MM-DD
    - `t_min` … temperature_2m の最小
    - `t_max` … temperature_2m の最大
    - `precip_sum` … precipitation の合計（mm）
    - `wind_max` … wind_speed_10m の最大
    - `hours` … その日に集計に使った時刻数
- 並び順: date 昇順
- 欠損: 配列長が異なる・値が null の場合はスキップし、`--debug` で警告

---

## 出力形式

### table（既定）

```
CITY: Tokyo (JP), lat=35.6895, lon=139.6917, tz=Asia/Tokyo
DAYS: 3

+------------+--------+--------+------------+----------+-------+
| date       | t_min  | t_max  | precip_sum | wind_max | hours |
+------------+--------+--------+------------+----------+-------+
| 2025-08-26 |  --    |   --   |     --     |    --    |  --   |
...
```

### csv

```
date,t_min,t_max,precip_sum,wind_max,hours
2025-08-26, , , , , 
```

### json（配列）

```json
[
  {"date":"2025-08-26","t_min":null,"t_max":null,"precip_sum":null,"wind_max":null,"hours":0}
]
```

- `--out` 指定時は同内容を保存（既存ファイルは上書き）

---

## HTTP/信頼性要件

- `HttpClient` を使用。接続/読み込みタイムアウトは5秒程度
- リトライ: ステータス 429/502/503/504 のとき指数バックオフ（例: 0.5s→1s→2s、最大3回）
- User-Agent などは簡潔に設定
- ネットワーク未接続やタイムアウトは終了コード2

---

## JSON パース要件（外部ライブラリ禁止）

- 必要なキーの抽出に限定し、自作の軽量パーサで対応
- 配列同士の同一インデックス対応（time[i] vs temperature_2m[i] 等）を壊さないこと
- 不正 JSON（想定キー欠落）はスキップし、最終的に集計不能なら終了コード3
- 文字コードは UTF-8 前提

---

## 使い方（実行例・出力イメージ）

### 表形式（既定）

```sh
javac Main.java
java Main --city 東京
```

### JSONで5日分を保存

```sh
java Main --city Nagoya --days 5 --format json --out .\nagoya_5days.json
```

### デバッグログを見ながらCSV出力

```sh
java Main --city Sapporo --format csv --debug
```

- 先頭に CITY メタ情報（都市名、国コード、lat/lon、APIが返した timezone）を1行表示（json の場合はトップに meta オブジェクトを含めても良い）
- 次に日付ごとの行が `--format` に従って表示される
- 実数の小数点桁は 1〜2 桁程度に統一

---

## 評価観点

- Geocoding API で最上位候補から lat/lon/country_code/timezone/name を取得できている
- Forecast API から hourly の time / temperature_2m / precipitation / wind_speed_10m を取得し、日本時間ベースで正しく日次集計できている
- 3形式（table/csv/json）を切り替え可能で、`--out` 保存も正しく行える
- タイムアウト・指数バックオフ・警告ログ（--debug）など、堅牢性の要件を満たす
- 単一 .java ファイルで完結し、外部ライブラリ不使用

---

## 参考・ヒント

- 軽量 JSON パーサは「文字列走査＋状態機械」で十分実装可能
- table 出力は列幅計算→桁揃え→罫線生成の順に作ると綺麗
- 例外は起点で捕捉し終了コードへマップ。中では例外を極力投げて、外で一元ハンドリング
- 文字列フォーマットは `String.format(Locale.ROOT, ...)` を使って小数桁を揃えると見やすい

---


