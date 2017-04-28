
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
			
			void Set(int);
			void MakeEmpty();
			int Size();
			bool Insert(const HashObj &);
			HashObj Retrieve(const string, bool &);
			
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
			int hash(const string &);
			int hash2(const string &, int);
			unsigned int HornersHash(const string, int);
			
	};
	
	template <typename HashObj>
	HashTable<HashObj>::HashTable(int size) : _symVector(nextPrime(size))
	{ MakeEmpty(); _tblSize = _symVector.size();}
	
	template <typename HashObj>
	void HashTable<HashObj>::Set(int size)
	{
		_symVector.resize(nextPrime(size));
		MakeEmpty();
		_tblSize = _symVector.size();
	}
	
	template <typename HashObj>
	void HashTable<HashObj>::MakeEmpty()
	{
		for(int i = 0; i < _symVector.size(); i++)
			_symVector[i].info = EMPTY;
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
		int index = hash(temp.Name);
		
		_symVector[index].element = entry;
		_symVector[index].info = ACTIVE;
		cout<<"\nInserted: "<<temp.ToString();
		//rehash if need
		
		
		return true;
	}
	
	template <typename HashObj>
	HashObj HashTable<HashObj>::Retrieve(const string key, bool & isFound)
	{
		int index = hash(key);
		HashValue retObj = _symVector[index];
		cout<<"\nAsked: "<<key<<";index: "<<index;
		cout<<"\nRetrieved:\n"<<retObj.element.Name;
		if (retObj.info != ACTIVE)
			isFound = false;
		else
			isFound = true;
		
		return retObj.element;
	}
	
	template <typename HashObj>
	int HashTable<HashObj>::hash(const string & sym)
	{
		string temp = sym;
		string strHash = temp;
		
		int currentPos = HornersHash(temp, _tblSize);
		int offset = hash2(temp, 23);
		
		cout<<"\ncurrentPos: "<<currentPos<<"\noffset: "<<offset<<"\n";
		//getch();
		
		while(_symVector[currentPos].info != EMPTY && _symVector[currentPos].element.Name != temp)
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
	int HashTable<HashObj>::hash2(const string & sym, int R)
	{
		unsigned int hashVal = 0;
		string strName = sym;
		for(int i = 0; i < strName.size(); i++)
			hashVal = 37 * hashVal + strName[i];
			
		return R - (hashVal % R);
	}
	
	template <typename HashObj>
	unsigned int HashTable<HashObj>::HornersHash(const string sym, int tableSize)
	{
		unsigned int hashVal = 0;
		
		string strName = sym;
		
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
