import argparse
class PopulationData:
    def __init__(self, file_path):
        self.file_path = file_path
        self.initialize_data()
        with open(self.file_path, 'r', encoding = 'utf-8') as file:
            label_lines = file.readlines()
            self.labels = label_lines[0].strip().split(",")
            for line in label_lines[1:]:
                if not line.strip():
                    continue
                split_line = line.strip().split(",")
                if len(split_line) != len(self.labels):
                    print(f"Warning: Data length mismatch in line: {line.strip()}")
                self.add_data(split_line)

    def initialize_data(self):
        self.data = []  # データを空のリストに初期化
    
    def add_data(self, data_list):
        if len(data_list) != len(self.labels):
            print(f"Warning: Data length mismatch for line: {data_list}")
        else:
            record = {
                'prefecture' : data_list[0],
                'city' : data_list[1],
                'population' : int(data_list[2]),
            }
            self.data.append(record)
    
    def dump(self):
        for record in self.data:
            print(record)

    def get_prefectures_data_dict(self):
        prefecture_dict = {}
        for record in self.data:
            prefecture = record['prefecture']
            population = record['population']
            if prefecture in prefecture_dict:
                prefecture_dict[prefecture] += population
            else:
                prefecture_dict[prefecture] = population
        return prefecture_dict

def main():
    # データファイルのパスを指定
    parser = argparse.ArgumentParser()
    parser.add_argument("--file", default="population.csv", help="使用するファイル名")
    args = parser.parse_args()

    population_data = PopulationData(args.file)

    # Debug
    # population_data.dump()

    # 都道府県のリストを取得
    prefectures = population_data.get_prefectures_data_dict()
    sorted_prefectures = sorted(prefectures.items(), key=lambda x: x[1], reverse=True)
    for pref, pop in sorted_prefectures:
        print(f"{pref}: {pop}")
if __name__ == "__main__":
    main()