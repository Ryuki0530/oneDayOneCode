import pandas as pd
import matplotlib.pyplot as plt
import sys

def plot_data(data):
    # 日本語フォントを明示的に指定（Windowsの場合）
    plt.rcParams['font.family'] = 'Meiryo'
    plt.figure(figsize=(10, 5))
    plt.plot(data['date'], data['sales'], marker='o')
    plt.title("日別売上推移")
    plt.xlabel("日付")
    plt.ylabel("売上金額")
    plt.xticks(rotation=45)
    plt.grid()
    plt.tight_layout()
    plt.gcf().autofmt_xdate()  # x軸ラベルの重なりを自動調整
    plt.show()

def load_data(file_path):
    try:
        df = pd.read_csv(file_path)
    except Exception as e:
        print(f"Error loading data: {e}")
        return 1, pd.DataFrame()
    df['sales'] = df['price'] * df['quantity']
    return 0, df

def main():
    error, data = load_data("sales.csv")
    if error:
        print("データの読み込みに失敗しました。プログラムを終了します。")
        sys.exit(1)
    daily_sales = data.groupby('date')['sales'].sum().reset_index()
    print("[日別売上]")
    for index, row in daily_sales.iterrows():
        print(f"{row['date']}: {row['sales']}円")
    print("[書籍別累計販売冊数]")
    book_sales = data.groupby('title')['quantity'].sum().reset_index()
    for index, row in book_sales.iterrows():
        print(f"{row['title']}: {row['quantity']}冊")
    plot_data(daily_sales)

if __name__ == "__main__":
    main()