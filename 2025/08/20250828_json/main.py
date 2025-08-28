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

class Book_Table_Analyzer:
    def __init__(self, book_table: 'Book_Table'):
        self.book_table = book_table

    def total_stack(self):
        sum = 0
        for record in self.book_table.records:
            sum += 1
        return sum

    def total_price(self):
        sum = 0
        for record in self.book_table.records:
            sum += record.price
        return sum

    def authors_info(self):
        author_stats = {}
        for record in self.book_table.records:
            author = record.author
            if author not in author_stats:
                author_stats[author] = {"count": 0, "total_price": 0}
            author_stats[author]["count"] += 1
            author_stats[author]["total_price"] += record.price

        result = {}
        for author, stats in author_stats.items():
            avg_price = stats["total_price"] / stats["count"] if stats["count"] > 0 else 0
            result[author] = {"count": stats["count"], "average_price": avg_price}
        return result

    def sort_by_author(self):
        self.book_table.records.sort(key=lambda x: x.author)

    def sort_by_year_desc(self):
        self.book_table.records.sort(key=lambda x: x.year, reverse=True)


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
        