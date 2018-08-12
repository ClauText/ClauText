
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
		size_t no_sort_start_idx = 1;
	public:
		bool empty() const {
			
			return arr.empty();
		}
		auto size() const {
			
			return arr.size();
		}
		void clear() {
			
			arr.clear();
			no_sort_start_idx = 1;
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
				return key == arr[0].first ? begin() : end();
			}

			// binary search
			{
				long long left = 0, right = no_sort_start_idx - 1;
				long long middle = (left + right) / 2;

				while (left <= right) {
					if (arr[middle].first == key) {
						return begin() + middle;
					}
					else if (arr[middle].first < key) {
						left = middle + 1;
					}
					else {
						right = middle - 1;
					}
					middle = (left + right) / 2;
				}
			}

			// linear search
			{
				for (long long i = no_sort_start_idx; i < arr.size(); ++i) {
					if (arr[i].first == key) {
						return begin() + i;
					}
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

			// binary search
			{
				long long left = 0, right = no_sort_start_idx - 1;
				long long middle = (left + right) / 2;
				
				while (left <= right) {
					if (arr[middle].first == key) {
						return begin() + middle;
					}
					else if (arr[middle].first < key) {
						left = middle + 1;
					}
					else {
						right = middle - 1;
					}

					middle = (left + right) / 2;
				}
			}

			// linear search
			{
				for (long long i = no_sort_start_idx; i < arr.size(); ++i) {
					if (arr[i].first == key) {
						return begin() + i;
					}
				}
			}
			return end();
		}
	public:
		// using maybe quick sort, cf) insertion sort?
		void insert(const std::pair<Key, Data>& value, bool do_sort = false) {
			if (end() != find(value.first)) { return; }
			arr.push_back(value);

			if (do_sort && no_sort_start_idx < arr.size()) {
				std::sort(arr.begin(), arr.end());
				no_sort_start_idx = arr.size();
			}
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
				insert(std::pair(key, Data()));
				return find(key)->second;
			}
			else {
				return idx->second;
			}
		}
	};
}


#endif

