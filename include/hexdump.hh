#include <cctype>
#include <cstddef>
#include <format>
#include <fstream>

#if __cplusplus >= 202302L
#	include <print>
#endif
#include <iostream>
#include <vector>

namespace hexdump
{
	class config
	{
	public:
		// how much to output per line
		int width = 16;

		// decode as ascii and show on the side?
		bool show_ascii = true;

		// show address as hex on the left
		bool show_address = true;

	private:
		std::string hexdump_single(unsigned char *data, std::size_t offset, int left_line)
		{
			std::string ret = "|";

			if (show_address) { ret = std::format("|{:08x}|", offset); }

			for (std::size_t i = 0; i < width && i < left_line; i++)
			{
				ret += std::format("{:02x} ", data[i + offset]);
			}


			if (show_ascii)
			{
				if (left_line != width)
				{
					for (int i = left_line; i < width; i++) { ret += "   "; }
				}


				ret += "| ";

				for (std::size_t i = 0; i < width && i < left_line; i++)
				{
					auto c = data[i + offset];
					if (std::isprint(c) && c != '\n') { c = data[i + offset]; }
					else
					{
						c = '.';
					}
					ret += std::format("{:c}", c);
				}
			}


			return ret;
		}

	public:
		inline void dump(unsigned char *data, std::size_t size)
		{

			auto aligned_size = size - (size % width);


			for (std::size_t offset = 0; offset < aligned_size; offset += width)
			{
#if __cplusplus >= 202302L
				std::println("{}", hexdump_single(data, offset, width));
#else
				std::cout << hexdump_single(data, size) << '\n';
#endif
			}

#if __cplusplus >= 202302L
			std::println("{}", hexdump_single(data, aligned_size, size % width));
#else
			std::cout << hexdump_single(data, size) << '\n';
#endif
		}
		inline bool dump(std::ifstream &i)
		{
			i.seekg(std::ios::end);

			auto size = i.tellg();

			i.seekg(std::ios::beg);


			std::vector<unsigned char> buffer(size);


			// this is ugly
			if (i.read((char *)buffer.data(), size))
			{
				dump(buffer.data(), size);
				return true;
			}
			else
			{
				return false;
			}
		}
	};
}
