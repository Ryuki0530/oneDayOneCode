#include <iostream>

class PermissionFlags {
public:
	static constexpr unsigned int READ = 1;
	static constexpr unsigned int WRITE = 2;
	static constexpr unsigned int EXECUTE = 4;

	void grant(unsigned int flags) {
		permissions_ |= flags;
	}

	void revoke(unsigned int flags) {
		permissions_ &= ~flags;
	}

	bool has(unsigned int flags) const {
		return (permissions_ & flags) == flags;
	}

private:
	unsigned int permissions_ = 0;
};

//test
int main() {
	PermissionFlags permissions;

	std::cout << std::boolalpha;
	std::cout << permissions.has(PermissionFlags::READ) << '\n';

	permissions.grant(PermissionFlags::READ | PermissionFlags::WRITE);
	std::cout << permissions.has(PermissionFlags::READ) << '\n';
	std::cout << permissions.has(PermissionFlags::READ | PermissionFlags::EXECUTE) << '\n';

	permissions.grant(PermissionFlags::EXECUTE);
	std::cout << permissions.has(PermissionFlags::READ | PermissionFlags::WRITE |
								  PermissionFlags::EXECUTE) << '\n';

	permissions.revoke(PermissionFlags::WRITE);
	std::cout << permissions.has(PermissionFlags::WRITE) << '\n';
	std::cout << permissions.has(PermissionFlags::READ | PermissionFlags::EXECUTE) << '\n';

	return 0;
}
