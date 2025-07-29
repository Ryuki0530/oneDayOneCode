import requests

class web_connector:
    
    def __init__(self, url,debug=False):
        self.url = url
        self.debug = debug

    def connect(self):
        if self.url:
            if self.debug:
                print(f"Connecting to {self.url}")
            try:
                self.response = requests.get(self.url)
                self.response.raise_for_status()
            except requests.RequestException as e:
                print(f"Error connecting to {self.url}: {e}")
                return 1
        else:
            print("No URL provided for connection.")
            return 1
    
        return 0
    
    def dump_response(self):
        if hasattr(self, 'response'):
            print(f"Response from {self.url}:")
            print(self.response.text)
        else:
            print("No response to dump. Please connect first.") 
    
    def get_response(self):
        if hasattr(self, 'response'):
            return self.response
        else:
            print("No response available. Please connect first.")
            return None