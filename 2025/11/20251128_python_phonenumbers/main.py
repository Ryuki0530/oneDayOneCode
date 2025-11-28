import phonenumbers
from phonenumbers import geocoder, carrier, PhoneNumberFormat
import json
import argparse
import sys

def extract_numbers(text, default_region="JP"):
    matcher = phonenumbers.PhoneNumberMatcher(text, default_region)
    for match in matcher:
        yield match.raw_string, match.number


def build_info_dict(raw, number):
    info = {}
    info["raw"] = raw
    info["e164"] = phonenumbers.format_number(number, PhoneNumberFormat.E164)
    info["valid"] = phonenumbers.is_valid_number(number)
    info["region"] = geocoder.description_for_number(number, "en") or "N/A"
    info["carrier"] = carrier.name_for_number(number, "en") or "N/A"
    return info

def main():
    parser = argparse.ArgumentParser(description="Phone Number Inspector")
    print("Input text:")
    text = sys.stdin.readline().strip()
    results = []
    for raw, number in extract_numbers(text, default_region="JP"):
        info = build_info_dict(raw, number)
        results.append(info)
    print(f"[Raw] {info['raw']}")
    print(f"[E164] {info['e164']}")
    print(f"[Region] {info['region']}")
    print(f"[Carrier] {info['carrier']}")
    print(f"[Valid] {info['valid']}")
    print()


if __name__ == "__main__":
    main()
