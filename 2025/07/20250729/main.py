import sys
import os
from web_connector import web_connector

class main:
    def __init__(self):
        self.debug = False
        if len(sys.argv) > 1:
            self.url = sys.argv[1]
        else:
            self.url = None

        if (len(sys.argv) > 2):
            if sys.argv[2] == '--debug':
                self.debug = True
                print("Debug mode is ENABLED")

        self.connector = web_connector(self.url, self.debug)

    def main(self):
        if self.url:
            print(f"URL provided: {self.url}")
            self.connector.connect()
            if self.debug:
                self.connector.dump_response()
            response = self.connector.get_response()
            if response:
                print(f"Response status code: {response.status_code}")
                
        else:
            print("No URL provided.")
            return 1
        return 0


if __name__ == "__main__":
    main_instance = main()
    exitcode = main_instance.main()
    sys.exit(exitcode)
