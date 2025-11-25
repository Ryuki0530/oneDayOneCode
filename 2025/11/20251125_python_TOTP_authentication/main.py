import json
import os
import sys
from pathlib import Path
import pyotp
import qrcode
import gc

class TOTPAuth:

    issuer = "OneDayOneCode"

    def __init__(self, key_data_path="users_key.json"):
        self.key_data_path = key_data_path
        self.users = self._load_users()

    def _save_users(self):
        with open(self.key_data_path, 'w') as f:
            json.dump(self.users, f)
    
    def _load_users(self):
        try:
            if os.path.exists(self.key_data_path):
                with open(self.key_data_path, 'r') as f:
                    return json.load(f)
        except Exception as e:
            print(f"Error loading users: {e}")
        return {}
    
    def _user_name_check(self, username):
        if not username.isalnum():
            return 1,"Username must be alphanumeric."
        if len(username) < 3 or len(username) > 20:
            return 1,"Username must be between 3 and 20 characters."
        if ' ' in username:
            return 1,"Username must not contain spaces."
        return 0,"OK"


    def register_user(self, username):
        
        error_check = self._user_name_check(username)
        if error_check[0] != 0:
            return error_check
        
        key = pyotp.random_base32()
        self.users[username] = key
        self._save_users()
        self._load_users()
        print(f"User '{username}' registered.")
        print(f"Secret: {key}")
        issuer = self.issuer
        otpauth = pyotp.totp.TOTP(key).provisioning_uri(name=username, issuer_name=issuer)
        print("Scan this URL in Google Authenticator:")
        print(otpauth)

        img = qrcode.make(otpauth)
        img_path = Path(f"{username}_qrcode.png")
        img.save(img_path)
        print(f"QR code saved to {img_path}")

        return 0,"OK"

    def login_user(self, username, totp_code):
        if username not in self.users:
            return 1,"User does not exist."
        
        key = self.users[username]
        totp = pyotp.TOTP(key)
        if totp.verify(totp_code):
            print("Login success!")
            return 0,"OK"
        else:
            print("Invalid TOTP code.")
            return -1,"Invalid TOTP code."
        
    def shutdown(self):
        self._save_users()
        self.users.clear()
        return 0,"OK"


class CLI:
    @staticmethod
    def regist_user(auth: TOTPAuth):
        user_name = input("Enter username: ")
        error_check = auth.register_user(user_name)

        if error_check[0] != 0:
            print(error_check[1])
        return error_check[0]

    @staticmethod
    def login_user(Auth : TOTPAuth):
        user_name = input("Enter username: ")
        totp_code = input("Enter TOTP: ")
        error_check = Auth.login_user(user_name, totp_code)
        if error_check[0] != 0:
            print(error_check[1])
        return error_check[0]

    @staticmethod
    def shutdown(authenticator: TOTPAuth):
        print("Exiting the program.")
        error_check = authenticator.shutdown()
        if error_check[0] != 0:
            print(error_check[1])
        sys.exit(0)

    @staticmethod
    def main():
        authenticator = TOTPAuth()

        #メインループ
        while True:
            mode = 0
            print (
                "1. register\n" \
                "2. login\n" \
                "3. exit"
            )
            mode = input (":")
            if mode == "1":
                CLI.regist_user(authenticator)
            elif mode == "2":
                CLI.login_user(authenticator)
            elif mode == "3":
                CLI.shutdown(authenticator)
            else:
                print("Invalid option. Please try again.")
        return 1

if __name__ == "__main__":
    CLI.main()