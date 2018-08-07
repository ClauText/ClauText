
#include "wiz/global.h"

namespace wiz {

	long long checkDelimiter(const char* start, const char* last, const std::vector<std::string>& delimiter)
	{
		const int delim_size = delimiter.size();

		for (int delim_num = 0; delim_num < delim_size; ++delim_num) {
			bool pass = true;

			// size check
			if (start + delimiter[delim_num].size() - 1 > last) {
				continue;
			}

			for (const char* x = start; x <= start + delimiter[delim_num].size() - 1; ++x) {
				if (*x == delimiter[delim_num][x - start]) {

				}
				else {
					pass = false;
					break;
				}
			}

			if (pass) {
				return delim_num;
			}
		}

		return -1;
	}

}

