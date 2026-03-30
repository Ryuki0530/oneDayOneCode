n = int(input())
inventory = {}

# ログを処理して在庫を集計
for _ in range(n):
    line = input().split()
    item = line[0]
    delta = int(line[1])
    
    if item not in inventory:
        inventory[item] = 0
    inventory[item] += delta

# 在庫が1以上の商品を抽出して辞書順でソート
results = sorted([
    (item, stock) for item, stock in inventory.items() if stock >= 1
])

# 出力
for item, stock in results:
    print(f"{item} {stock}")