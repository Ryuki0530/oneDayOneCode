import string
import math

DEBUG = False
common_words = ["the", "and", "is", "in", "it", "of", "to", "a", "that"]

def decode_caesar(cipher_text: str) -> str:
    
    best_shift = 0
    best_score = -math.inf

    #最良の探索
    input_all_low = cipher_text.lower()
    for n in range (1,26):
        shifted = _shift_string(input_all_low, n)
        eve = _eval_string(shifted)
        if DEBUG : print(shifted ," : ",eve)
        if eve > best_score:
            best_score = eve
            best_shift = n

    best_decoded = _shift_string(cipher_text, best_shift)
    return best_decoded

def _eval_string(str : str ) -> int:
    words = str.split()
    count = sum(1 for word in words if word in common_words)
    return count

def _shift_string(str : str, n:int ) ->str:
    out = []
    for c in str:
        out.append(_shift_char(c, n))
    return ''.join(out)


def _shift_char(c: str, n: int) -> str:
    if 'a' <= c <= 'z':
        return chr((ord(c) - ord('a') - n) % 26 + ord('a'))
    if 'A' <= c <= 'Z':
        return chr((ord(c) - ord('A') - n) % 26 + ord('A'))
    return c



def main():
    data = input("暗号文を入力してください: ")
    print(decode_caesar(data))


if __name__ == "__main__":
    main()
    