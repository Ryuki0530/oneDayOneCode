import os
import sys
import tkinter as tk
import argparse
import openpyxl

from tkinter import filedialog, ttk


class ExcelFileProcessor:
    def __init__(self, debug=False):
        self.file_path = None
        self.debug = debug
        self.workbook = None

    def load(self, file_path):
        self.workbook = openpyxl.load_workbook(file_path)
        # Simulate reading an Excel file
        if self.debug:
            print(f"Debug mode is ON. Reading file: {file_path}")

    def get_workbook(self):
        # Simulate getting sheet names from an Excel file
        if self.debug:
            print(f"Debug mode is ON. Getting sheet names from: {self.file_path}")
        return self.workbook
    


class gui:
    def __init__(self, root, gui_root, debug=False):
        self.root = root
        self.gui_root = gui_root
        self.gui_root.title("Excel Viewer")
        self.debug = debug

        # 「ファイルを開く」ボタン
        self.button = tk.Button(gui_root, text="ファイルを開く", command=self.root.open_file)
        self.button.pack(pady=10)

        # Treeview（表の部分）
        self.tree = ttk.Treeview(gui_root)
        self.tree.pack(expand=True, fill="both")

    def display_sheet(self, sheet=None):
        if not sheet:
            return
        
        self.tree.delete(*self.tree.get_children())
        self.tree["columns"] = ()

        headers = []

        for idx, cell in enumerate(sheet[1]):
            header = cell.value if cell.value is not None else f"Column{idx+1}"
            headers.append(header)

        # Treeviewの設定
        self.tree["columns"] = headers
        self.tree["show"] = "headings"  # 行番号を表示しない

        for header in headers:
            self.tree.heading(header, text=header)
            self.tree.column(header, width=100, anchor="center")

        # データの追加（2行目以降）
        for row in sheet.iter_rows(min_row=2, values_only=True):
            self.tree.insert("", "end", values=row)

    def fileSelector(self):
        # Simulate file selection
        print("Opening file selector")
        file_path = filedialog.askopenfilename(title="ファイルを選択してください")
        if file_path:
            if self.debug:
                print(f"選択されたファイル: {file_path}")
            return file_path
        else:
            return None


class main:
    def __init__(self, debug=False):
        parser = argparse.ArgumentParser(description="Excel File Processor")
        parser.add_argument("--debug", action="store_true", help="Enable debug mode")
        args = parser.parse_args()
        self.working_book = None
        self.working_sheet = None
        self.debug = args.debug
        self.excel_processor = ExcelFileProcessor(debug=self.debug)
        gui_root = tk.Tk()
        self.gui_instance = gui(self, gui_root=gui_root, debug=self.debug)
        
        gui_root.mainloop()

    def open_file(self):
        file_path = self.gui_instance.fileSelector()
        if not file_path:
            print("No file selected.")
            sys.exit(1)
        _, ext = os.path.splitext(file_path)
        if ext.lower() != ".xlsx":
            print("サポートされているファイル形式は .xlsx のみです。")
            sys.exit(1)
        if self.debug:
            print(f"Opening file: {file_path}")
        self.working_book = self.excel_processor.load(file_path)
        self.working_book = self.excel_processor.get_workbook()

        self.view_sheet()

        if self.debug:
            print(f"Workbook loaded with sheets: {self.working_book}")
        
    
    
    def view_sheet(self):
        if not self.working_book:
            print("No workbook loaded.")
            exit(1)
        sheet = self.working_book.active
        if self.debug:
            print(f"Viewing sheet: {sheet.title}")
        self.gui_instance.display_sheet(sheet)


if __name__ == "__main__":
    main = main()