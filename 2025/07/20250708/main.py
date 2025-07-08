import os
from datetime import datetime

ACCESS_LOG = "access.log"
OUTPUT_LOG = "output.txt"

class FileOperations():
    def __init__(self, filename=OUTPUT_LOG):
        self.file = open(filename, "w")
    
    def add_line(self, line):
        self.file.write(line + "\n")

    def __del__(self):
        self.file.close()


class AccessLogDatesCounter():
    def __init__(self ,filename=ACCESS_LOG):
        self.date_counts = {} 
        with open(filename, "r") as file:
            self.logs = file.readlines()
        for log in self.logs:
                # ログの先頭から日付部分（YYYY-MM-DD）だけを抽出
                date_str = log[:10]
                try:
                    current_date = datetime.strptime(date_str, "%Y-%m-%d").date()
                    self.date_counts[current_date] = self.date_counts.get(current_date, 0) + 1
                except ValueError:
                    print(f"Invalid date format in log: {log.strip()}")
        file.close()

    
    def sort_date_counts_by_count(self):
        return dict(sorted(self.date_counts.items(), key=lambda item: item[1], reverse=True))
    
    # Accessors
    def get_raw_logs(self):
        return self.logs
    def get_date_counts(self):
        return self.date_counts


def main():
    logCounter = AccessLogDatesCounter()
    # # debug
    # print(logCounter.get_raw_logs())
    #print(logCounter.get_date_counts())
    sorted_counts = logCounter.sort_date_counts_by_count()
    output = FileOperations(OUTPUT_LOG)
    for date, count in sorted_counts.items():
        output.add_line(f"{date} {count}")
    
    del logCounter
    del output

    

if __name__ == "__main__":
    main()
