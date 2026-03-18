import spacy
from spacy.tokens import Token

# GiNZAモデルをロード
try:
    nlp = spacy.load("ja_ginza")
except OSError:
    print("モデルが見つかりません。`pipを用いてja-ginza`をインストールしてください。")
    exit()

def print_tree(token: Token, level: int = 0):
    """トークンの階層構造を再帰的に表示"""
    indent = "    " * level
    print(f"{indent} {token.text}")
    for child in token.children:
        print_tree(child, level + 1)

def parse_sentence(sentence: str):
    """日本語文を解析し、階層構造を表示"""
    doc = nlp(sentence)
    for sent in doc.sents:
        print(f"Sentence: {sent.text}")
        root = [token for token in sent if token.head == token][0]  # ROOTトークンを取得
        print(f"ROOT: {root.text}")
        for child in root.children:
            print_tree(child)
        print()

def main():
    print("日本語文の階層構造解析プログラム")
    print("終了するには 'exit' を入力してください。\n")
    while True:
        sentence = input("解析する日本語文を入力してください: ")
        if sentence.lower() == "exit":
            print("終了します。")
            break
        try:
            parse_sentence(sentence)
        except Exception as e:
            print(f"解析中にエラーが発生しました: {e}")

if __name__ == "__main__":
    main()