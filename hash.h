
#ifndef HASH_H_
#define HASH_H_

#include <iostream>
#include "search.h"

using std::cout;
using std::string;

namespace Hash
{
	
	template <typename HashObj>
	class HashTable
	{
		public:
			HashTable(size_t size = 100);
			
			void Set(size_t);
			void MakeEmpty();
			int Size();
			bool Insert(const HashObj &);
			HashObj Retrieve(const string, bool &);
			std::list<HashObj> GetList();
			int activeCount();
			void SetHash2_R(int newR);
			int GetHash2_R();
			int Collision();
			
		private:
			//enum EntryType { ACTIVE, EMPTY, DELETED };
			struct HashValue
			{
				HashObj element;
				int info;
				
				HashValue(const HashObj & e = HashObj(), int i = 0)
				{
					element = e;
					info = i;
				}			
			};
			
			HashValue* _symVector;
			size_t _tblSize;
			int hash2_R;
			int colCount;
			int active;
			
			unsigned int nextPrime(const unsigned int);
			size_t hash(const string &);
			size_t hash2(const string &, int);
			size_t HornersHash(const string, int);
			void rehash();
			
	};
	
	template <typename HashObj>
	HashTable<HashObj>::HashTable(size_t size)
	{
		_tblSize = nextPrime(size);
		_symVector = new HashValue[_tblSize];
		MakeEmpty();
		hash2_R = nextPrime(_tblSize / 10);
	}
	
	template <typename HashObj>
	void HashTable<HashObj>::Set(size_t size)
	{
		_tblSize = nextPrime(size);
		_symVector = new HashValue[_tblSize];
		MakeEmpty();
		hash2_R = nextPrime(_tblSize / 10);
	}
	
	template <typename HashObj>
	void HashTable<HashObj>::MakeEmpty()
	{
		colCount = 0;
		active = 0;
		for(size_t i = 0; i < _tblSize; i++)
			_symVector[i].info = 0;
	}
	
	template <typename HashObj>
	int HashTable<HashObj>::Size()
	{
		return _tblSize;
	}
	
	template <typename HashObj>
	bool HashTable<HashObj>::Insert(const HashObj & entry)
	{
		HashObj temp = entry;
		size_t index = hash(temp.Name);
		
		if (_symVector[index].info == 1)
			return false;
		
		_symVector[index].element = entry;
		_symVector[index].info = 1;
		active++;
		
		//rehash if need
		// TODO: rehashCount
		if (active + 1 >= _tblSize)
		{
			rehash();
			cout<<"\nrehash\n";
		}
		
		return true;
	}
	
	template <typename HashObj>
	HashObj HashTable<HashObj>::Retrieve(const string key, bool & isFound)
	{
		size_t index = hash(key);
		HashValue retObj = _symVector[index];
		
		if (retObj.info != 1)
			isFound = false;
		else
			isFound = true;
		
		return retObj.element;
	}
	
	template <typename HashObj>
	std::list<HashObj> HashTable<HashObj>::GetList()
	{
		std::list<HashObj> tempList;
		for(int i = 0; i < _tblSize; i++)
			tempList.push_back(_symVector[i].element);
		return tempList;
	}
	
	template <typename HashObj>
	void HashTable<HashObj>::SetHash2_R(int newR)
	{
		hash2_R = nextPrime(newR);
	}
	
	template <typename HashObj>
	int HashTable<HashObj>::GetHash2_R()
	{
		return hash2_R;
	}
	
	template <typename HashObj>
	int HashTable<HashObj>::Collision()
	{
		return colCount;
	}
	
		// hi(x) = (hash(x) + f(i)) mod Table_Size
		// hi(x) is an index in the table to insert x
		// hash(x) is the hash function
		// f(i) is the collision resolution function
		// i is the current attempt to insert an element
		// x is the HashObj
		// Double Hashing
		// f(i) = i * hash2(x)
		// hash2(x) = R - (x mod R)
		// R: is prime, smaller than the table size
	
	template <typename HashObj>
	size_t HashTable<HashObj>::hash(const string & sym)
	{
		string temp = sym;
		string strHash = temp;
		
		size_t currentPos = HornersHash(temp, _tblSize);
		size_t offset = hash2(temp, hash2_R);
		
		while(_symVector[currentPos].info == 1 && _symVector[currentPos].element.Name != temp)
		{
			// TODO: collision count
			colCount++;
			currentPos += offset;
			if (currentPos >= _tblSize)
				currentPos -= _tblSize;
		}
		
		return currentPos;
	}
	
	template <typename HashObj>
	size_t HashTable<HashObj>::hash2(const string & sym, int R)
	{
		size_t hashVal = 0;
		string strName = sym;
		for(int i = 0; i < strName.size(); i++)
			hashVal = (37 * hashVal + strName[i]) % R;
			
		return R - hashVal;
	}
	
	template <typename HashObj>
	size_t HashTable<HashObj>::HornersHash(const string sym, int tableSize)
	{
		size_t hashVal = 0;
		string strName = sym;
		for(int i = 0; i < strName.size(); i++)
		{
			hashVal = (37 * hashVal + strName[i]) % tableSize;
		}
		
		return hashVal;
	}
	
	template <typename HashObj>
	unsigned int HashTable<HashObj>::nextPrime(const unsigned int num)
	{
		unsigned int newPrime = num;
		bool foundPrime = false;
		
		while (!foundPrime)
		{
			foundPrime = true;
			newPrime++;
			
			for(int div = 2; div <= newPrime / 2; div++)
			{
				int remainder = newPrime % div;
				if(remainder == 0)
					foundPrime = false;
			}
		}
		return newPrime;
	}
	
	template <typename HashObj>
	void HashTable<HashObj>::rehash()
	{
		HashValue* oldArray = _symVector;
		
		size_t oldSize = _tblSize;
		_tblSize = nextPrime( 2 * oldSize);
		_symVector = new HashValue[_tblSize];
		
		MakeEmpty();
		
		for(size_t i = 0; i < oldSize; i++)
			if(oldArray[i].info == 1)
				Insert(oldArray[i].element);
	}
	
	template <typename HashObj>
	int HashTable<HashObj>::activeCount()
	{
		return active;
	}
}

#endif /* HASH_H_ */
