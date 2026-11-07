# MergeSettings

使用言語: Python
目安: 15〜20分

## 目的
入れ子になった辞書を扱い、デフォルト設定と追加設定を統合する関数を作成してください。

## 課題
merge_settings(base, override) を実装してください。
base に override を上書きした、新しい辞書を返します。

## 要件
1. 片方にしかないキーは、そのまま結果に含めてください。
2. 同じキーの値が両方とも辞書なら、その中身も再帰的に統合してください。
3. それ以外は override の値を採用してください。
   リストは結合せず、丸ごと置き換えます。
4. 元の辞書を変更しないでください。また、結果内の辞書やリストを変更しても、元の入力に影響しないようにしてください。
5. 値は文字列・数値・真偽値・None・リスト・辞書のみとします。
   標準ライブラリの copy.deepcopy を使用して構いません。

## 入力例
base = {
    "theme": "light",
    "window": {"width": 800, "height": 600},
    "plugins": ["spellcheck"],
    "autosave": True
}

override = {
    "theme": "dark",
    "window": {"width": 1200},
    "plugins": ["formatter"],
    "timeout": None
}

result = merge_settings(base, override)
print(result)

## 期待出力
{
    "theme": "dark",
    "window": {"width": 1200, "height": 600},
    "plugins": ["formatter"],
    "autosave": True,
    "timeout": None
}

※ 改行やキーの表示順は問いません。

## 完了条件
- 入力例の結果が正しいこと。
- 空の辞書同士なら {} を返すこと。
- 辞書の値を数値で上書きする場合も動くこと。
- result["window"]["height"] や result["plugins"] を変更しても、base と override が変化しないこと。