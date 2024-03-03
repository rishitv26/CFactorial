#ifndef UTILS_CF_H
#define UTILS_CF_H

#include <vector>
#include <map>
#include <stdexcept>
#include "parser.h"
#include "lexer.h"

template <class K, class V>
class OrderedMap
{
private:
	std::vector<std::pair<K, V>> cpp_map;
public:
	OrderedMap() {}
	OrderedMap(SyntaxGrammerMap s);
	std::pair<K, V>& operator[](size_t i);
	size_t size();
};

template<class K, class V>
inline OrderedMap<K, V>::OrderedMap(SyntaxGrammerMap s)
{
	for (auto i = s.begin(); i != s.end(); ++i) {
		std::string first = i->first;
		if (isinteger(i->first.back())) {
			first.erase(first.begin() + first.size() - 1);
		}
		cpp_map.push_back(std::make_pair(first, i->second));
	}
}

template<class K, class V>
inline std::pair<K, V>& OrderedMap<K, V>::operator[](size_t i)
{
	if (i < 0 || i >= size()) throw std::invalid_argument("Index out of range.");
	return cpp_map[i];
}

template<class K, class V>
inline size_t OrderedMap<K, V>::size()
{
	return cpp_map.size();
}

#endif // UTILS_CF_H
