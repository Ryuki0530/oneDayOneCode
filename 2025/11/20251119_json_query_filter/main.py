import sys
import json


def parse_input_all():
    raw = sys.stdin.read()
    if not raw:
        return None, None
    lines = raw.splitlines()

    idx = len(lines) - 1
    while idx >= 0 and lines[idx].strip() == "":
        idx -= 1
    if idx < 0:
        return None, None
    condition = lines[idx].strip()
    json_str = "\n".join(lines[:idx]).strip()


    if not json_str:
        parts = raw.strip().splitlines()
        if len(parts) >= 2:
            json_str = parts[0].strip()
            condition = parts[1].strip()
    return json_str, condition

def parse_condition(cond):
    if "==" in cond:
        left, right = cond.split("==", 1)
        op = "=="
    elif ">" in cond:
        left, right = cond.split(">", 1)
        op = ">"
    elif "<" in cond:
        left, right = cond.split("<", 1)
        op = "<"
    else:
        raise ValueError("unsupported operator")
    key = left.strip()
    r = right.strip()

    if len(r) >= 2 and r[0] == '"' and r[-1] == '"':
        val = r[1:-1].encode("utf-8").decode("unicode_escape")
        val_type = "str"
    else:
        try:
            val = int(r)
            val_type = "num"
        except Exception:
            try:
                val = float(r)
                val_type = "num"
            except Exception:
                val = r
                val_type = "str"
    return key, op, val, val_type

def match_item(item, key, op, target, target_type):
    if not isinstance(item, dict) or key not in item:
        return False
    v = item[key]
    if op == "==":
        if target_type == "str":
            return isinstance(v, str) and v == target
        else:
            if isinstance(v, (int, float)):
                return float(v) == float(target)
            try:
                return float(v) == float(target)
            except Exception:
                return False
    else: 
        try:
            left = float(v) if not isinstance(v, bool) else float(int(v))
            right = float(target)
        except Exception:
            return False
        if op == ">":
            return left > right
        else:
            return left < right

def main():
    json_str, cond = parse_input_all()
    if not json_str or not cond:
        print("[]")
        return
    try:
        arr = json.loads(json_str)
    except Exception:
        print("[]")
        return
    if not isinstance(arr, list):
        print("[]")
        return
    try:
        key, op, target, target_type = parse_condition(cond)
    except Exception:
        print("[]")
        return
    out = [item for item in arr if match_item(item, key, op, target, target_type)]
    print(json.dumps(out, ensure_ascii=False))

if __name__ == "__main__":
    main()