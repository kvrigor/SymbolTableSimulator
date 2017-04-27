
#ifndef HASH_H_
#define HASH_H_

#include <iostream>
#include "search.h"

using std::cout;
using std::string;

namespace Hash
{
	using namespace CustomTypes;
	//TODO: hash functions
	void SomeHashMethod(Symbol & input)
	{
		std::vector<Symbol> sampleVector;
		std::list<Symbol> sampleList;
	}
	
	class HashTable
	{
		private:
			std::list<Symbol> _symList;
			std::vector<Symbol> _symVector;
			int _tblSize;
			
			int hash(Symbol sym);
			
		public:
			HashTable();
			HashTable(const std::list<Symbol> &);
			
	};
	
	HashTable::HashTable()
	{
		_symList.push_back(Symbol());
		_symVector.push_back(Symbol());
		_tblSize = 1;
	}
	
	HashTable::HashTable(const std::list<Symbol> & lstSymbol)
	{
		int count = lstSymbol.size();
		for(int i = 0; i<count; i++)
		{
			
		}
	}
	
	/*
	int hash32 (Symbol sym, int tbl_size)
	{
		int h = 0;
		
		string asd = sym.ToString();
		std::cout<<"\n"<<sym.Name<<"\t"<<sym.Scope<<"\t"<<sym.Type<<"\t\n"
			<<asd<<"\n"<<asd.size()<<"\n";
		
		return asd.size();
	}
	
	
	int test(std::list<Symbol> testing)
	{
		std::vector<Symbol> testVector(testing.begin(), testing.end());
		return hash32(testVector[3], 2);
	}
	*/
	
}

namespace ColRes
{
	//TODO: collision resolution methods
}


#endif /* HASH_H_ */
