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
void CreateHashTable(Hash::HashTable<Symbol> &, const std::list<Symbol> &);
void AddSymbolToList(std::vector<Symbol> &);
void AddSymbolToHashTable(Hash::HashTable<Symbol> &);
void SearchFromList(std::vector<Symbol> &);
void SearchFromHashTable(Hash::HashTable<Symbol> &);
void DeleteFromList(std::vector<Symbol> &);
void DeleteFromHashTable(Hash::HashTable<Symbol> &);
void PrintSymbolTable(std::list<Symbol> &);
void PrintSymbolTable(std::vector<Symbol> &);
void PrintHashTable(Hash::HashTable<Symbol> &);
void _PrintBanner();
bool _PreloadDataset(char *, std::list<Symbol> &);
bool _SaveListToFile(char *, std::list<Symbol> &);
bool _SaveListToFile(char *, std::vector<Symbol> &);
void _PrintElapsedTime(SimpleTimer &);
Symbol _ExtractSymbol(string);

// -------------------------------------------------------------
// ------------------------    MAIN    -------------------------
// -------------------------------------------------------------
int main()
{
	std::list<Symbol> symbolTableRaw;
	Hash::HashTable<Symbol> symbolHashT;

	char * datasetFile = "symbol_table_1M.csv";
	if (!_PreloadDataset(datasetFile, symbolTableRaw))
	{
		cout << "Unable to open file "<<datasetFile;
		getch();
	}
	else
	{
		std::list<Symbol> symbolTableCopy(symbolTableRaw);
		symbolTableCopy.sort(CompareSymbols);
		std::vector<Symbol> symbolTableSorted(symbolTableCopy.begin(), symbolTableCopy.end());


		char choice;
		do
		{
			ClearScreen();
			_PrintBanner();
			cout<<"  [1] Create hash table from dataset\n"
				<<"  [2] Add new symbol to list\n"
				<<"  [3] Add new symbol to hash table\n"
				<<"  [4] Search symbol from sorted list using binary search\n"
				<<"  [5] Search symbol from hash table\n"
				<<"  [6] Delete symbol from list\n"
				<<"  [7] Delete symbol from hash table\n"
				<<"  [8] Print symbol table\n"
				<<"  [9] Exit\n"
				<<"  Enter your choice: ";
			cin>>choice;

			switch(choice)
			{
			case '1': CreateHashTable(symbolHashT, symbolTableCopy); break;
			case '2': AddSymbolToList(symbolTableSorted); break;
			case '3': AddSymbolToHashTable(symbolHashT); break;
			case '4': SearchFromList(symbolTableSorted); break;
			case '5': SearchFromHashTable(symbolHashT); break;
			case '6': DeleteFromList(symbolTableSorted); break;
			case '7': DeleteFromHashTable(symbolHashT); break;
			case '8':
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
					case 'b': PrintSymbolTable(symbolTableSorted); break;
					case 'c': PrintHashTable(symbolHashT); break;
					}
				} while(subChoice != 'd');
				break;
			}
		} while(choice != '9');
	}
	return 0;
}

// ----------------------- Main menu methods -----------------------
void CreateHashTable(Hash::HashTable<Symbol> & symbolHashTable, const std::list<Symbol> & symbolTable)
{
	ClearScreen();
	size_t tblsize, symCounts;
	int hash2R;
	do {
		cout<<"Specify initial table size (1 to 1,000,000): ";
		cin>>tblsize;
	} while(tblsize < 1 || tblsize > 1000000);
	symbolHashTable.Set(tblsize);
	do {
		cout<<"Specify hash2 function's R value (1 to "<<tblsize * 0.9<<", use 0 for auto): ";
		cin>>hash2R;
	} while(hash2R < 0 || hash2R > tblsize * 0.9);
	if (hash2R == 0)
		hash2R = tblsize / 10;
	symbolHashTable.SetHash2_R(hash2R);
	do {
		cout<<"Specify input array size (1 to 1,000,000): ";
		cin>>symCounts;
	} while(symCounts < 1 || symCounts > 1000000);
	std::vector<Symbol> symbolVTbl(symbolTable.begin(),symbolTable.end());
	SimpleTimer stopwatch(true);
	for(int i = 0; i < symCounts; i++)
		symbolHashTable.Insert(symbolVTbl[i]);
	_PrintElapsedTime(stopwatch);
	cout<<"\nhash2_R: "<<symbolHashTable.GetHash2_R()<<"\n";
	cout<<"table size: "<<symbolHashTable.Size()<<"\n";
	cout<<"collision: "<<symbolHashTable.Collision()<<"\n";
	
	cout<<"\nTotal symbols added: "<<symbolHashTable.activeCount()<<"\n";
	getch();
}

void AddSymbolToList(std::vector<Symbol> & symbolTable)
{
	Symbol newSymbol, searchResult;
	SimpleTimer stopwatch;
	char choice;
	do
	{
		ClearScreen();
		cout<<"Symbol name: ";
		cin>>newSymbol.Name;

		if(BinarySearch(symbolTable, newSymbol.Name, searchResult))
			cout<<endl<<"Symbol '"<<newSymbol.Name<<"' already exists.";
		else
		{
			stopwatch.Pause();
			cout<<"       Type: ";
			cin>>newSymbol.Type;
			cout<<"      Scope: ";
			cin>>newSymbol.Scope;
			stopwatch.Restart();
			if (BinaryInsert(symbolTable, newSymbol))
			{
				cout<<endl<<"Symbol '"<<newSymbol.Name<<"' successfully added to the list.";
				_PrintElapsedTime(stopwatch);
			}
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
	SimpleTimer stopwatch;
	do
	{
		ClearScreen();
		cout<<"** Adding new Symbol to Hash Table **\n";
		cout<<"Symbol name: ";
		cin>>newSymbol.Name;
		stopwatch.Restart();
		symbolHashTable.Retrieve(newSymbol.Name, alreadyExist);		
		if(alreadyExist)
		{
			cout<<endl<<"Symbol '"<<newSymbol.Name<<"' already exists.";
			_PrintElapsedTime(stopwatch);
		}
		else
		{
			stopwatch.Pause();
			cout<<"       Type: ";
			cin>>newSymbol.Type;
			cout<<"      Scope: ";
			cin>>newSymbol.Scope;
			stopwatch.Start();
			symbolHashTable.Insert(newSymbol);
			cout<<endl<<"Symbol '"<<newSymbol.Name<<"' successfully added to the Hash list.";
			_PrintElapsedTime(stopwatch);
		}
		cout<<endl<<endl<<"Try to add another symbol? (y/n) ";
		cin>>choice;
	} while (choice == 'y' || choice == 'Y');
}

void SearchFromList(std::vector<Symbol> & symbolTable)
{
	string symbolName;
	Symbol result;
	char choice;
	SimpleTimer stopwatch;
	do
	{
		ClearScreen();
		cout<<"Enter symbol name to search: ";
		cin>>symbolName;
		stopwatch.Restart();
		bool found = BinarySearch(symbolTable, symbolName, result);
		stopwatch.Pause();
		if (found)
		{
			cout<<endl<<endl;
			cout<<"\tSymbol name: "<<result.Name<<endl;
			cout<<"\t       Type: "<<result.Type<<endl;
			cout<<"\t      Scope: "<<result.Scope<<endl;
		}
		else
			cout<<endl<<"Symbol '"<<symbolName<<"' not found.";
		_PrintElapsedTime(stopwatch);
		cout<<endl<<endl<<"Perform another search? (y/n) ";
		cin>>choice;
	} while (choice == 'y' || choice == 'Y');
}

void SearchFromHashTable(Hash::HashTable<Symbol> & symbolHashTable)
{
	string symbolName;
	Symbol result;
	SimpleTimer stopwatch;
	char choice;
	bool isFound;
	do
	{
		ClearScreen();
		cout<<"** SearchFromHashTable **\n";
		cout<<"\nsize: "<<symbolHashTable.Size()<<"\n";
		cout<<"Enter symbol name to search: ";
		cin>>symbolName;
		stopwatch.Start();
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
		_PrintElapsedTime(stopwatch);
		cout<<endl<<endl<<"Perform another search? (y/n) ";
		cin>>choice;
	} while (choice == 'y' || choice == 'Y');
}

void DeleteFromList(std::vector<Symbol> & symbolTable)
{
	string symbolName;
	char choice;
	SimpleTimer stopwatch;
	do
	{
		ClearScreen();
		cout<<"Enter symbol name to delete: ";
		cin>>symbolName;
		stopwatch.Restart();
		bool deleted = BinaryDelete(symbolTable, symbolName);
		stopwatch.Pause();
		if (deleted)
			cout<<endl<<"Symbol '"<<symbolName<<"' successfully deleted.";
		else
			cout<<endl<<"Symbol '"<<symbolName<<"' not found.";
		_PrintElapsedTime(stopwatch);
		cout<<endl<<endl<<"Delete another symbol? (y/n) ";
		cin>>choice;
	} while (choice == 'y' || choice == 'Y');
}

void DeleteFromHashTable(Hash::HashTable<Symbol> & symbolHashTable)
{
	string symbolName;
	char choice;
	SimpleTimer stopwatch;
	do
	{
		ClearScreen();
		cout<<"Enter symbol name to delete: ";
		cin>>symbolName;
		stopwatch.Restart();
		bool deleted = symbolHashTable.Delete(symbolName);
		stopwatch.Pause();
		if (deleted)
			cout<<endl<<"Symbol '"<<symbolName<<"' successfully deleted.";
		else
			cout<<endl<<"Symbol '"<<symbolName<<"' not found.";
		_PrintElapsedTime(stopwatch);
		cout<<endl<<endl<<"Delete another symbol? (y/n) ";
		cin>>choice;
	} while (choice == 'y' || choice == 'Y');
}

void PrintSymbolTable(std::list<Symbol> & symbolTable)
{
	ClearScreen();
	unsigned int symbolCount = symbolTable.size();
	bool savedToFile;
	char * outputFileName = "list_raw.csv";
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
		savedToFile = _SaveListToFile(outputFileName,symbolTable);
	}
	if (savedToFile)
		cout<<"\n\nOutput saved to "<<outputFileName;
	else
		cout<<"\n\nOutput not successfully saved to file";
	getch();
}

void PrintSymbolTable(std::vector<Symbol> & symbolTable)
{
	ClearScreen();
	unsigned int symbolCount = symbolTable.size();
	bool savedToFile;
	char * outputFileName = "list_sorted.csv";
	cout<<"Total number of symbols: "<<symbolCount<<endl<<endl;
	if (symbolCount > 0)
	{
		SetConsoleBufferHeight(15 + symbolCount);
		cout<<endl
			<<std::left<<std::setw(20)<<std::setfill(' ')<<"Symbol Name"
			<<std::left<<std::setw(18)<<std::setfill(' ')<<"Type"
			<<std::left<<std::setw(15)<<std::setfill(' ')<<"Scope"<<endl<<endl;
		for (std::vector<Symbol>::iterator it=symbolTable.begin(); it != symbolTable.end(); ++it)
		{
			cout<<endl
				<<std::left<<std::setw(20)<<std::setfill(' ')<<it->Name
				<<std::left<<std::setw(18)<<std::setfill(' ')<<it->Type
				<<std::left<<std::setw(15)<<std::setfill(' ')<<it->Scope;
		}
		savedToFile = _SaveListToFile(outputFileName,symbolTable);
	}
	if (savedToFile)
		cout<<"\n\nOutput saved to "<<outputFileName;
	else
		cout<<"\n\nOutput not successfully saved to file";
	getch();
}

void PrintHashTable(Hash::HashTable<Symbol> & symbolHashTable)
{
	ClearScreen();
	unsigned int symbolCount = symbolHashTable.Size();
	bool savedToFile;
	char * outputFileName = "hashtable.csv";

	cout<<"Total number of symbols: "<<symbolCount<<endl<<endl;
	if (symbolCount > 0)
	{
		SetConsoleBufferHeight(15 + symbolCount);
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
		savedToFile = _SaveListToFile(outputFileName, tempList);
	}
	cout<<"\n\nTotal used slot: "<<symbolHashTable.activeCount()<<endl;
	if (savedToFile)
		cout<<"Output saved to "<<outputFileName;
	else
		cout<<"Output not successfully saved to file";
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

void _PrintElapsedTime(SimpleTimer & stopwatch)
{
	cout<<"\nTime taken: "<<stopwatch.Elapsed_ms_str();
}

bool _PreloadDataset(char* fileName, std::list<Symbol> & table)
{
	int tableSize;
	do
	{
		ClearScreen();
		cout<<"Enter desired table size: (1-1000000) ";
		cin>>tableSize;
		if (tableSize < 1 or tableSize > 1000000)
		{
			cout<<endl<<"Invalid table size.";
			getch();
		}
	}while(tableSize < 1 or tableSize > 1000000);

	cout<<endl<<"Loading dataset...";
	SimpleTimer stopwatch(true);
	std::ifstream myfile (fileName);
	if (myfile.good())
	{
		string line;
		std::getline(myfile, line); //ignore header
		for (int i = 0; i < tableSize; i++)
		{
			std::getline(myfile, line);
			table.push_back(_ExtractSymbol(line));
		}
//
//		while(std::getline(myfile, line))
//			table.push_back(_ExtractSymbol(line));
		myfile.close();
		cout<<endl<<"Success! Operation took "<<stopwatch.Elapsed_ms_str();
		getch();
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

bool _SaveListToFile(char* fileName, std::list<Symbol> & symbolTable)
{
	std::ofstream myfile(fileName, std::ofstream::out | std::ofstream::trunc);
	 if (myfile.is_open())
	 {
		 myfile<<"Symbol Name"<<","<<"Type"<<","<<"Scope"<<endl;
		 for (std::list<Symbol>::iterator it=symbolTable.begin(); it != symbolTable.end(); ++it)
			 myfile<<it->Name<<","<<it->Type<<","<<it->Scope<<endl;
		 myfile.close();
		 return true;
	 }
	 else
		 return false;
}

bool _SaveListToFile(char* fileName, std::vector<Symbol> & symbolTable)
{
	std::ofstream myfile(fileName, std::ofstream::out | std::ofstream::trunc);
	 if (myfile.is_open())
	 {
		 myfile<<"Symbol Name"<<","<<"Type"<<","<<"Scope"<<endl;
		 for (std::vector<Symbol>::iterator it=symbolTable.begin(); it != symbolTable.end(); ++it)
			 myfile<<it->Name<<","<<it->Type<<","<<it->Scope<<endl;
		 myfile.close();
		 return true;
	 }
	 else
		 return false;
}





