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
	class Pair
	{
	public:
		Key first;
		Data second;
	public:
		explicit Pair() { }
		explicit Pair(const Key& key, const Data& data)
		: first(key), second(data) {

		}
	public:
		bool operator<(const Pair& other) const
		{
			return first < other.first;
		}
		bool operator==(const Pair& other) const
		{
			return first == other.first;
		}
		bool operator!=(const Pair& other) const
		{
			return first != other.first;
		}
	};

	template <class Key, class Data>
	class ArrayMap
	{
	private:
		mutable std::vector<wiz::Pair<Key, Data>> _map;
	public:
		using iterator = typename std::vector<wiz::Pair<Key, Data>>::iterator;
		using const_iterator = typename std::vector<wiz::Pair<Key, Data>>::const_iterator;
	public:
		void insert(std::pair<Key, Data>&& item) {
			for (int i = 0; i < _map.size(); ++i) {
				if (_map[i].first == item.first) {
					_map[i] = wiz::Pair(std::move(item.first), std::move(item.second));
					return;
				}
			}
			_map.push_back(wiz::Pair(std::move(item.first), std::move(item.second)));
		}
		void insert(const std::pair<Key, Data>& item) {
			for (int i = 0; i < _map.size(); ++i) {
				if (_map[i].first == item.first) {
					_map[i] = wiz::Pair(item.first, item.second);
					return;
				}
			}
			_map.push_back(wiz::Pair(item.first, item.second));
		}

		auto find(Key&& key) {
			return std::find(_map.begin(), _map.end(), wiz::Pair(std::move(key), Data()));
		}
		auto find(Key&& key) const {
			return std::find(_map.begin(), _map.end(), wiz::Pair(std::move(key), Data()));
		}
		auto find(const Key& key) {
			return std::find(_map.begin(), _map.end(), wiz::Pair(key, Data()));
		}
		auto find(const Key& key) const {
			return std::find(_map.begin(), _map.end(), wiz::Pair(key, Data()));
		}

		Data& operator[] (Key&& key) {
			for (int i = 0; i < _map.size(); ++i) {
				if (_map[i].first == key) {
					return _map[i].second;
				}
			}

			_map.push_back(wiz::Pair(std::move(key), Data()));
			return _map.back().second;
		}
		const Data& operator[] (Key&& key) const {
			for (int i = 0; i < _map.size(); ++i) {
				if (_map[i].first == key) {
					return _map[i].second;
				}
			}

			_map.push_back(wiz::Pair(std::move(key), Data()));
			return _map.back().second;
		}

		Data& operator[] (const Key& key) {
			for (int i = 0; i < _map.size(); ++i) {
				if (_map[i].first == key) {
					return _map[i].second;
				}
			}

			_map.push_back(wiz::Pair(key, Data()));
			return _map.back().second;
		}
		const Data& operator[] (const Key& key) const {
			for (int i = 0; i < _map.size(); ++i) {
				if (_map[i].first == key) {
					return _map[i].second;
				}
			}

			_map.push_back(wiz::Pair(key, Data()));
			return _map.back().second;
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
			return (*this)[std::move(key)]; 
		}
		auto at(Key&& key) const {
			return (*this)[std::move(key)];
		}
		auto at(const Key& key) {
			return (*this)[key];
		}
		auto at(const Key& key) const {
			return (*this)[key];
		}
	};
	
	template <class Key, class Data>
	class Map2
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