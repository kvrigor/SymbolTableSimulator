
#ifndef SEARCH_H_
#define SEARCH_H_

#include "structures.h"
#include <vector>
#include <list>


namespace SearchAlgorithms
{
	using namespace CustomTypes;

	bool BinarySearch(std::vector<Symbol> & symbolTable, string searchKey, Symbol & result)
	{
		int low = 0;
		int high = symbolTable.size() - 1;
		int mid;

		while (low <= high)
		{
			mid = (low + high) / 2;
			if (symbolTable[mid].Name > searchKey)
			{
				high = mid - 1;
			}
			else if (symbolTable[mid].Name < searchKey)
			{
				low = mid + 1;
			}
			else
			{
				result = symbolTable[mid];
				return true;
			}
		}
		return false;
	}

	bool BinaryInsert(std::vector<Symbol> & symbolTable, Symbol newSymbol)
	{
		int low = 0;
		int high = symbolTable.size() - 1;
		int mid, insertPosition;

		//std::cout<<"\n";
		while (low <= high)
		{
			mid = (low + high) / 2;
			//std::cout<<"low = "<<low<<", high = "<<high<<", mid = "<<mid<<"\n";
			if (symbolTable[mid].Name > newSymbol.Name)
			{
				//std::cout<<symbolTable[mid].Name<<" > "<<searchKey<<"\n\n";
				insertPosition = mid;
				high = mid - 1;
			}
			else if (symbolTable[mid].Name < newSymbol.Name)
			{
				//std::cout<<symbolTable[mid].Name<<" < "<<searchKey<<"\n\n";
				insertPosition = mid + 1;
				low = mid + 1;
			}
			else
			{
				//std::cout<<symbolTable[mid].Name<<" = "<<searchKey<<"\n\n";
				return false;
			}
		}
		//std::cout<<"Will insert before pos "<<insertPosition;

		symbolTable.insert(symbolTable.begin()+insertPosition,newSymbol);
		return true;
	}

	bool BinaryDelete(std::vector<Symbol> & symbolTable, Symbol newSymbol)
	{
		//TODO
	}
}

#endif /* SEARCH_H_ */
