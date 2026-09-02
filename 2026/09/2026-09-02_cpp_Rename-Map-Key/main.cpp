 #include <iostream>
 #include <map>
 #include <string>
 #include <utility>

bool renameKey(
	std::map<std::string, int>& scores,
	const std::string& oldKey,
	const std::string& newKey)
{
	if (scores.find(oldKey) == scores.end() ||
		scores.find(newKey) != scores.end()) {
		return false;
	}

	auto node = scores.extract(oldKey);
	node.key() = newKey;
	scores.insert(std::move(node));
	return true;
}

void printScores(const std::map<std::string, int>& scores)
{
	for (const auto& [name, score] : scores) {
		std::cout << name << ": " << score << '\n';
	}
}

int main()
{
	std::map<std::string, int> scores{
		{"Alice", 80},
		{"Bob", 95},
		{"Carol", 70}
	};

	if (renameKey(scores, "Bob", "Robert")) {
		std::cout << "Renamed successfully\n";
	} else {
		std::cout << "Rename failed\n";
	}

	printScores(scores);
	return 0;
}
