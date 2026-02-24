# main.py
import sys
import os
import tempfile

LOG_FILE = "kvs.log"
class LogStructuredKV:

    def __init__(self):
        self.index = {}
        self.load()

    @staticmethod
    def _value_offset_from_record(payload_offset: int, key: str) -> int:

        return payload_offset + 4 + len(key) + 1

    def load(self):
        if not os.path.exists(LOG_FILE):
            return

        self.index.clear()

        with open(LOG_FILE, "rb") as f:
            while True:
                record_start = f.tell()

                length_bytes = f.read(4)
                if not length_bytes:
                    break

                length = int.from_bytes(length_bytes, "big")
                payload_offset = record_start + 4

                record_bytes = f.read(length)
                if len(record_bytes) != length:
                    break

                record = record_bytes.decode("utf-8")
                parts = record.split(" ", 2)

                op = parts[0]
                key = parts[1] if len(parts) >= 2 else ""

                if op == "PUT":
                    if len(parts) < 3:
                        continue
                    value = parts[2]

                    value_offset = self._value_offset_from_record(payload_offset, key)
                    self.index[key] = (value_offset, len(value))

                elif op == "DEL":
                    self.index.pop(key, None)

                else:
                    continue

    def _append_log(self, record: str) -> int:
        record_bytes = record.encode("utf-8")
        length = len(record_bytes)

        start_offset = self._get_end_offset()

        with open(LOG_FILE, "ab") as f:
            f.write(length.to_bytes(4, "big"))
            f.write(record_bytes)

        return start_offset

    def put(self, key: str, value: str):
        record = f"PUT {key} {value}"

        record_start = self._append_log(record)
        payload_offset = record_start + 4
        value_offset = self._value_offset_from_record(payload_offset, key)

        self.index[key] = (value_offset, len(value))

    def get(self, key: str) -> str:
        if key not in self.index:
            return "NOT_FOUND"

        offset, length = self.index[key]
        with open(LOG_FILE, "rb") as f:
            f.seek(offset)
            value = f.read(length).decode("utf-8", errors="strict")

        return value

    def delete(self, key: str):
        if key in self.index:
            record = f"DEL {key}"
            self._append_log(record)
            self.index.pop(key, None)

    def count(self) -> int:
        return len(self.index)

    def compact(self):
        temp_fd, temp_path = tempfile.mkstemp(suffix=".log")
        os.close(temp_fd)

        new_index = {}

        with open(LOG_FILE, "rb") as old_f, open(temp_path, "ab") as new_f:
            for key, (old_value_offset, value_length) in self.index.items():
                old_f.seek(old_value_offset)
                value = old_f.read(value_length).decode("utf-8")

                record = f"PUT {key} {value}"
                record_bytes = record.encode("utf-8")
                length = len(record_bytes)

                new_record_start = new_f.tell()
                new_f.write(length.to_bytes(4, "big"))
                new_f.write(record_bytes)

                payload_offset = new_record_start + 4
                value_offset = self._value_offset_from_record(payload_offset, key)
                new_index[key] = (value_offset, len(value))

        os.replace(temp_path, LOG_FILE)
        self.index = new_index

    def _get_end_offset(self) -> int:
        if not os.path.exists(LOG_FILE):
            return 0
        return os.path.getsize(LOG_FILE)


def main():
    kvs = LogStructuredKV()

    for line in sys.stdin:
        line = line.strip()
        if not line:
            continue

        parts = line.split()
        cmd = parts[0]

        if cmd == "PUT":
            key, value = parts[1], parts[2]
            kvs.put(key, value)

        elif cmd == "GET":
            key = parts[1]
            print(kvs.get(key))

        elif cmd == "DEL":
            key = parts[1]
            kvs.delete(key)

        elif cmd == "COUNT":
            print(kvs.count())

        elif cmd == "COMPACT":
            kvs.compact()
            print("OK")


if __name__ == "__main__":
    main()