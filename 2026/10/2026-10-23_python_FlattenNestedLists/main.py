def flatten(nested_list):
    flattened_list = []
    for item in nested_list:
        if isinstance(item, list):
            flattened_list.extend(flatten(item))
        if not isinstance(item, list):
            flattened_list.append(item)
    return flattened_list

def main():
    data = [1, [2, 3], [], [4, [5, [6]]], 7]
    print(flatten(data))

    ex_test = [
        1,[
            2,3,4,[
                5,[
                    6,7,8,
                    ],
                ],[
                    9,10
                ],[
                    [
                        11,12
                    ],13
                ],14,[
                    16,[
                        17,18,19
                    ]
                ],20,21,22,23,24,25,26,27,28,29,30
            ]
        ]

    print("EXTRA TEST:", flatten(ex_test))
if __name__ == "__main__":
    main()