def parse_csv_line(line):
	fields = []
	current = []
	in_quotes = False
	index = 0

	while index < len(line):
		character = line[index]

		if character == '"':
			if in_quotes and index + 1 < len(line) and line[index + 1] == '"':
				current.append('"')
				index += 1
			else:
				in_quotes = not in_quotes
		elif character == ',' and not in_quotes:
			fields.append(''.join(current))
			current = []
		else:
			current.append(character)

		index += 1

	fields.append(''.join(current))
	return fields

# test
if __name__ == '__main__':
	print(parse_csv_line('apple,"banana,orange",grape') == [
		'apple', 'banana,orange', 'grape'
	])
	print(parse_csv_line('"say ""hello""",,end,') == [
		'say "hello"', '', 'end', ''
	])
	print(parse_csv_line('') == [''])
