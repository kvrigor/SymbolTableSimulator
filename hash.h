
#ifndef HASH_H_
#define HASH_H_

#include <iostream>
#include "search.h"

using std::cout;
using std::string;

namespace Hash
{
	//using namespace CustomTypes;
	
	template <typename HashObj>
	class HashTable
	{
		public:
			HashTable(int size = 100);
			
			void MakeEmpty();
			bool Insert(const HashObj &);
			
		private:
			enum EntryType { ACTIVE, EMPTY, DELETED };
			struct HashValue
			{
				HashObj element;
				EntryType info;
				
				HashValue(const HashObj & e = HashObj(), EntryType i = EMPTY)
				{
					element = e;
					info = i;
				}
				//HashValue(HashObj & e, EntryType i = EMPTY)
				//	: element{ e }, info{ i } {}				
			};
			
			std::vector<HashValue> _symVector;
			int _tblSize;
			
			unsigned int nextPrime(const int);
			int hash(const HashObj &);
			int hash2(const HashObj &, int);
			
			unsigned int HornersHash(const HashObj, int);
			
	};
	
	template <typename HashObj>
	HashTable<HashObj>::HashTable(int size) : _symVector(nextPrime(size))
	{ MakeEmpty(); _tblSize = _symVector.size();}
	
	template <typename HashObj>
	void HashTable<HashObj>::MakeEmpty()
	{
		_tblSize = 0;
		//for(int i = 0; i < _symVector.size(); i++)
		//_symVector[i]
	}
	
	template <typename HashObj>
	bool HashTable<HashObj>::Insert(const HashObj & entry)
	{
		int index = hash(entry);
		
		_symVector[index].element = entry;
		_symVector[index].info = ACTIVE;
		
		//rehash if need
		
		
		return true;
	}
	
	template <typename HashObj>
	int HashTable<HashObj>::hash(const HashObj & sym)
	{
		HashObj temp = sym;
		string strHash = temp.ToString();
		
		int currentPos = HornersHash(temp, _tblSize);
		int offset = hash2(temp, 23);
		
		cout<<"\ncurrentPos: "<<currentPos<<"\noffset: "<<offset<<"\n";
		//getch();
		
		while(_symVector[currentPos].info != EMPTY && !CustomTypes::CompareSymbols(_symVector[currentPos].element, temp))
		{
			currentPos += offset;
			if (currentPos >= _symVector.size())
				currentPos -= _symVector.size();
		}
		cout<<"\nused position: "<<currentPos<<"\n";
		
		return currentPos;
		
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
	}
	
	template <typename HashObj>
	int HashTable<HashObj>::hash2(const HashObj & sym, int R)
	{
		unsigned int hashVal = 0;
		string strName = sym.Name;
		for(int i = 0; i < strName.size(); i++)
			hashVal = 37 * hashVal + strName[i];
			
		return R - (hashVal % R);
	}
	
	template <typename HashObj>
	unsigned int HashTable<HashObj>::HornersHash(const HashObj sym, int tableSize)
	{
		unsigned int hashVal = 0;
		
		string strName = sym.Name;
		
		cout<<"\n**HornersHash**\nstrName: "<<strName<<"\nlength: "<<strName.size()<<"\ntableSize: "<<tableSize;
		//getch();
		
		for(int i = 0; i < strName.size(); i++)
		{
			hashVal = (37 * hashVal + strName[i]);// % tableSize;
			cout<<"\ni="<<i<<"; hashVal="<<hashVal;
			//getch();
		}
		cout<<"\n";
		
		return hashVal % tableSize;
	}
	
	template <typename HashObj>
	unsigned int HashTable<HashObj>::nextPrime(const int num)
	{
		int newPrime = num;
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
		std::cout<<"\nnewprime is "<<newPrime<<"\n";
		return newPrime;
	}
}

namespace ColRes
{
	//TODO: collision resolution methods
}


#endif /* HASH_H_ */
