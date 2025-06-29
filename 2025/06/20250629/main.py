import os
import datetime
import re

#define files path
LOG_FILE = "log.txt"
ERROR_OUTPUT_FILE = "error_output.txt"
WARNING_OUTPUT_FILE = "warning_output.txt"

#prototype definitions
def error_counter(log_file_path, error_log_dump=False):
    pass
def warning_counter(log_file_path, warning_log_dump=False):
    pass
# use error_counter and warning_counter. load the log file
def logfile_anal(log_file_path, error_log_dump=False, warning_log_dump=False):
    pass
# main function use logfile_anal
def main():
    pass

def main():
    logfile_anal(
        LOG_FILE,
        error_output_file_path=ERROR_OUTPUT_FILE,
        warning_output_file_path=WARNING_OUTPUT_FILE,
        error_anal=True,
        warning_anal=True,
    )  

def error_counter(string_log, error_log_dump=False):
    error_count = 0
    lines = string_log.splitlines()
    output_lines = []

    for line in lines:
        match = re.match(r"\[(.*?)\] ERROR: (.+)", line)
        if match:
            timestamp, message = match.groups()
            formatted = f"{timestamp} - {message}"
            output_lines.append(formatted)
            error_count += 1
            if error_log_dump:
                print(formatted)

    return error_count, output_lines

def warning_counter(string_log, warning_log_dump=False):
    warning_count = 0
    lines = string_log.splitlines()
    output_lines = []

    for line in lines:
        match = re.match(r"\[(.*?)\] WARNING: (.+)", line)
        if match:
            timestamp, message = match.groups()
            formatted = f"{timestamp} - {message}"
            output_lines.append(formatted)
            warning_count += 1
            if warning_log_dump:
                print(formatted)

    return warning_count, output_lines

def write_string_to_file(file_path, content):
    with open(file_path, 'w', encoding='utf-8') as f:
        f.write(content)

def logfile_anal(log_file_path, error_output_file_path=None, warning_output_file_path=None, error_anal=False, warning_anal=False):
    error_outputs = ""
    warning_outputs = ""

    if not os.path.exists(log_file_path):
        print(f"[ERROR] Log file {log_file_path} does not exist.")
        return -1
    with open(log_file_path, 'r') as file:
        string_log = file.read()

    if error_anal:
        error_count, error_lines = error_counter(string_log)
        error_outputs += f"Error lines:\n" + "\n".join(error_lines) + "\n\n"
        error_outputs += f"Total errors found: {error_count}\n"
        if error_output_file_path:
            write_string_to_file(error_output_file_path, error_outputs)
        else:
            print(error_outputs)

    if warning_anal:
        warning_count, warning_lines = warning_counter(string_log)
        warning_outputs += f"Warning lines:\n" + "\n".join(warning_lines) + "\n\n"
        warning_outputs += f"Total warnings found: {warning_count}\n"
        if warning_output_file_path:
            write_string_to_file(warning_output_file_path, warning_outputs)
        else:
            print(warning_outputs)


if __name__ == "__main__":
    main()