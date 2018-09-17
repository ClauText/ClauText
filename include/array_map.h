
#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <vector>
#include <algorithm>
#include <utility>

// like set, map,   instead of map!
namespace wiz {
	template <class Key, class Data>
	class ArrayMap {
	public:
		using iterator = typename std::vector<std::pair<Key, Data>>::iterator;
		using const_iterator = typename std::vector<std::pair<Key, Data>>::const_iterator;
	private:
		std::vector<std::pair<Key, Data>> arr;
	public:
		explicit ArrayMap(size_t reserve_num = 0) {
			if (reserve_num > 0) {
				arr.reserve(reserve_num);
			}
		}
	private:
		void _sort(const size_t last, const size_t num) {
			size_t count = num >> 1; // two group, count is size of one group.

			std::inplace_merge(arr.begin() + (last - 2 * count + 1), arr.begin() + (last - count + 1), arr.begin() + (last + 1));

			//std::sort(begin() + last - 2 * count + 1, begin() + last + 1);//_merge(last - 2 * count + 1, last - count + 1, count);
		}
		iterator binary_search(long long left, long long right, const Key& key)
		{
			long long middle = (left + right) >> 1; // / 2;

			while (left <= right) {
				auto& x = arr[middle].first;

				if (x == key) {
					return begin() + middle;
				}
				else if (x < key) {
					left = middle + 1;
				}
				else {
					right = middle - 1;
				}
				middle = (left + right) >> 1;
			}

			return end();
		}
		const_iterator binary_search(long long left, long long right, const Key& key) const
		{
			long long middle = (left + right) >> 1; // / 2;

			while (left <= right) {
				auto& x = arr[middle].first;

				if (x == key) {
					return begin() + middle;
				}
				else if (x < key) {
					left = middle + 1;
				}
				else {
					right = middle - 1;
				}
				middle = (left + right) >> 1;
			}

			return end();
		}

	public:
		bool empty() const {
			return arr.empty();
		}
		auto size() const {
			return arr.size();
		}
		void clear() {
			arr.clear();
		}
	public:
		const_iterator begin() const {
			return arr.begin();
		}
		const_iterator end() const {
			return arr.end();
		}
		iterator begin() {
			return arr.begin();
		}
		iterator end() {
			return arr.end();
		}

		iterator find(const Key& key) {
			if (arr.empty()) {
				return end();
			}
			if (arr.size() == 1) {
				return key == arr[0].first ? begin() + 0 : end();
			}

			// forest binary search
			{
				// size_t : unsigned ~?
				size_t num = 2;
				size_t len = arr.size();

				if (1 == len % 2) {
					if (key == arr.back().first) {
						return begin() + arr.size() - 1;
					}
					--len;
				}
				const size_t _len = len;
				long long left = std::max((long long)0, (long long)_len - 2);
				size_t right = _len - 1;

				len = len >> 1;

				while (len > 0) {
					if (1 == len % 2) {
						auto x = binary_search(left, right, key);
						if (x != end()) {
							return x;
						}
						right = left - 1;
					}
					else {

					}

					len = len >> 1;
					num = num * 2;

					left = right - num + 1;
				}
			}

			return end();
		}
		const_iterator find(const Key& key) const {
			if (arr.empty()) {
				return end();
			}
			if (arr.size() == 1) {
				return key == arr[0].first ? begin() : end();
			}

			// forest binary search
			{
				// size_t : unsigned ~?
				size_t num = 2;
				size_t len = arr.size();

				if (1 == len % 2) {
					if (key == arr.back().first) {
						return begin() + arr.size() - 1;
					}
					--len;
				}
				const size_t _len = len;
				long long left = std::max((long long)0, (long long)_len - 2);
				size_t right = _len - 1;

				len = len >> 1;

				while (len > 0) {
					if (1 == len % 2) {
						auto x = binary_search(left, right, key);
						if (x != end()) {
							return x;
						}
						right = left - 1;
					}
					else {

					}

					len = len >> 1;
					num = num * 2;

					left = right - num + 1;
				}
			}

			return end();
		}
	public:
		// using maybe quick sort, cf) insertion sort?
		// different point compared by std::map?
		void insert(std::pair<Key, Data>& value) {
			if (end() != find(value.first)) { find(value.first)->second = value.second; return; }
			arr.push_back(value);

			// -sort
			{
				// size_t : unsigned ~?
				size_t num = 2;
				size_t len = arr.size();

				if (1 == len % 2) {
					return;
				}

				len = len - 1;
				size_t x = arr.size() - 1;
				size_t temp = arr.size() ^ (arr.size() - 1);

				// when len is even... > 0
				while (len > 0 && temp > 0) {
					if (1 == len % 2) {
						//if (x - num < 0) {
						//	break;
						//}
						_sort(x, num);
					}
					len = len >> 1;
					num = num * 2;
					temp = temp >> 1;
				}
			}

			// debug

			//std::cout << std::endl;
			//std::cout << std::endl;
			//for (auto& x : arr) {
			//	std::cout << x.first << " ";
			//}
			//std::cout << std::endl;
			//std::cout << std::endl;
		}
		Data& at(const Key& key) {
			return (*this)[key];
		}
		const Data& at(const Key& key) const {
			return find(key)->second;
		}
		Data& operator[](const Key& key) {
			iterator idx;
			if (end() == (idx = find(key))) {
				std::pair temp(key, Data());
				insert(temp);
				return find(key)->second;
			}
			else {
				return idx->second;
			}
		}
	};
}


#endif

