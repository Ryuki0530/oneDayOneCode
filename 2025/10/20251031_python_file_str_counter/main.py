# ファイル内の文字統計プログラム
import sys

# コマンドライン引数からファイル名を取得
if len(sys.argv) < 2:
    print("Usage: python main.py <filename>")
    sys.exit(1)

filename = sys.argv[1]

# カウンタの初期化
alphabet_count = 0
digit_count = 0
space_count = 0
other_count = 0

# ファイルを読み込んで文字をカウント
try:
    with open(filename, 'r', encoding='utf-8') as file:
        content = file.read()
        
        for char in content:
            if char.isalpha():
                alphabet_count += 1
            elif char.isdigit():
                digit_count += 1
            elif char.isspace():
                space_count += 1
            else:
                other_count += 1
    
    # 結果を出力
    print(f"Alphabet: {alphabet_count}")
    print(f"Digit: {digit_count}")
    print(f"Space: {space_count}")
    print(f"Other: {other_count}")
    
except FileNotFoundError:
    print(f"Error: File '{filename}' not found.")
except Exception as e:
    print(f"Error: {e}")