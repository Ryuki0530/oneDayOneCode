# 📷 今日の課題（Python / OpenCV）  
**テーマ：リアルタイム画像処理パイプラインの実装**

あなたのPCに接続されたカメラ（WebCam/USB/内蔵）を**リアルタイム**で使い、以下の3モードを切り替えて動作する画像処理ツールを作成せよ。  
実装は **Python + OpenCV(cv2)**（＋必要ならNumPy）に限定する。

---

## 要求仕様

### 1) 起動・入出力仕様
- 実行形式（コマンドライン引数対応）  
python main.py --source 0 --mode color --width 1280 --height 720 --record out.avi

- `--source`: カメラIDまたは動画ファイルパス（例：`0` / `1` / `sample.mp4`）
- `--mode`: `color` / `edge` / `motion`（起動時の初期モード）
- `--width`, `--height`: キャプチャ解像度（設定できない場合は可能な範囲に丸めてもよい）
- `--record`: 指定時、処理後フレームを動画保存（MJPG または XVID など利用可能な FourCC）

- 実行中のキー操作
- `c` … **color** モードへ切替（色領域抽出）
- `e` … **edge** モードへ切替（エッジ/輪郭）
- `m` … **motion** モードへ切替（フレーム間差分）
- `s` … 処理後フレームを PNG でスナップショット保存（連番）
- `i` … オーバレイ情報（FPS/解像度/検出数）表示の ON/OFF
- `q` / `ESC` … 終了

- 出力
- ウィンドウ表示：処理後フレーム（オーバレイで現在モード・FPS・検出数を表示）
- コンソール：1 秒に 1 回、**JSON Lines** 形式で統計を出力  
  例） `{"ts":"2025-12-23T21:03:12","mode":"motion","fps":29.8,"detections":3}`

---

### 2) 3つの処理モード

#### A. `color`（色領域抽出）
- 目的：指定色の物体をリアルタイム追跡  
- 仕様：
- HSV 空間でしきい値抽出（`cv2.cvtColor` + `cv2.inRange`）
- Trackbar（`cv2.createTrackbar`）で `H_low/H_high/S_low/S_high/V_low/V_high` を**ライブ調整**
- 抽出マスクの最大輪郭を検出し、**最小外接円**または**バウンディングボックス**を描画
- 検出がある場合は**重心座標**をオーバレイ表示

#### B. `edge`（エッジ・輪郭強調）
- 目的：シンプルな特徴抽出パイプラインの構築
- 仕様：
- グレースケール → ガウシアンぼかし → Canny エッジ検出
- Trackbar で `threshold1` / `threshold2` を調整
- 輪郭（`cv2.findContours`）を抽出し、面積上位 N 件（N ≥ 5）を描画
- 面積・外接矩形サイズの統計をコンソール JSON に含めても良い（任意）

#### C. `motion`（動体検知）
- 目的：背景差分による動きの可視化
- 仕様：
- 背景モデル：`cv2.createBackgroundSubtractorMOG2()` ※または直前フレーム差分
- モルフォロジー（開閉処理）でノイズ除去 → 輪郭検出
- 一定面積以上のみを「動体」とカウントし、矩形を描画
- 動体の**個数**と**合計面積**を JSON 出力に含める

---

### 3) オーバレイ & メタ情報
- 画面左上に以下を表示（`i` で表示切替可）
- 現在モード：`MODE: COLOR/EDGE/MOTION`
- FPS（移動平均でOK）・画像サイズ `WxH`
- 検出数（色なら最大輪郭=1でも良い／エッジは上位輪郭数／動体は検出個数）

---

### 4) 例：コンソール出力（1秒ごと）
```
{"ts":"2025-12-23T21:10:05","mode":"color","fps":30.1,"detections":1,"center":[640,360]}
{"ts":"2025-12-23T21:10:06","mode":"edge","fps":29.7,"detections":5}
{"ts":"2025-12-23T21:10:07","mode":"motion","fps":28.9,"detections":3,"area_sum":15420}
```
---

## 受け入れ条件（チェック項目）
- [ ] 引数 `--source` に **カメラID** と **動画パス** の両方が使える  
- [ ] 3 モードが**キー操作**で即時切替できる  
- [ ] `color` モードに **HSV Trackbar** があり、調整が反映される  
- [ ] `edge` モードに **Canny Trackbar** があり、調整が反映される  
- [ ] `motion` モードで**動体の個数**がカウントされ矩形描画される  
- [ ] `--record` 指定時に**処理後フレーム**が動画に保存される  
- [ ] `s` キーで**PNG スナップショット**が保存される  
- [ ] 1 秒に 1 回、**JSON Lines** で統計が標準出力に出る  
- [ ] `q` / `ESC` で安全に終了でき、リソース（カメラ/ウィンドウ/Writer）を解放

---

## 実装ヒント
- カメラ解像度は `cap.set(cv2.CAP_PROP_FRAME_WIDTH, ...)` / `CAP_PROP_FRAME_HEIGHT`  
- FPS 計測は `time.time()` で直近 N フレーム移動平均にすると安定  
- FourCC 例：`cv2.VideoWriter_fourcc(*"MJPG")`  
- Trackbar は `cv2.namedWindow` → `cv2.createTrackbar` → `cv2.getTrackbarPos`  
- JSON 出力は `json.dumps(dict, ensure_ascii=False)` を `print`  
- 処理順序は **キャプチャ → 前処理 → モード別処理 → オーバレイ → 表示/保存** の一本化

---

## 動作例コマンド
```bash
# 1) 内蔵カメラ、color モード、720p、録画あり
python main.py --source 0 --mode color --width 1280 --height 720 --record out.avi

# 2) 動画ファイルを入力にして edge モードで再生
python main.py --source sample.mp4 --mode edge
```