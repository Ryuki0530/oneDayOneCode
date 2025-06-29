import sys
import os
import pandas as pd
import re 

LOG_FILE = "access.log"
OUTPUT_FILE = "os_summary.txt"

user_agent_os_counts = {} 

def main():
    with open(LOG_FILE, "r") as file:
        logs = [line.rstrip('\n') for line in file]
        logs = [line.split() for line in logs]

    for log in logs:
        current_agent = log[2]
        if current_agent not in user_agent_os_counts:
            user_agent_os_counts[current_agent] = 1
        else:
            user_agent_os_counts[current_agent] += 1

    sorted_user_agent_os_counts = dict(
        sorted(user_agent_os_counts.items(), key=lambda x: x[0].lower())
    )

    with open(OUTPUT_FILE, "w") as output_file:
        for user_agent, count in sorted_user_agent_os_counts.items():
            #現在の行の文字列作成
            current_output = user_agent + ": " + str(count) + "\n"
            output_file.write(current_output)
            

    # #Debug
    # print("Debug")
    # print("logs :",logs)
    # print("user_agent_os_counts:",user_agent_os_counts)
    # print("sorted_user_agent_os_counts:",sorted_user_agent_os_counts)

if __name__ == "__main__":
    main()