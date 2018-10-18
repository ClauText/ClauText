#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include "global.h"
#include <vector>
#include <queue>
#include <algorithm>
#include <utility>
#include <execution>

// like set, map,   instead of map!
namespace wiz {

	template <class Key, class Data>
	class Pair
	{
	public:
		Key first;
		Data second;
		//	std::string second;
	public:
		Pair() { }

		Pair(const Key& key, const Data& data)
			: first(key), second(data)
		{

		}

	public:
		bool operator<(const Pair& other) const noexcept {
			return this->first < other.first;
		}
		bool operator!=(const Pair& other) const noexcept {
			return this->first != other.first;
		}
		bool operator==(const Pair& other) const noexcept {
			return this->first == other.first;
		}
	};

	typedef enum _Color { RED = 0, BLACK = 1 } Color;

	template < class T >
	class RB_Node // no REMOVE method!
	{
	public:
		long long id = 0; // NULL value? id == -1 ?
		T key;
		long long  left = 0;
		long long  right = 0;
		long long  p = 0; // parent
		Color color;
		long long next = 0; // for dead..
		long long min_next = 0;
		long long min_before = 0;
		long long max_next = 0;
		long long max_before = 0;
		bool dead = false; //
	public:
		void Clear()
		{
			id = 0;
			key = T();
			left = 0;
			right = 0;
			p = 0;
			color = RED;
			next = 0;
			dead = false;

			min_next = 0;
			max_next = 0;
			min_before = 0;
			max_before = 0;
		}
	public:
		explicit RB_Node(const T& key = T()) : key(key), color(BLACK) { }

	public:
		bool operator==(const RB_Node<T>& other)const {
			return this->id == other.id && other.id != -1;
		}
	};

	template < class T, class COMP = ASC<T>, class COMP2 = EQ<T> >
	class RB_Tree
	{
	private:
		bool IsNULL(const RB_Node<T>& x)const noexcept
		{
			return x.id <= 0 || x.id > arr.size();
		}
		bool IsNULL(const long long id) const noexcept
		{
			return id <= 0 || id > arr.size();
		}
		void Clear()
		{
			arr.clear();
			arr.push_back(RB_Node<T>());
			root = 0;
			count = 0;

			max_list = 0;
			min_list = 0;

			remain_list.clear();
		}
	private:
		std::vector<T> remain_list;
		std::vector<RB_Node<T>> arr = { RB_Node<T>() };
		long long root = 0;
		long long count = 0;
		long long dead_list = 0;

		long long max_list = 0;
		long long min_list = 0;
	public:
		explicit RB_Tree() {  }
		virtual ~RB_Tree() {
			//
		}
		void reserve(size_t num) {
			arr.reserve(num + 1);
		}

		using iterator = typename std::vector<RB_Node<T>>::iterator;
		using const_iterator = typename std::vector<RB_Node<T>>::const_iterator;

		iterator begin() {
			return arr.begin() + 1;
		}
		const_iterator begin() const {
			return arr.cbegin() + 1;
		}
		iterator end() {
			return arr.end();
		}
		const_iterator end() const {
			return arr.cend();
		}
	private:// LEFT ROTATE Tree x
		void LEFT_ROTATE(RB_Tree<T, COMP>* tree, RB_Node<T>*  x) {
			//if( x !=NIL<T>::Get() || tree.root != NIL<T>::Get() ) { return; }
			RB_Node <T>*   y;
			// y = right[x]
			y = &(tree->arr[x->right]);
			// right[x] = left[y]
			x->right = y->left;
			// if(left[y] != nil[T]
			if (!IsNULL(tree->arr[y->left])) {
				//    then p[left[y]] = x
				tree->arr[y->left].p = x->id;
			}
			// p[y] = p[x]
			y->p = x->p;
			// if( p[x] = nil[T]
			if (IsNULL(tree->arr[x->p])) {
				//  then root[T] = y
				tree->root = y->id;
			}
			//  else if x = left[p[x]]
			else if (x == &(tree->arr[tree->arr[x->p].left])) {
				//      then left[p[x]] = y
				tree->arr[x->p].left = y->id;
			}
			//  else right[p[x]] = y
			else { tree->arr[x->p].right = y->id; }
			// left[y] = x
			y->left = x->id;
			// p[x] = y
			x->p = y->id;
		}
		// Right ROTATE Tree x // left <> right
		void RIGHT_ROTATE(RB_Tree<T, COMP>* tree, RB_Node<T>*  x) {
			//if( x !=NIL<T>::Get() || tree.root != NIL<T>::Get() ) { return; }

			RB_Node <T>*  y;
			// y = right[x]
			y = &(tree->arr[x->left]);
			// right[x] = left[y]
			x->left = y->right;
			// if(left[y] != nil[T]
			if (!IsNULL(tree->arr[y->right])) {
				//    then p[left[y]] = x
				tree->arr[y->right].p = x->id;
			}
			// p[y] = p[x]
			y->p = x->p;
			// if( p[x] = nil[T]
			if (IsNULL(tree->arr[x->p])) {
				//  then root[T] = y
				tree->root = y->id;
			}
			//  else if x = left[p[x]]
			else if (x == &(tree->arr[tree->arr[x->p].right])) {
				//      then left[p[x]] = y
				tree->arr[x->p].right = y->id;
			}
			//  else right[p[x]] = y
			else { tree->arr[x->p].left = y->id; }
			// left[y] = x
			y->right = x->id;
			// p[x] = y
			x->p = y->id;
		}
		const RB_Node <T>*  SEARCH(const RB_Node <T>*   x, const T& k) const
		{
			COMP comp;

			while (!IsNULL(*x) && k != x->key) { // != nil
				if (comp(k, x->key)) { // k < x.key
					x = const_cast<RB_Node<T>*>(&arr[x->left]);
				}
				else {
					x = const_cast<RB_Node<T>*>(&arr[x->right]);
				}
			}
			return x;
		}
		RB_Node <T>*  SEARCH(RB_Node <T>*   x, const T& k)
		{
			COMP comp;

			while (!IsNULL(*x) && k != x->key) { // != nil
				if (comp(k, x->key)) { // k < x.key
					x = &arr[x->left];
				}
				else {
					x = &arr[x->right];
				}
			}
			return x;
		}

		void INSERT_FIXUP(RB_Tree<T, COMP>* tree, RB_Node<T>*  z) /// google ..
		{
			RB_Node <T>*   y;
			while (tree->arr[z->p].color == RED) {
				if (z->p == tree->arr[tree->arr[z->p].p].left) {
					y = &(tree->arr[tree->arr[tree->arr[z->p].p].right]);
					if (y->color == RED) {
						tree->arr[z->p].color = BLACK;
						y->color = BLACK;
						tree->arr[tree->arr[z->p].p].color = RED;
						z = &(tree->arr[tree->arr[z->p].p]);
					}
					else
					{
						if (z == &(tree->arr[tree->arr[z->p].right])) {
							z = &(tree->arr[z->p]);
							LEFT_ROTATE(tree, z);
						}
						tree->arr[z->p].color = BLACK;
						tree->arr[tree->arr[z->p].p].color = RED;
						RIGHT_ROTATE(tree, &(tree->arr[tree->arr[z->p].p]));
					}
				}
				else {
					y = &(tree->arr[arr[tree->arr[z->p].p].left]);
					if (y->color == RED) {
						tree->arr[z->p].color = BLACK;
						y->color = BLACK;
						tree->arr[tree->arr[z->p].p].color = RED;
						z = &(tree->arr[tree->arr[z->p].p]);
					}
					else {
						if (z == &(tree->arr[tree->arr[z->p].left])) {
							z = &(tree->arr[z->p]);
							RIGHT_ROTATE(tree, z);
						}
						tree->arr[z->p].color = BLACK;
						tree->arr[tree->arr[z->p].p].color = RED;
						LEFT_ROTATE(tree, &(tree->arr[tree->arr[z->p].p]));
					}
				}
			}
			tree->arr[tree->root].color = BLACK;
		}
		long long INSERT(RB_Tree<T, COMP>* tree, const T& key, long long hint = 0)
		{
			COMP comp;
			COMP2 eq;

			long long _count = 0;
			const long long _count_max = 2;

			long long y_idx = 0;
			long long x_idx = tree->root;
			auto& chk = tree->arr;
			long long pass = 0;

			long long iter_min = 0, iter_min2 = 0;
			long long iter_max = 0, iter_max2 = 0;

			long long now = tree->arr.size(); //


			if (!IsNULL(tree->root)) {
				iter_min = tree->min_list;

				_count = 0;
				while (true) {
					_count++;
					if (_count > _count_max || iter_min == 0) {
						break;
					}
					else if (comp(key, chk[iter_min].key)) {
						x_idx = iter_min;

						pass = 1;
					}
					iter_min2 = iter_min;
					iter_min = chk[iter_min].min_next;
				}

				iter_max = tree->max_list;

				_count = 0;
				while (true) {
					_count++;
					if (_count > _count_max || iter_max == 0) {
						break;
					}
					else if (comp(chk[iter_max].key, key)) {
						x_idx = iter_max;

						pass = 2;
					}
					iter_max2 = iter_max;
					iter_max = chk[iter_max].max_next;
				}


				while (//!IsNULL(tree->arr[x_idx]) &&
					!IsNULL(x_idx) && !hint
					)
				{
					y_idx = x_idx;
					// if( z.key < x.key )
					if (comp(key, tree->arr[x_idx].key))
					{
						x_idx = tree->arr[x_idx].left;
					}
					else if (comp(tree->arr[x_idx].key, key)) {
						x_idx = tree->arr[x_idx].right;
					}
					else {
						break;
					}
				}

				if (hint) {
					y_idx = hint;
				}

				if (!IsNULL(x_idx) && !hint && eq(key, tree->arr[x_idx].key)) {
					tree->arr[x_idx].key = key;
					return x_idx;
				}
			}

			RB_Node<T>* z = nullptr;

			if (0 == tree->dead_list) {
				tree->arr.push_back(RB_Node<T>());
				tree->arr.back().id = now;
				tree->arr.back().key = key;

				z = &(tree->arr.back());

				if (1 == pass) {
					tree->min_list = now;
					arr[now].min_before = tree->min_list;
				}
				else if (2 == pass) {
					tree->max_list = now;
					arr[now].max_before = tree->max_list;
				}
			}
			else {
				now = tree->dead_list;
				tree->dead_list = tree->arr[now].next;
				long long id = tree->arr[now].id;
				long long next = tree->arr[now].next;

				tree->arr[now].Clear();
				tree->arr[now].id = id;
				tree->arr[now].key = key;
				tree->arr[now].next = next;
				tree->arr[now].dead = false;

				z = &(tree->arr[now]);

				if (1 == pass) {
					tree->min_list = now;
					arr[now].min_before = tree->min_list;
				}
				else if (2 == pass) {
					tree->max_list = now;
					arr[now].max_before = tree->max_list;
				}
			}

			z->p = tree->arr[y_idx].id;

			if (IsNULL(tree->arr[y_idx])) {
				tree->root = z->id;
				if (tree->empty()) {
					tree->min_list = z->id;
					tree->max_list = z->id;
				}
			}
			else if (comp(z->key, tree->arr[y_idx].key)) {
				tree->arr[y_idx].left = z->id;//
			}
			else {
				tree->arr[y_idx].right = z->id;//
			}

			z->left = 0; // = nil
			z->right = 0;
			z->color = RED; // = RED

			// insert fixup
			INSERT_FIXUP(tree, z);

			count++;

			return z->id;
		}

		void LAZYINSERT(RB_Tree<T, COMP>* tree, const T& key) {
			long long now = tree->arr.size();

			tree->remain_list.push_back(key);
		}

	private:
		void REALINSERT(RB_Tree<T, COMP>* tree) {
			if (tree->remain_list.empty()) {
				return;
			}

			std::sort(std::execution::par, tree->remain_list.begin(), tree->remain_list.end());
			for (const auto& x : tree->remain_list) {
				INSERT(tree, x);
			}
			tree->remain_list.clear();
		}
		RB_Node<T>* MAXIMUM(RB_Node<T>* x) {
			while (!IsNULL(x->right)) {
				x = &arr[x->right];
			}
			return x;
		}
		RB_Node<T>* MINIMUM(RB_Node<T>* x)
		{
			while (!IsNULL(x->left)) { // != nil
				x = &arr[x->left];
			}
			return x;
		}
		RB_Node<T>* SUCCESSOR(RB_Node<T>* x)
		{
			if (!IsNULL(x->right)) { // nil
				return MINIMUM(&arr[x->right]);
			}

			RB_Node<T>* y = &arr[x->p];
			while (!IsNULL(y->id) && x == &arr[y->right])
			{
				x = y; y = &arr[y->p];
			}
			return y;
		}

		void REMOVE_FIXUP(RB_Tree<T, COMP>* tree, RB_Node<T>* x)
		{
			RB_Node<T>* w;

			while (x->id != root && x->color == BLACK)
			{
				if (x == &(arr[arr[x->p].left])) {
					w = &(arr[arr[x->p].right]);
					if (w->color == RED) {
						w->color = BLACK;
						arr[x->p].color = RED;
						LEFT_ROTATE(tree, &arr[x->p]);
						w = &arr[arr[x->p].right];
					}
					if (arr[w->left].color == BLACK && arr[w->right].color == BLACK) {
						w->color = RED;
						x = &arr[x->p];
					}
					else {
						if (arr[w->right].color == BLACK) {
							arr[w->left].color = BLACK;
							w->color = RED;
							RIGHT_ROTATE(tree, w);
							w = &arr[arr[x->p].right];
						}
						w->color = arr[x->p].color;
						arr[x->p].color = BLACK;
						arr[w->right].color = BLACK;
						LEFT_ROTATE(tree, &arr[x->p]);
						x = &arr[root];
					}
				}
				else {
					w = &arr[arr[x->p].left];
					if (w->color == RED) {
						w->color = BLACK;
						arr[x->p].color = RED;
						RIGHT_ROTATE(tree, &arr[x->p]);
						w = &(arr[arr[x->p].left]);
					}
					if (arr[w->left].color == BLACK && arr[w->right].color == BLACK) {
						w->color = RED;
						x = &arr[x->p];
					}
					else {
						if (arr[w->left].color == BLACK) {
							arr[w->right].color = BLACK;
							w->color = RED;
							LEFT_ROTATE(tree, w);
							w = &(arr[arr[x->p].left]);
						}
						w->color = arr[x->p].color;
						arr[x->p].color = BLACK;
						arr[w->left].color = BLACK;
						RIGHT_ROTATE(tree, &arr[x->p]);
						x = &arr[root];
					}
				}
			}
			x->color = BLACK;
		}


		RB_Node<T>* REMOVE(RB_Tree<T, COMP>* tree, RB_Node<T>* z)
		{
			RB_Node<T>* x;
			RB_Node<T>* y;

			if (IsNULL(z->left)
				|| IsNULL(z->right)) {
				y = z;
			}
			else { y = SUCCESSOR(z); }

			if (!IsNULL(y->left)) {
				x = &(tree->arr[y->left]);
			}
			else { x = &(tree->arr[y->right]); }

			x->p = y->p;

			if (IsNULL(y->p)) {
				tree->root = x->id;
			}
			else if (y == &(arr[arr[y->p].left])) {
				arr[y->p].left = x->id;
			}
			else { arr[y->p].right = x->id; }

			if (y != z) { //important part!
				z->key = y->key; // chk??
				std::swap(z->dead, y->dead);
				std::swap(z->next, y->next);
				std::swap(z->min_before, y->min_before);
				std::swap(z->min_next, y->min_next);
				std::swap(z->max_before, y->max_before);
				std::swap(z->max_next, y->max_next);
			}
			if (y->color == BLACK) {
				REMOVE_FIXUP(tree, x);
			}
			return y;
		}
	public:
		void LazyInsert(RB_Tree<T, COMP>* tree, const T& key) {
			LAZYINSERT(this, key);
		}
		// insert, search, remove.
		long long Insert(const T& key)
		{
			return INSERT(this, key);
		}
		bool IsExist(const T& key)
		{
			// NULL != ~
			return !IsNULL(*SEARCH(&arr[root], key));
		}

		RB_Node<T>* Search(const T& key, long long* id = nullptr) {
			wiz::RB_Node<T>* x = SEARCH(&arr[root], key);

			if (nullptr != id) {
				*id = x->id;
			}

			return x;
		}

		const RB_Node<T>* Search(const T& key, long long* id = nullptr) const {
			const wiz::RB_Node<T>* x = SEARCH(&arr[root], key);

			if (nullptr != id) {
				*id = x->id;
			}

			return x;
		}

		void RealInsert()
		{
			REALINSERT(this);
		}

		void Remove(const T& key)
		{
			RB_Node<T>* node = SEARCH(&arr[root], key);

			if (!IsNULL(*node))
			{
				if (!IsNULL(node->min_before)) {
					arr[node->min_before].min_next = node->min_next;
				}
				if (!IsNULL(node->max_before)) {
					arr[node->max_before].max_next = node->max_next;
				}

				RB_Node<T>* temp = REMOVE(this, node);

				temp->dead = true;
				temp->next = this->dead_list;
				this->dead_list = temp->id;
				count--;
			}
		}

		T& Idx(long long idx) {
			return arr[idx].key;
		}
		const T& Idx(long long idx) const {
			return arr[idx].key;
		}

		bool IsEmpty() const
		{
			return 0 == count;
		}
		bool empty() const { return IsEmpty(); }
		long long GetCount() const
		{
			return count;
		}
		long long size() const { return count; }
		void clear() {
			Clear();
		}
	};

	template <class Key, class Data>
	class ArrayMap {
	public:
		using iterator = typename std::vector<RB_Node<wiz::Pair<Key, Data>>>::iterator;
		using const_iterator = typename std::vector<RB_Node<wiz::Pair<Key, Data>>>::const_iterator;
	private:
		mutable RB_Tree<wiz::Pair<Key, Data>> arr;
	public:
		explicit ArrayMap(size_t reserve_num = 0) {
			if (reserve_num > 0) {
				arr.reserve(reserve_num);
			}
		}

	public:
		bool empty() const {
			arr.RealInsert();

			return arr.empty();
		}
		auto size() const {
			arr.RealInsert();

			return arr.size();
		}
		void clear() {
			arr.clear();
		}
		void reserve(long long x) {
			arr.reserve(x);
		}
	public:
		iterator begin() {
			return arr.begin();
		}
		iterator end() {
			return arr.end();
		}
		const_iterator begin() const {
			return arr.begin();
		}
		const_iterator end() const {
			return arr.end();
		}
		iterator find(const Key& key) {
			arr.RealInsert();

			long long id;
			RB_Node<wiz::Pair<Key, Data>>* x = arr.Search(wiz::Pair<Key, Data>(key, Data()), &id);
			if (0 == x->id) {
				return arr.end();
			}
			return arr.begin() + (x->id - 1);
		}
		const_iterator find(const Key& key) const {
			arr.RealInsert();

			long long id;
			RB_Node<wiz::Pair<Key, Data>>* x = arr.Search(wiz::Pair<Key, Data>(key, Data()), &id);
			if (0 == x->id) {
				return arr.end();
			}
			return arr.begin() + (x->id - 1);
		}
	public:
		// different polong long compared by std::map?
		void insert(const std::pair<Key, Data>& value) {
			lazy_insert(value);

			//	arr.RealInsert();

			//	arr.Insert(wiz::Pair<Key, Data>(value.first, value.second));
		}
		void update()
		{
			arr.RealInsert();
		}
		void lazy_insert(const std::pair<Key, Data>& value) {
			arr.LazyInsert(&arr, wiz::Pair<Key, Data>(value.first, value.second));
		}

		void remove(const std::pair<Key, Data>& value)
		{
			arr.RealInsert();

			arr.Remove(wiz::Pair<Key, Data>(value.first, value.second));
		}

		Data& at(const Key& key) {
			return (*this)[key];
		}
		const Data& at(const Key& key) const {
			return find(key)->key.second;
		}

		Data& operator[](const Key& key) {
			arr.RealInsert();

			RB_Node<wiz::Pair<Key, Data>>* idx = arr.Search(wiz::Pair<Key, Data>(key, Data()));
			if (0 == idx->id) {
				long long _idx = arr.Insert(wiz::Pair<Key, Data>(key, Data())); //// return positon? - to do
				return arr.Idx(_idx).second;
			}
			else {
				return idx->key.second;
			}
		}
	};
}


#endif