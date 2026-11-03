def round_robin(groups):
    max_length = max(len(group) for group in groups)
    for i in range(max_length):
        for group in groups:
            if i < len(group):
                yield group[i]

def main():
    groups = [
        ["A", "B", "C"],
        [1],
        [],
        ["x", "y"]
    ]

    print(list(round_robin(groups)))

if __name__ == "__main__":
    main()