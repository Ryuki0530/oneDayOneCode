import sys

class AccessCounter:
    def __init__(self):
        self.logs = []
        self.user_count = {}
        self.action_count = {}
    
    def read_logs(self):
        for line in sys.stdin:
            parts = line.strip().split()
            if len(parts) == 2:
                user, action = parts
                self.logs.append((user, action))
    
    def count(self):
        for user, action in self.logs:
            self.user_count[user] = self.user_count.get(user, 0) + 1
            self.action_count[action] = self.action_count.get(action, 0) + 1
    
    def print_results(self):
        total = len(self.logs)
        print(f"TOTAL {total}")
        
        print("USERS")
        for user in sorted(self.user_count.keys()):
            print(f"{user} {self.user_count[user]}")
        
        print("ACTIONS")
        for action in sorted(self.action_count.keys()):
            print(f"{action} {self.action_count[action]}")
        
        top_user = min(self.user_count.items(), key=lambda x: (-x[1], x[0]))
        top_action = min(self.action_count.items(), key=lambda x: (-x[1], x[0]))
        
        print(f"TOP_USER {top_user[0]} {top_user[1]}")
        print(f"TOP_ACTION {top_action[0]} {top_action[1]}")

def main():
    access_counter = AccessCounter()
    access_counter.read_logs()
    access_counter.count()
    access_counter.print_results()

if __name__ == "__main__":
    main()