
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
		std::vector<Symbol> symbolTable(sortedSymbolTable.begin(),sortedSymbolTable.end());
		int low = 0;
		int high = symbolTable.size() - 1;
		int mid;

		//std::cout<<"\n";
		while (low <= high)
		{
			mid = (low + high) / 2;
			//std::cout<<"low = "<<low<<", high = "<<high<<", mid = "<<mid<<"\n";
			if (symbolTable[mid].Name > searchKey)
			{
				//std::cout<<symbolTable[mid].Name<<" > "<<searchKey<<"\n\n";
				high = mid - 1;
			}
			else if (symbolTable[mid].Name < searchKey)
			{
				//std::cout<<symbolTable[mid].Name<<" < "<<searchKey<<"\n\n";
				low = mid + 1;
			}
			else
			{
				//std::cout<<symbolTable[mid].Name<<" = "<<searchKey<<"\n\n";
				result = symbolTable[mid];
				return true;
			}
		}
		return false;
	}
}

#endif /* SEARCH_H_ */
