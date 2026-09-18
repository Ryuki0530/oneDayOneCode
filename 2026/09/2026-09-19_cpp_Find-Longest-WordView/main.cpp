 #include <cstddef>
 #include <string_view>

 std::string_view findLongestWord(std::string_view text) {
	 std::string_view longest;
	 std::size_t start = 0;

	 while (start < text.size()) {
		 const std::size_t end = text.find(' ', start);
		 const std::size_t length = end == std::string_view::npos
			 ? text.size() - start
			 : end - start;
		 const std::string_view word = text.substr(start, length);

		 if (word.size() > longest.size()) {
			 longest = word;
		 }

		 if (end == std::string_view::npos) {
			 break;
		 }
		 start = end + 1;
	 }

	 return longest;
 }
