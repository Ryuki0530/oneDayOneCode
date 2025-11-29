# Java CLI Recording Tool

あなたの課題は、Java で **音声録音機能を持つ CLI ソフト**を作成することである。

## 🔧 機能要件

1. **起動時に録音デバイスを列挙**
   - Java Sound API を使い、`TargetDataLine` として利用可能なデバイスを取得する。
   - 使用するデバイスをユーザに選択させる。

2. **待機モード**
   - デバイス選択後、「待機モード」に入る。
   - キー入力を監視し、以下の動作を行う。

3. **録音開始 (Rキー)**
   - `R` が押されると録音を開始。
   - シンプルな PCM (44.1kHz, 16bit, mono) で録音する。

4. **録音終了＆保存 (Sキー)**
   - `S` が押されると録音を停止。
   - WAV 形式で保存する。
   - 保存ファイル名は `record_開始時刻_終了時刻.wav` とする。
   - 保存先は **実行ディレクトリ**。

5. **終了**
   - 録音終了後はプログラムを終了してよい。

---

## 📦 技術制約

- 外部ライブラリ禁止（Java Sound API のみ）
- プロジェクトは Gradle で構築すること
- Windows / Linux で動作する必要がある

---

## 🎯 完成イメージ

```
=== Java CLI Recorder ===
[0] Microphone (Realtek)  
[1] USB Audio Device  
Use device number: 0

Waiting mode... (R=Record, S=Save & Exit)
> R
Recording...
> S
Saved: record_20251129_20251129.wav
```

---

## 🧪 テスト項目

- デバイス一覧が正しく取得できるか
- 録音開始／停止が 1 回の実行で動作するか
- WAV ファイルが正常に再生できるか
- 実行ディレクトリに書き込まれるか

