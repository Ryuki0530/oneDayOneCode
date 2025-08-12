class Csv:
    def __init__(self, file_path):
        self.file_path = file_path

    def read(self):
        with open(self.file_path, 'r') as file:
            return file.readlines()

    def write(self, data):
        with open(self.file_path, 'w') as file:
            file.writelines(data)

    def get_columns(self):
        with open(self.file_path, 'r') as file:
            header = file.readline().strip()
            return header.split(',')
        
    def get_values_by_column(self, column_name):
        with open(self.file_path, 'r') as file:
            header = file.readline().strip().split(',')
            index = header.index(column_name)
            return [line.strip().split(',')[index] for line in file.readlines()]
        
class Main:
    def __init__(self, csv_file):
        self.csv_file = Csv(csv_file)
        self.data = self.csv_file.read()

    def run(self):
        print("CSV File Data:")
        print(self.data)
        self.main_loop()

    def main_loop(self):
        while True:
            command = input("Enter command (read/write/columns/values/exit): ").strip().lower()
            if command == 'read':
                print(self.data)
            elif command == 'write':
                new_data = input("Enter data to write (comma-separated): ")
                self.csv_file.write([new_data + '\n'])
                print("Data written successfully.")
            elif command == 'columns':
                columns = self.csv_file.get_columns()
                print("Columns:", columns)
            elif command == 'values':
                column_name = input("Enter column name: ")
                values = self.csv_file.get_values_by_column(column_name)
                print(f"Values in '{column_name}':", values)
            elif command == 'exit':
                break
            else:
                print("Unknown command. Please try again.")

    