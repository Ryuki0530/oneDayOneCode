import argparse
import sys
import os
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import numpy as np

class GraphPlotter:
    def __init__(self, csv_file):
        self.csv = csv_file
        self.df = pd.read_csv(csv_file.file_path)
    
    def plot_histogram(self, column_name):
        """単一列のヒストグラムを描画"""
        try:
            if column_name not in self.df.columns:
                print(f"列 '{column_name}' が見つかりません。")
                return
            
            data = pd.to_numeric(self.df[column_name], errors='coerce').dropna()
            if data.empty:
                print(f"列 '{column_name}' に数値データがありません。")
                return
            
            plt.figure(figsize=(10, 6))
            plt.hist(data, bins=20, alpha=0.7, color='skyblue', edgecolor='black')
            plt.title(f'{column_name} のヒストグラム')
            plt.xlabel(column_name)
            plt.ylabel('頻度')
            plt.grid(True, alpha=0.3)
            plt.show()
            
        except Exception as e:
            print(f"ヒストグラム描画エラー: {e}")
    
    def plot_scatter(self, x_column, y_column):
        """2列の散布図を描画"""
        try:
            if x_column not in self.df.columns or y_column not in self.df.columns:
                print(f"指定された列が見つかりません。")
                return
            
            x_data = pd.to_numeric(self.df[x_column], errors='coerce')
            y_data = pd.to_numeric(self.df[y_column], errors='coerce')
            
            # NaNを含む行を除去
            valid_data = pd.DataFrame({x_column: x_data, y_column: y_data}).dropna()
            
            if valid_data.empty:
                print("数値データが不十分です。")
                return
            
            plt.figure(figsize=(10, 6))
            plt.scatter(valid_data[x_column], valid_data[y_column], alpha=0.6, color='coral')
            plt.title(f'{x_column} vs {y_column} の散布図')
            plt.xlabel(x_column)
            plt.ylabel(y_column)
            plt.grid(True, alpha=0.3)
            plt.show()
            
        except Exception as e:
            print(f"散布図描画エラー: {e}")
    
    def plot_correlation_heatmap(self):
        """相関行列のヒートマップを描画"""
        try:
            # 数値列のみを選択
            numeric_df = self.df.select_dtypes(include=[np.number])
            
            if numeric_df.empty:
                print("数値データが見つかりません。")
                return
            
            correlation_matrix = numeric_df.corr()
            
            plt.figure(figsize=(12, 8))
            sns.heatmap(correlation_matrix, annot=True, cmap='coolwarm', center=0,
                       square=True, linewidths=0.5)
            plt.title('相関行列のヒートマップ')
            plt.tight_layout()
            plt.show()
            
        except Exception as e:
            print(f"ヒートマップ描画エラー: {e}")
    
    def get_numeric_columns(self):
        """数値列の一覧を取得"""
        return self.df.select_dtypes(include=[np.number]).columns.tolist()
    
    def show_columns(self):
        """利用可能な列を表示"""
        print("\n利用可能な列:")
        for i, col in enumerate(self.df.columns, 1):
            print(f"{i}. {col}")

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
        parser = argparse.ArgumentParser(description="CSV File Utility")
        parser.add_argument('--debug', action='store_true', help='Enable debug mode')
        parser.add_argument('input', help='CSV file path')
        if len(sys.argv) == 1:
            parser.print_help()
            sys.exit(1)
        args = parser.parse_args()
        if args.debug:
            print("Debug mode is enabled.")
            self.debug = True
        else:
            self.debug = False
        self.csv_file = Csv(csv_file)
        self.data = self.csv_file.read()
        self.plotter = GraphPlotter(self.csv_file)

    def run(self):
        print("CSV File Data:")
        print(self.data)
        self.main_loop()

    def main_loop(self):
        while True:
            print("\nメニュー:")
            print("1. 単一列のヒストグラム")
            print("2. 2列の散布図")
            print("3. 相関行列のヒートマップ")
            print("4. 利用可能な列を表示")
            print("5. 終了")
            choice = input("番号を選択してください: ").strip()
            
            if choice == '1':
                self.plotter.show_columns()
                column = input("ヒストグラムを描画する列名を入力してください: ").strip()
                self.plotter.plot_histogram(column)
            elif choice == '2':
                self.plotter.show_columns()
                x_col = input("X軸の列名を入力してください: ").strip()
                y_col = input("Y軸の列名を入力してください: ").strip()
                self.plotter.plot_scatter(x_col, y_col)
            elif choice == '3':
                self.plotter.plot_correlation_heatmap()
            elif choice == '4':
                self.plotter.show_columns()
            elif choice == '5':
                break
            else:
                print("無効な選択です。もう一度入力してください。")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python main.py <csv_file_path> [--debug]")
        sys.exit(1)
    csv_file_path = sys.argv[1]
    main_app = Main(csv_file_path)
    main_app.run()