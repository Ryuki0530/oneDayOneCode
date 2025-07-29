# main.py

import sys
import os
from web_connector import web_connector
from html_analizer import html_analizer

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
        self.analizer = html_analizer("", self.debug)


    def main(self):
        if self.url:
            print(f"URL provided: {self.url}")
            self.connector.connect()
            if self.debug:
                self.connector.dump_response()
            response = self.connector.get_response()
            if response:
                print(f"Response status code: {response.status_code}")
                
                self.analizer.set_html(response.text)
                tag_counts = self.analizer.tag_count()
                tag_hierarchy = self.analizer.tag_hierarchy()

                print("\n\n[Tag counts]")
                for tag, count in tag_counts.items():
                    print(f"{tag}: {count}")

                print("\n\n[Tag hierarchy]")
                prev_level = 0
                for level, tag_name in tag_hierarchy:
                    print(f"{'  ' * level}{tag_name}")

        else:
            print("No URL provided.")
            return 1
        return 0


if __name__ == "__main__":
    main_instance = main()
    exitcode = main_instance.main()
    sys.exit(exitcode)
