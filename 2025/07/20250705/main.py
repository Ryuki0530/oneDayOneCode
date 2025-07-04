import os
import sys

INPUT_FILE = 'input.txt'
OUTPUT_FILE = 'output.txt'

class WordCounter:
    def __init__(self):
        self.counter = {}
        
    def count_words_from_str(self, input_data):
        input_data = input_data.lower()
        words = input_data.split()
        words = [word.strip('.,!?') for word in words]
        for word in words:
            if word in self.counter:
                self.counter[word] += 1
            else:
                self.counter[word] = 1
        return 0

    def min_count(self, input_num):
        self.output = {}
        for word, count in self.counter.items():
            if count >= input_num:
                self.output[word] = count
        return self.output
    
    def data_sort(self):
        self.counter = dict(sorted(self.counter.items(), key=lambda item: item[0]))

class IOCtrl:
    def __init__(self,input_file, output_file):
        self.input_file = input_file
        self.output_file = output_file
        self.output_buffer = []

    def read_input(self):
        if not os.path.exists(self.input_file):
            print(f"[ERROR] 入力ファイル '{self.input_file}' が見つかりません。")
            sys.exit(1)
        with open(self.input_file, 'r') as file:
            return file.read().strip()


    def write_output(self, data):
        self.output_buffer.append(data)

    def flush_output(self):
        with open(self.output_file, 'w') as file:
            file.write("\n".join(self.output_buffer))



def main():
    wc = WordCounter()
    io = IOCtrl(INPUT_FILE, OUTPUT_FILE)
    input_data = io.read_input()
    if wc.count_words_from_str(input_data) != 0:
        print("単語のカウントに失敗しました。")
        sys.exit(1)

    input_num = int(input("出力する単語の最小出現回数を入力してください: "))
    wc.data_sort()
    output_data = wc.min_count(input_num)
    for word, count in output_data.items():
        io.write_output(f"{word}: {count}")

    io.flush_output()

if __name__ == "__main__":
    main()