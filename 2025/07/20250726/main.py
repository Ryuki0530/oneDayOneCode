import mysql.connector
class Customer:
    def __init__(self, name, email, age):
        self.name = name
        self.email = email
        self.age = age

    def __str__(self):
        return f"Customer(name={self.name}, email={self.email}, age={self.age})"
    
class DatabaseManager:
    def __init__(self, host, user, password, database):
        self.conn = mysql.connector.connect(
            host=host,
            user=user,
            password=password,
            database=database
        )
        self.cursor = self.conn.cursor()

    def insert_customer(self, table, customer):
        sql = f"INSERT INTO {table} (name, email, age) VALUES (%s, %s, %s)"
        values = (customer.name, customer.email, customer.age)
        self.cursor.execute(sql, values)
        self.conn.commit()

    def close(self):
        self.cursor.close()
        self.conn.close()

    def search_customer_by_age(self, table, age):
        sql = f"SELECT * FROM {table} WHERE age = %s"
        self.cursor.execute(sql, (age,))
        return self.cursor.fetchall()
    
    def delete_customer(self, table, customer_id):
        sql = f"DELETE FROM {table} WHERE id = %s"
        self.cursor.execute(sql, (customer_id,))
        self.conn.commit()
    
    def view_all_customers(self, table):
        self.cursor.execute(f"SELECT * FROM {table}")
        return self.cursor.fetchall()
    

class Main:

    def main():
        print("modes")
        print("1. Insert Customer")
        print("2. View all customers")
        print("3. Search Customer by Age")
        print("4. Delete Customer")
        print("5. Exit")
        print("Select Mode :\n")
        db_manager = DatabaseManager('localhost','root','', 'store')
        

        while True:
            mode = input("Select Mode: ")
            if mode == '1':
                name = input("Enter customer name: ")
                email = input("Enter customer email: ")
                age = int(input("Enter customer age: "))
                customer = Customer(name, email, age)
                db_manager.insert_customer('customers', customer)
                print(f"Inserted: {customer}")
            elif mode == '2':
                customers = db_manager.view_all_customers('customers')
                for cust in customers:
                    print(cust)
            elif mode == '3':
                age = int(input("Enter age to search: "))
                customers = db_manager.search_customer_by_age('customers', age)
                for cust in customers:
                    print(cust)
            elif mode == '4':
                customer_id = int(input("Enter customer ID to delete: "))
                db_manager.delete_customer('customers', customer_id)
                print(f"Deleted customer with ID: {customer_id}")
            elif mode == '5':
                print("Exiting...")
                db_manager.close()
                break
            else:
                print("Invalid mode selected. Please try again.")

if __name__ == "__main__":
    Main.main()