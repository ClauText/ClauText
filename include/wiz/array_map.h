#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include "global.h"
#include <vector>
#include <algorithm>
#include <utility>

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
		void Clear()
		{
			arr.clear();
			arr.push_back(RB_Node<T>());
			root = 0;
			count = 0;
			min_idx = 0;
			max_idx = 0;
		}
	private:
		std::vector<RB_Node<T>> arr = { RB_Node<T>() };
		long long root = 0;
		long long count = 0;
		long long min_idx = 0;
		long long max_idx = 0;
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
		void INSERT(RB_Tree<T, COMP>* tree, const T& key)
        {
			COMP comp;
			COMP2 eq;

			// y= nil<T>?
			//RB_Node <T>*   y = &arr[0]; 
			//RB_Node <T>*   x = &arr[tree->root];
			long long y_idx = 0;
			long long x_idx = tree->root;
			auto& chk = tree->arr;
			bool pass = false;

			if (comp(key, chk[min_idx].key)) {
				y_idx = min_idx;
				min_idx = arr.size();
				pass = true;
			}
			else if (comp(chk[max_idx].key, key)) {
				y_idx = max_idx;
				max_idx = arr.size();
				pass = true;
			}

			while (!pass && !IsNULL(chk[x_idx]) && key != chk[x_idx].key)
			{
				y_idx = x_idx;
				// if( z.key < x.key )
				if (comp(key, tree->arr[x_idx].key))
				{
					x_idx = tree->arr[x_idx].left;
				}
				else {
					x_idx = tree->arr[x_idx].right;
				}
			}

			if (!pass && eq(key, tree->arr[x_idx].key)) {
				tree->arr[x_idx].key = key;
				return;
			}

			long long now = arr.size();
			arr.push_back(RB_Node<T>());
			arr.back().id = now;
			arr.back().key = key;
			RB_Node<T>* z = &arr.back();


			z->p = tree->arr[y_idx].id;

			if (IsNULL(tree->arr[y_idx])) {
				tree->root = 1;
				tree->min_idx = 1;
				tree->max_idx = 1;
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
		}

	public:
	
		// insert, search, remove.
		void Insert(const T& key)
		{
			INSERT(this, key);
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

		bool IsEmpty() const
		{
			return 0 == count;
		}
		bool empty() const { return IsEmpty(); }
		int GetCount() const
		{
			return count;
		}
		int size() const { return count; }
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
		RB_Tree<wiz::Pair<Key, Data>> arr;
	public:
		explicit ArrayMap(size_t reserve_num = 0) {
			if (reserve_num > 0) {
				arr.reserve(reserve_num);
			}
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
		void reserve(long long x) {
			arr.reserve(x);
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
			long long id;
			RB_Node<wiz::Pair<Key, Data>>* x = arr.Search(wiz::Pair<Key, Data>(key, Data()), &id);
			if (0 == x->id) {
				return arr.end();
			}
			return arr.begin() + (x->id - 1);
		}
		const_iterator find(const Key& key) const {
			long long id;
			const RB_Node<wiz::Pair<Key, Data>>* x = arr.Search(wiz::Pair<Key, Data>(key, Data()), &id);
			if (0 == x->id) {
				return arr.end();
			}
			return arr.begin() + (x->id - 1);
		}
	public:
		// different point compared by std::map?
		void insert(std::pair<Key, Data>& value) {
			arr.Insert(wiz::Pair<Key, Data>(value.first, value.second));
		}

		Data& at(const Key& key) {
			return (*this)[key];
		}
		const Data& at(const Key& key) const {
			return find(key)->key.second;
		}

		Data& operator[](const Key& key) {
			RB_Node<wiz::Pair<Key, Data>>* idx = arr.Search(wiz::Pair<Key, Data>(key, Data()));
			if (0 == idx->id) {
				arr.Insert(wiz::Pair<Key, Data>(key, Data())); // return positon?
				idx = arr.Search(wiz::Pair<Key, Data>(key, Data()));
				return idx->key.second;
			}
			else {
				return idx->key.second;
			}
		}
	};
}


#endif