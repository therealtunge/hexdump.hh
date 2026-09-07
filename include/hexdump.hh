#include <cctype>
#include <cstddef>
#include <format>
#include <fstream>
#include <span>
#include <string_view>

#if __cplusplus >= 202302L
#	include <print>
#else
#include <iostream>
#endif

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
		std::string hexdump_single(const unsigned char *data, std::size_t offset, int left_line)
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
		void dump(unsigned char const *data, std::size_t size)
		{

			auto aligned_size = size - (size % width);


			for (std::size_t offset = 0; offset < aligned_size; offset += width)
			{
#if __cplusplus >= 202302L
				std::println("{}", hexdump_single(data, offset, width));
#else
				std::cout << hexdump_single(data, size, width) << '\n';
#endif
			}

#if __cplusplus >= 202302L
			std::println("{}", hexdump_single(data, aligned_size, size % width));
#else
			std::cout << hexdump_single(data, size, width) << '\n';
#endif
		}


		void dump(std::span<unsigned char> span)
		{
			dump(span.data(), span.size());
		}

		void dump(std::string_view view)
		{
			dump((unsigned const char*)view.begin(), view.size());
		}

		bool dump_file(std::ifstream &i)
		{
			auto buffer = std::string(std::istreambuf_iterator<char>(i), {});
			dump((unsigned char *)buffer.data(), buffer.size());

			return true;
		}
	};
}
