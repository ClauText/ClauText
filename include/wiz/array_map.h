#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <unordered_map>
#include "queues.h"
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <utility>

namespace wiz {
	template <class Key, class Data>
	class ArrayMap
	{
	private:
		std::unordered_map<Key, Data> _map;
	public:
		using iterator = typename std::unordered_map<Key, Data>::iterator;
		using const_iterator = typename std::unordered_map<Key, Data>::const_iterator;
	public:
		void insert(std::pair<Key, Data>&& item) {
			_map[std::move(item.first)] = std::move(item.second);
		}
		void insert(const std::pair<Key, Data>& item) {
			_map[item.first] = item.second;
		}

		auto find(Key&& key) {
			return _map.find(std::move(key));
		}
		auto find(Key&& key) const {
			return _map.find(std::move(key));
		}
		auto find(const Key& key) {
			return _map.find(key);
		}
		auto find(const Key& key) const  {
			return _map.find(key);
		}

		Data& operator[] (Key&& key) {
			return _map[std::move(key)];
		}
		const Data& operator[] (Key&& key) const {
			return _map[std::move(key)];
		}

		Data& operator[] (const Key& key) {
			return _map[key];
		}
		const Data& operator[] (const Key& key) const {
			return _map[key];
		}

		void clear() {
			_map.clear();
		}

		auto size() const {
			return _map.size();
		}

		auto begin() {
			return _map.begin();
		}
		auto begin() const {
			return _map.begin();
		}
		auto end() {
			return _map.end();
		}
		auto end() const {
			return _map.end();
		}

		auto at(Key&& key) {
			return _map.at(std::move(key));
		}
		auto at(Key&& key) const {
			return _map.at(std::move(key));
		}
		auto at(const Key& key) {
			return _map.at(key);
		}
		auto at(const Key& key) const {
			return _map.at(key);
		}
	};
}

#endif