#ifndef UTILS_CF_H
#define UTILS_CF_H

#include <vector>
#include <map>
#include <stdexcept>
#include <iostream>
#include "parser.h"
#include "lexer.h"
using std::cout;
using std::endl;

template <typename K, typename V>
class OrderedMap
{
private:
	std::vector<std::pair<K, V>> cpp_map;
public:
	OrderedMap() {}
	void initialize(std::map<std::string, std::vector<std::string>> s);
	std::pair<K, V>& operator[](size_t i);
	V& value(K key);
	size_t size();
};

template <typename K, typename V>
inline void OrderedMap<K, V>::initialize(std::map<std::string, std::vector<std::string>> s)
{
	for (auto i = s.begin(); i != s.end(); ++i) {
		std::string first = i->first;
		if (isinteger(i->first.back())) {
			first.erase(first.begin() + first.size() - 1);
		}
		cpp_map.push_back(std::make_pair(first, i->second));
	}
}

template <typename K, typename V>
inline std::pair<K, V>& OrderedMap<K, V>::operator[](size_t i)
{
	if (i < 0 || i >= size()) throw std::invalid_argument("Index out of range.");
	return cpp_map[i];
}

template <typename K, typename V>
inline V& OrderedMap<K, V>::value(K key)
{
	for (auto& i : cpp_map) {
		if (i->first == key) return i->second;
	}
	throw std::invalid_argument("Key not defined.");
}

template <typename K, typename V>
inline size_t OrderedMap<K, V>::size()
{
	return cpp_map.size();
}

template <typename T>
class Stack {
private:
	std::vector<T> cpp_vect;
public:
	Stack() {};
	Stack(std::vector<T>& inp);
	
	void push(T obj);
	T pull();
	T& access();

	bool is_empty();
	size_t size();

	std::vector<T>& cvect();
	void print();
};

template <typename T>
inline Stack<T>::Stack(std::vector<T>& inp) { cpp_vect = inp; }

template <typename T>
inline void Stack<T>::push(T obj) { cpp_vect.push_back(obj); }

template <typename T>
inline T Stack<T>::pull() { 
	T tor = cpp_vect.back();
	cpp_vect.erase(cpp_vect.begin() + size() - 1);
	return tor; 
}

template <typename T>
inline T& Stack<T>::access() { return cpp_vect.back(); }

template <typename T>
inline bool Stack<T>::is_empty() { return cpp_vect.size() == 0; }

template <typename T>
inline size_t Stack<T>::size() { return cpp_vect.size(); }

template <typename T>
inline std::vector<T>& Stack<T>::cvect() { return cpp_vect; }

template<typename T>
inline void Stack<T>::print()
{
	cout << "[";
	for (T& i : cpp_vect) {
		cout << i.value << ", ";
	}
	cout << "\b\b]\n";
}

#endif // UTILS_CF_H
