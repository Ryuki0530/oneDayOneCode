from copy import deepcopy


def merge_settings(base, override):
	"""Return a deep, recursively merged copy of two settings dictionaries."""
	result = deepcopy(base)

	for key, override_value in override.items():
		base_value = result.get(key)
		if isinstance(base_value, dict) and isinstance(override_value, dict):
			result[key] = merge_settings(base_value, override_value)
		else:
			result[key] = deepcopy(override_value)

	return result


if __name__ == "__main__":
	base = {
		"theme": "light",
		"window": {"width": 800, "height": 600},
		"plugins": ["spellcheck"],
		"autosave": True,
	}
	override = {
		"theme": "dark",
		"window": {"width": 1200},
		"plugins": ["formatter"],
		"timeout": None,
	}
	print(merge_settings(base, override))

