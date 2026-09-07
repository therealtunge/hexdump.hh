#include <hexdump.hh>

const char data[] = {
	'h',
	'e',
	'l',
	'l',
	'o',
	'\0',
	0x69,
	0x42,
};

int main() {
	hexdump::config config {};

	std::cout << "dumping normally" << std::endl;

	config.dump((unsigned char*)data, sizeof(data));

	config.show_ascii = false;

	std::cout << "dumping with no ascii" << std::endl;

	config.dump((unsigned char*)data, sizeof(data));

	config.show_ascii = true;
	config.show_address = false;

	std::cout << "dumping with no address" << std::endl;

	config.dump((unsigned char*)data, sizeof(data));

	config.width = 3;

	std::cout << "dumping with smaller width" << std::endl;

	config.dump((unsigned char*)data, sizeof(data));
}