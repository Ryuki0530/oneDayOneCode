# translate.py
import sys
import requests

def main():
    text = sys.argv[1]
    deepl_api_key = sys.argv[2]
    endpoint = "https://api-free.deepl.com/v2/translate"

    res = requests.post(endpoint, data={
        "auth_key": deepl_api_key,
        "text": text,
        "source_lang": "EN",
        "target_lang": "JA"
    })

    try:
        translated = res.json()["translations"][0]["text"]
        print(translated)
    except Exception as e:
        print("[ERROR] Translation failed:", e, file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()
