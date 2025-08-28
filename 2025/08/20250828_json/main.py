import json

class Book_Record:
    def __init__(self, title, author, price, year):
        self.title = title
        self.author = author
        self.price = price
        self.year = year

    def dump(self):
        print(f"Title: {self.title}, Author: {self.author}, Price: {self.price}, Year: {self.year}")

class Book_Table:
    def __init__(self, file_path):
        self.records = []
        self.load_from_file(file_path)

    def add_record(self, title, author, price, year):
        record = Book_Record(title, author, price, year)
        self.records.append(record)
        return 0
    # overload
    def add_record(self, record: Book_Record):
        self.records.append(record)
        return 0

    def dump(self):
        for record in self.records:
            record.dump()

class Json_Loader:
    def __init__(self, file_path: str, Book_Table: 'Book_Table'):
        self.file_path: str = file_path
        self.Book_Table: 'Book_Table' = Book_Table

    def load(self):
        raw_data = self._open_file()
        if raw_data:
            data = self._parse_records_json(raw_data)
        else:
            print("No data to load.")
            return 1
        
        if data:
            for record in data:
                self.Book_Table.add_record(*record)
            return 0

    def _open_file(self):
        try:
            return open(self.file_path, 'r', encoding='utf-8').read()
        except FileNotFoundError:
            print(f"File not found: {self.file_path}")
            return None
        
    def _parse_records_json(self, raw_data: str):
        try:
            data = json.loads(raw_data)
            records_array = []
            for item in data:
                record = [
                    item.get("title", ""),
                    item.get("author", ""),
                    item.get("price", 0),
                    item.get("year", 0)
                ]
                records_array.append(record)
            return records_array
        except json.JSONDecodeError as e:
            print(f"JSON decode error: {e}")
            return []