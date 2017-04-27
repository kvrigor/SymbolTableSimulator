
#ifndef SEARCH_H_
#define SEARCH_H_

#include "structures.h"
#include <vector>
#include <list>


namespace SearchAlgorithms
{
	using namespace CustomTypes;

	bool BinarySearch(std::list<Symbol> & sortedSymbolTable, string searchKey, Symbol & result)
	{
		//std::vector<int> first;
		std::vector<Symbol> symTable(sortedSymbolTable.begin(),sortedSymbolTable.end());
		return true;
	}
}

#endif /* SEARCH_H_ */
