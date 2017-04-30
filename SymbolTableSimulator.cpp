//============================================================================
// Name        : SymbolTableSimulator.cpp
// Authors     : chester.tan, paul.rigor
// Version     : v0.2 [Beta]
// Description : Simulates symbol table operations using hashing and binary search.
// Git repo    : https://github.com/kvrigor/SymbolTableSimulator
//============================================================================

#include <iostream>
#include <fstream>
#include <conio.h>
#include <stdio.h>
#include <iomanip>
#include "utils.h"
#include "hash.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using namespace CustomTypes;
using namespace SearchAlgorithms;

// -----------------------  prototypes -----------------------
void SortSymbolTable(std::list<Symbol> &);
void CreateHashTable(Hash::HashTable<Symbol> &, const std::list<Symbol> &);
void AddSymbolToList(std::list<Symbol> &, std::list<Symbol> &);
void AddSymbolToHashTable(Hash::HashTable<Symbol> &);
void SearchFromList(std::list<Symbol> &);
void SearchFromHashTable(Hash::HashTable<Symbol> &);
void PrintSymbolTable(std::list<Symbol> &);
void PrintHashTable(Hash::HashTable<Symbol> &);
void _PrintBanner();
bool _PreloadDataset(char *, std::list<Symbol> &);
Symbol _ExtractSymbol(string);

// -------------------------------------------------------------
// ------------------------    MAIN    -------------------------
// -------------------------------------------------------------
int main()
{
	std::list<Symbol> symbolTableRaw;
	Hash::HashTable<Symbol> symbolHashT;

	char * datasetFile = "symbol_table_mock1.csv";
	if (!_PreloadDataset(datasetFile, symbolTableRaw))
	{
		cout << "Unable to open file "<<datasetFile;
		getch();
	}
	else
	{
		std::list<Symbol> symbolTableCopy(symbolTableRaw);
		char choice;
		do
		{
			ClearScreen();
			_PrintBanner();
			cout<<"  [1] Create sorted list from dataset\n"
				<<"  [2] Create hash table from dataset\n"
				<<"  [3] Add new symbol to list\n"
				<<"  [4] Add new symbol to hash table\n"
				<<"  [5] Search symbol from sorted list using binary search\n"
				<<"  [6] Search symbol from hash table\n"
				<<"  [7] Print symbol table\n"
				<<"  [8] Exit\n"
				<<"  Enter your choice: ";
			cin>>choice;

			switch(choice)
			{
			case '1': SortSymbolTable(symbolTableCopy); break;
			case '2': CreateHashTable(symbolHashT, symbolTableCopy); break;
			case '3': AddSymbolToList(symbolTableRaw, symbolTableCopy); break;
			case '4': AddSymbolToHashTable(symbolHashT); break;
			case '5': SearchFromList(symbolTableCopy); break;
			case '6': SearchFromHashTable(symbolHashT); break;
			case '7':
				char subChoice;
				do
				{
					ClearScreen();
					cout<<endl<<endl;
					cout<<"  [a] Print raw symbol table\n"
						<<"  [b] Print sorted symbol table\n"
						<<"  [c] Print hash table \n"
						<<"  [d] Back to main menu\n"
						<<"  Enter your choice: ";
					cin>>subChoice;
					switch(subChoice)
					{
					case 'a': PrintSymbolTable(symbolTableRaw); break;
					case 'b': PrintSymbolTable(symbolTableCopy); break;
					case 'c': PrintHashTable(symbolHashT); break;
					}
				} while(subChoice != 'd');
				break;
			}
		} while(choice != '8');
	}
	return 0;
}

// ----------------------- Main menu methods -----------------------
void SortSymbolTable(std::list<Symbol> & symbolTable)
{
	ClearScreen();
	symbolTable.sort(CompareSymbols);
	cout<<"Sorting done. Display sorted symbol table? (y/n) ";
	char choice;
	cin>>choice;
	if (choice == 'y' || choice == 'Y')
		PrintSymbolTable(symbolTable);
}

void CreateHashTable(Hash::HashTable<Symbol> & symbolHashTable, const std::list<Symbol> & symbolTable)
{
	ClearScreen();
	symbolHashTable.Set(symbolTable.size());
	std::vector<Symbol> symbolVTbl(symbolTable.begin(),symbolTable.end());
	for(int i = 0; i < symbolTable.size(); i++)
	{
		symbolHashTable.Insert(symbolVTbl[i]);
	}
	
	cout<<"\nTotal symbols added: "<<symbolHashTable.Size()<<"\n";
	getch();
}

void AddSymbolToList(std::list<Symbol> & rawSymbolTable, std::list<Symbol> & sortedSymbolTable)
{
	Symbol newSymbol, searchResult;
	char choice;
	do
	{
		ClearScreen();
		cout<<"Symbol name: ";
		cin>>newSymbol.Name;
		if(BinarySearch(sortedSymbolTable, newSymbol.Name, searchResult))
			cout<<endl<<"Symbol '"<<newSymbol.Name<<"' already exists.";
		else
		{
			cout<<"       Type: ";
			cin>>newSymbol.Type;
			cout<<"      Scope: ";
			cin>>newSymbol.Scope;
			//stuff below is not so efficient.. we'll make things work for now
			rawSymbolTable.push_back(newSymbol);
			sortedSymbolTable.push_back(newSymbol);
			sortedSymbolTable.sort(CompareSymbols);
			cout<<endl<<"Symbol '"<<newSymbol.Name<<"' successfully added to the list.";
		}
		cout<<endl<<endl<<"Try to add another symbol? (y/n) ";
		cin>>choice;
	} while (choice == 'y' || choice == 'Y');
}

void AddSymbolToHashTable(Hash::HashTable<Symbol> & symbolHashTable)
{
	Symbol newSymbol, searchResult;
	char choice;
	bool alreadyExist;
	do
	{
		ClearScreen();
		cout<<"** Adding new Symbol to Hash Table **\n";
		cout<<"Symbol name: ";
		cin>>newSymbol.Name;
		symbolHashTable.Retrieve(newSymbol.Name, alreadyExist);		
		if(alreadyExist)
			cout<<endl<<"Symbol '"<<newSymbol.Name<<"' already exists.";
		else
		{
			cout<<"       Type: ";
			cin>>newSymbol.Type;
			cout<<"      Scope: ";
			cin>>newSymbol.Scope;
			symbolHashTable.Insert(newSymbol);
			cout<<endl<<"Symbol '"<<newSymbol.Name<<"' successfully added to the Hash list.";
		}
		cout<<endl<<endl<<"Try to add another symbol? (y/n) ";
		cin>>choice;
	} while (choice == 'y' || choice == 'Y');
}

void SearchFromList(std::list<Symbol> & symbolTable)
{
	string symbolName;
	Symbol result;
	char choice;
	do
	{
		ClearScreen();
		cout<<"Enter symbol name to search: ";
		cin>>symbolName;
		bool found = BinarySearch(symbolTable, symbolName, result);
		if (found)
		{
			cout<<endl<<endl;
			cout<<"\tSymbol name: "<<result.Name<<endl;
			cout<<"\t       Type: "<<result.Type<<endl;
			cout<<"\t      Scope: "<<result.Scope<<endl;
		}
		else
			cout<<endl<<"Symbol '"<<symbolName<<"' not found.";

		cout<<endl<<endl<<"Perform another search? (y/n) ";
		cin>>choice;
	} while (choice == 'y' || choice == 'Y');
}

void SearchFromHashTable(Hash::HashTable<Symbol> & symbolHashTable)
{
	string symbolName;
	Symbol result;
	char choice;
	bool isFound;
	do
	{
		ClearScreen();
		cout<<"** SearchFromHashTable **\n";
		cout<<"\nsize: "<<symbolHashTable.Size()<<"\n";
		cout<<"Enter symbol name to search: ";
		cin>>symbolName;
		result = symbolHashTable.Retrieve(symbolName, isFound);
		if (isFound)
		{
			cout<<endl<<endl;
			cout<<"\tSymbol name: "<<result.Name<<endl;
			cout<<"\t       Type: "<<result.Type<<endl;
			cout<<"\t      Scope: "<<result.Scope<<endl;
		}
		else
			cout<<endl<<"Symbol '"<<symbolName<<"' not found.";

		cout<<endl<<endl<<"Perform another search? (y/n) ";
		cin>>choice;
	} while (choice == 'y' || choice == 'Y');
}

void PrintSymbolTable(std::list<Symbol> & symbolTable)
{
	ClearScreen();
	unsigned int symbolCount = symbolTable.size();
	cout<<"Total number of symbols: "<<symbolCount<<endl<<endl;
	if (symbolCount > 0)
	{
		SetConsoleBufferHeight(5 + symbolCount);

		cout<<endl
			<<std::left<<std::setw(20)<<std::setfill(' ')<<"Symbol Name"
			<<std::left<<std::setw(18)<<std::setfill(' ')<<"Type"
			<<std::left<<std::setw(15)<<std::setfill(' ')<<"Scope"<<endl<<endl;
		for (std::list<Symbol>::iterator it=symbolTable.begin(); it != symbolTable.end(); ++it)
		{
			cout<<endl
				<<std::left<<std::setw(20)<<std::setfill(' ')<<it->Name
				<<std::left<<std::setw(18)<<std::setfill(' ')<<it->Type
				<<std::left<<std::setw(15)<<std::setfill(' ')<<it->Scope;
		}
	}
	getch();
}

void PrintHashTable(Hash::HashTable<Symbol> & symbolHashTable)
{
	ClearScreen();
	unsigned int symbolCount = symbolHashTable.Size();
	cout<<"Total number of symbols: "<<symbolCount<<endl<<endl;
	if (symbolCount > 0)
	{
		SetConsoleBufferHeight(5 + symbolCount);
		std::list<Symbol> tempList = symbolHashTable.GetList();
		int index = 0;

		cout<<endl
			<<std::left<<std::setw(7)<<std::setfill(' ')<<"index"
			<<std::left<<std::setw(20)<<std::setfill(' ')<<"Symbol Name"
			<<std::left<<std::setw(18)<<std::setfill(' ')<<"Type"
			<<std::left<<std::setw(15)<<std::setfill(' ')<<"Scope"<<endl<<endl;
		for (std::list<Symbol>::iterator it=tempList.begin(); it != tempList.end(); ++it)
		{
			cout<<endl
				<<std::left<<std::setw(7)<<std::setfill(' ')<<index
				<<std::left<<std::setw(20)<<std::setfill(' ')<<it->Name
				<<std::left<<std::setw(18)<<std::setfill(' ')<<it->Type
				<<std::left<<std::setw(15)<<std::setfill(' ')<<it->Scope;
			index++;
		}
	}
	cout<<"\n\nTotal used slot: "<<symbolHashTable.activeCount()<<endl;
	cout<<endl;
	getch();
}

// ----------------------- Misc. helper methods -----------------------
void _PrintBanner()
{
	cout<<endl<<endl;
	cout<<" =========================="<<endl<<endl
        <<"   SYMBOL TABLE SIMULATOR  "<<endl<<endl
        <<" =========================="<<endl<<endl;
}

bool _PreloadDataset(char* fileName, std::list<Symbol> & table)
{
	std::ifstream myfile (fileName);
	if (myfile.good())
	{
		string line;
		std::getline(myfile, line); //ignore header
		while(std::getline(myfile, line))
			table.push_back(_ExtractSymbol(line));
		return true;
	}
	else
		return false;
}

Symbol _ExtractSymbol(string line)
{
	Symbol newSymbol;
	std::size_t comma1, comma2;
	comma1 = line.find(",");
	comma2 = line.rfind(",");
	newSymbol.Name = line.substr(0, comma1);
	newSymbol.Type = line.substr(comma1 + 1, comma2 - comma1 - 1);
	newSymbol.Scope = line.substr(comma2 + 1);
	return newSymbol;
}


