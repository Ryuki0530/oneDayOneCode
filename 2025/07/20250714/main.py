import csv

class CSVManager:
    def __init__(self, file_path):
        self.file_path = file_path
        self.headers = []
        self.records = []
        self.load()

    def load(self):
        with open(self.file_path, mode='r', encoding='utf-8') as f:
            reader = csv.DictReader(f)
            self.headers = reader.fieldnames
            self.records = [row for row in reader]

    def get_all(self):
        return self.records
    
    def search(self, column, value):
        return [row for row in self.records if value in row.get(column, '')]

def main():

    csv_manager = CSVManager('books.csv')

    # # Debug
    # print(csv_manager.get_all())

    print("著者の名前から検索: ", end='')
    keyword = input()
    results = csv_manager.search('author', keyword)
    if not results:
        print("該当する書籍が見つかりませんでした。")
        return
    print("[検索結果]")
    for book in results:
        print(f"ID: {book['id']}")
        print(f"書名: {book['title']}")
        print(f"著者: {book['author']}")
        print(f"出版年: {book['year']}", end='\n\n')


if __name__ == "__main__":
    main()