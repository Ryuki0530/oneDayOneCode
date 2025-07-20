import csv

CSV_NAME = 'KEN_ALL.csv'

class CSVManager:
    def __init__(self, file_path):
        self.file_path = file_path
        self.headers = ['code', 'old_zip', 'zip', 'kana_pref', 'kana_city', 'kana_town',
                        'pref', 'city', 'town']  # 必要な範囲に調整
        self.records = []
        self.load()

    def load(self):
        with open(self.file_path, mode='r', encoding='shift_jis') as f:
            reader = csv.DictReader(f, fieldnames=self.headers)
            self.records = [row for row in reader]

    def get_all(self):
        return self.records

    def search_by_all(self, value):
        return [row for row in self.records if any(value in row.get(col, '') for col in self.headers)]

def main():
    csv_manager = CSVManager(CSV_NAME)
    print("検索キーワードを入力してください: ", end='')
    keyword = input()
    results = csv_manager.search_by_all(keyword)
    if not results:
        print("該当するレコードが見つかりませんでした。")
        return

    print("[検索結果]")
    for record in results:
        print(f"郵便番号: {record.get('zip', 'N/A')}")
        print(f"住所: {record.get('pref', '')}{record.get('city', '')}{record.get('town', '')}")
        print("-----")

if __name__ == "__main__":
    main()
