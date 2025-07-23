data = """注文者:田中太郎, 商品:リンゴ, 数量:3
注文者:佐藤花子, 商品:バナナ, 数量:2
注文者:鈴木一郎, 商品:リンゴ, 数量:1
注文者:田中太郎, 商品:バナナ, 数量:5
注文者:佐藤花子, 商品:リンゴ, 数量:4"""

class OrderManager:
    def __init__(self, data):
        self.orders = []
        for line in data.strip().split('\n'):
            order = {}
            for item in line.split(', '):
                key, value = item.split(':')
                order[key] = value
            self.orders.append(order)


    def get_orders(self):
        result = {}
        for order in self.orders:
            name = order['注文者']
            product = order['商品']
            quantity = int(order['数量'])
            if name not in result:
                result[name] = {}
            if product not in result[name]:
                result[name][product] = 0
            result[name][product] += quantity
        return result

def main():
    manager = OrderManager(data)
    orders = manager.get_orders()
    for user in orders:
        print(f"{user}:")
        for product in sorted(orders[user]):
            print(f"  {product}: {orders[user][product]}")
    
if __name__ == "__main__":
    main()