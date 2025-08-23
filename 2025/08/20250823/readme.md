# 1Day 1Code — Java 課題（OpenGL 編）

## 概要
LWJGL（Lightweight Java Game Library）+ GLFW + OpenGL を使って、**回転する図形を描画**しつつ、**標準入出力で挙動を操作**する小さなレンダリング・システムを作成せよ。  
環境は Windows 11／VSCode 前提とする。

---

## 目標仕様
1. **ウィンドウ生成**  
   - タイトル：`OD1C OpenGL`  
   - 初期サイズ：`800x600`、リサイズ可  
   - VSync ON/OFF 切り替え可（初期 ON）

2. **描画内容**  
   - **回転する三角形**をデフォルトで描画する。  
   - 標準入力からのコマンドで図形を切り替え：  
     - `shape triangle` / `shape quad` / `shape star`  
   - 回転角は時間ベースで更新する。

3. **入出力**  
   - **標準入力（stdin）**から以下のコマンドを受け付ける：  
     - `speed <float>` … 回転速度（度/秒）  
     - `color <r> <g> <b>` … 0〜1 の実数で色指定  
     - `shape <name>` … `triangle|quad|star`  
     - `vsync <on|off>` … VSync 切り替え  
     - `fps` … 現在の FPS を標準出力に出力  
     - `quit` … 正常終了（GL リソース解放を含む）
   - **標準出力（stdout）**は以下のように返す：  
     - 起動時：`READY`  
     - `fps`：`FPS <number>`  
     - `speed`：`OK SPEED <value>`  
     - `color`：`OK COLOR <r> <g> <b>`  
     - `shape`：`OK SHAPE <name>`  
     - `vsync`：`OK VSYNC <on|off>`  
     - `quit`：`BYE`  
   - 不正コマンドは `ERR <message>` を返す。

4. **終了処理**  
   - VAO/VBO、シェーダ、ウィンドウ、GLFW を必ず解放して終了する。

---

## 入力例
```
speed 120
color 0.2 0.8 1
shape star
fps
vsync off
quit
```
## 出力例
```
READY
OK SPEED 120.0
OK COLOR 0.2 0.8 1.0
OK SHAPE star
FPS 59.7
OK VSYNC off
BYE
```
---

## 実装要件
- 言語：**Java 17 以降**  
- 外部ライブラリ：**LWJGL 3（GLFW, OpenGL）**  
- ビルド：Gradle 推奨  
- 標準入力はノンブロッキングまたは別スレッドで処理し、描画ループを止めないこと。  
- シェーダは最小限でよい（頂点＋フラグメント）。色指定はユニフォームで渡す。  
- リサイズ時は `glViewport` を更新する。

---

## 評価観点
- 起動時に `READY` が出力されるか  
- 指定コマンドがすべて仕様通りに動作するか  
- 不正入力に対して `ERR ...` を返し、落ちないか  
- リソースが正しく解放され、終了時に `BYE` が出力されるか

---

## 発展課題（任意）
- 1 秒ごとに自動で `FPS <number>` を出力する機能  
- 色補間（`color` コマンド時に滑らかに遷移）  
- `shape circle` の追加  
- コマンドパーサを JUnit で単体テスト  
- `--headless` モードでオフスクリーン描画とベンチ出力

---