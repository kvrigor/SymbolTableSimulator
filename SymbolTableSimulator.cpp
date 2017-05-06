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

const int MAX_TABLE_SIZE = 1000000;

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

void AddSymbolToList_Many(std::vector<Symbol> &);
void AddSymbolToHashTable_Many(Hash::HashTable<Symbol> &);
void SearchFromList_Many(std::vector<Symbol> &);
void SearchFromHashTable_Many(Hash::HashTable<Symbol> &);
void DeleteFromList_Many(std::vector<Symbol> &);
void DeleteFromHashTable_Many(Hash::HashTable<Symbol> &);

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

start:
	if (_PreloadDataset(datasetFile, symbolTableRaw))
	{
		std::list<Symbol> symbolTableCopy(symbolTableRaw);
		symbolTableCopy.sort(CompareSymbols);
		std::vector<Symbol> symbolTableSorted(symbolTableCopy.begin(), symbolTableCopy.end());

		char choice;
		do
		{
			ClearScreen();
			_PrintBanner();
			cout<<"  Binary search\n"
				<<"    [a] Insert X random elements\n"
				<<"    [s] Delete X elements\n"
				<<"    [d] Search X elements\n"
				<<"    [f] Print table\n\n"
				<<"  Hash table\n"
				<<"    [z] Initialize hash table\n"
				<<"    [x] Insert X random elements\n"
				<<"    [c] Delete X elements\n"
				<<"    [v] Search X elements\n"
				<<"    [b] Print table\n\n"
				<<"  Dataset options\n"
				<<"    [o] Reset dataset\n"
				<<"    [p] Reload dataset\n\n"
				<<"  Enter your choice ('q' to exit): ";
			cin>>choice;

			switch(choice)
			{
				//Binary search options
				case 'a': AddSymbolToList_Many(symbolTableSorted); break;
				case 's': DeleteFromList_Many(symbolTableSorted); break;
				case 'd': SearchFromList_Many(symbolTableSorted); break;
				case 'f': PrintSymbolTable(symbolTableSorted); break;

				//hash table options
				case 'z': CreateHashTable(symbolHashT, symbolTableCopy); break;
				case 'x': AddSymbolToHashTable_Many(symbolHashT); break;
				case 'c': DeleteFromHashTable_Many(symbolHashT); break;
				case 'v': SearchFromHashTable_Many(symbolHashT); break;
				case 'b': PrintHashTable(symbolHashT); break;

				case 'o':
					symbolTableSorted.clear();
					symbolTableSorted.assign(symbolTableCopy.begin(), symbolTableCopy.end());
					cout<<endl<<"  >> Dataset reset to "<<symbolTableSorted.size()<<" elements.";
					getch();
					break;
				case 'p':
					goto start;
			}
		} while(choice != 'q');
	}
	return 0;
}

// ----------------------- Main menu methods -----------------------
void CreateHashTable(Hash::HashTable<Symbol> & symbolHashTable, const std::list<Symbol> & symbolTable)
{
	ClearScreen();
	size_t tblsize, symCounts;
	do {
		cout<<"Specify initial table size (1 to 1,000,000): ";
		cin>>tblsize;
	} while(tblsize < 1 || tblsize > 1000000);
	symbolHashTable.Set(tblsize);
	symbolHashTable.SetHash2_R(tblsize / 10);
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
	char * outputFileName = "varlist_sorted.csv";
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
	char * outputFileName = "varhashtable.csv";

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

void AddSymbolToList_Many(std::vector<Symbol> & symbolTable)
{
	int numNewSymbols;
	Symbol newSymbol;
	std::vector<Symbol>  newSymbols;
	ClearScreen();
	cout<<"Enter number of random elements to insert: ";
	cin>>numNewSymbols;

	for (int i = 0; i < numNewSymbols; i++)
	{
		newSymbol.Name = GetRandomSymbolName();
		newSymbol.Type = GetRandomSymbolType();
		newSymbol.Scope = GetRandomSymbolScope();
		newSymbols.push_back(newSymbol);
	}

	SimpleTimer stopwatch(true);
	int numDuplicates = 0;
	for (int j = 0; j < numNewSymbols; j++)
	{
		if (!BinaryInsert(symbolTable, newSymbols[j]))
			numDuplicates++;
	}
	stopwatch.Pause();
	if (numDuplicates == 0)
		cout<<endl<<"Successfully added "<<numNewSymbols<<" elements.";
	else
		cout<<endl<<numDuplicates<<" randomly generated symbols already exist on the table. Only "<<numNewSymbols-numDuplicates<<" elements were added.";
	_PrintElapsedTime(stopwatch);
	cout<<endl<<"Updated total number of elements: "<<symbolTable.size();

	char choice;
	cout<<endl<<endl<<"Print symbol table? (y/n) ";
	cin>>choice;
	if (choice == 'y')
		PrintSymbolTable(symbolTable);
}

void SearchFromList_Many(std::vector<Symbol> & symbolTable)
{
	int numSymbolsToSearch;
	unsigned int symbolCount = symbolTable.size();
	std::list<string>  symbolsToSearch;
	ClearScreen();
	cout<<"Enter number of random elements to search: (1-"<<symbolCount<<") ";
	cin>>numSymbolsToSearch;
	if (numSymbolsToSearch < 1 or numSymbolsToSearch > symbolCount)
	{
		cout<<endl<<"Invalid value.";
	}
	else
	{
		SetConsoleBufferHeight(15 + numSymbolsToSearch);
		Symbol tempSymbol;
		cout<<endl<<"Symbols to search: "<<endl;
		for (int i = 0; i < numSymbolsToSearch; i++)
		{
			tempSymbol = symbolTable[GetRandomNumber() % symbolCount];
			symbolsToSearch.push_back(tempSymbol.Name);
			cout<<"   "<<tempSymbol.Name<<endl;
		}

		SimpleTimer stopwatch(true);
		for (std::list<string>::iterator it=symbolsToSearch.begin(); it != symbolsToSearch.end(); ++it)
			BinarySearch(symbolTable, *it, tempSymbol);
		cout<<endl<<"Successfully searched "<<numSymbolsToSearch<<" elements.";
		_PrintElapsedTime(stopwatch);
	}
	getch();
}

void DeleteFromList_Many(std::vector<Symbol> & symbolTable)
{
	int numSymbolsToDelete;
	unsigned int symbolCount = symbolTable.size();
	ClearScreen();
	cout<<"Enter number of random elements to delete: (1-"<<symbolCount<<") ";
	cin>>numSymbolsToDelete;
	if (numSymbolsToDelete < 1 or numSymbolsToDelete > symbolCount)
	{
		cout<<endl<<"Invalid value.";
		getch();
	}
	else
	{
		SetConsoleBufferHeight(15 + numSymbolsToDelete);
		std::vector<Symbol>  symbolsTableCopy(symbolTable.begin(),symbolTable.end());
		std::random_shuffle(symbolsTableCopy.begin(),symbolsTableCopy.end());
		cout<<endl<<"Symbols to delete: "<<endl;
		for (int i = 0; i < numSymbolsToDelete; i++)
			cout<<"   "<<symbolsTableCopy[i].Name<<endl;

		int numDeletes = 0;
		SimpleTimer stopwatch(true);
		for (int j = 0; j < numSymbolsToDelete; j++)
		{
			if (BinaryDelete(symbolTable, symbolsTableCopy[j].Name))
				numDeletes++;
		}
		stopwatch.Pause();
		if (numDeletes == numSymbolsToDelete)
			cout<<endl<<"Successfully deleted "<<numSymbolsToDelete<<" elements.";
		else
			cout<<endl<<"Found duplicates on the delete list. Only deleted "<<numDeletes<<" elements. ";
		_PrintElapsedTime(stopwatch);
		cout<<endl<<"Updated total number of elements: "<<symbolTable.size();

		char choice;
		cout<<endl<<endl<<"Print symbol table? (y/n) ";
		cin>>choice;
		if (choice == 'y')
			PrintSymbolTable(symbolTable);
	}
}

void AddSymbolToHashTable_Many(Hash::HashTable<Symbol> & symbolHashTable)
{
	int numNewSymbols;
	Symbol newSymbol;
	std::vector<Symbol>  newSymbols;
	ClearScreen();
	cout<<"Enter number of random elements to insert: ";
	cin>>numNewSymbols;

	for (int i = 0; i < numNewSymbols; i++)
	{
		newSymbol.Name = GetRandomSymbolName();
		newSymbol.Type = GetRandomSymbolType();
		newSymbol.Scope = GetRandomSymbolScope();
		newSymbols.push_back(newSymbol);
	}

	SimpleTimer stopwatch(true);
	int numDuplicates = 0;
	for (int j = 0; j < numNewSymbols; j++)
	{
		symbolHashTable.Insert(newSymbols[j]);
	}
	stopwatch.Pause();
	/*if (numDuplicates == 0)
		cout<<endl<<"Successfully added "<<numNewSymbols<<" elements.";
	else
		cout<<endl<<numDuplicates<<" randomly generated symbols already exist on the table. Only "<<numNewSymbols-numDuplicates<<" elements were added.";*/
	_PrintElapsedTime(stopwatch);

	char choice;
	cout<<endl<<"Print symbol table? (y/n) ";
	cin>>choice;
	if (choice == 'y')
		PrintHashTable(symbolHashTable);
}

void SearchFromHashTable_Many(Hash::HashTable<Symbol> & symbolHashTable)
{
	int numSymbolsToSearch;
	bool found;
	unsigned int symbolCount = symbolHashTable.activeCount();
	std::list<string>  symbolsToSearch;
	ClearScreen();
	cout<<"Enter number of random elements to search: (1-"<<symbolCount<<") ";
	cin>>numSymbolsToSearch;
	if (numSymbolsToSearch < 1 or numSymbolsToSearch > symbolCount)
	{
		cout<<endl<<"Invalid value.";
	}
	else
	{
		SetConsoleBufferHeight(15 + numSymbolsToSearch);
		Symbol tempSymbol;
		cout<<endl<<"Symbols to search: "<<endl;
		for (int i = 0; i < numSymbolsToSearch; i++)
		{
			do {
				tempSymbol = symbolHashTable[GetRandomNumber() % symbolHashTable.Size()];
			} while(tempSymbol.Name == "");
			symbolsToSearch.push_back(tempSymbol.Name);
			cout<<"   "<<tempSymbol.Name<<endl;
		}

		SimpleTimer stopwatch(true);
		for (std::list<string>::iterator it=symbolsToSearch.begin(); it != symbolsToSearch.end(); ++it)
			symbolHashTable.Retrieve(*it, found);
		cout<<endl<<"Successfully searched "<<numSymbolsToSearch<<" elements.";
		_PrintElapsedTime(stopwatch);
	}
	getch();
}

void DeleteFromHashTable_Many(Hash::HashTable<Symbol> & symbolHashTable)
{
	int numSymbolsToDelete;
	unsigned int symbolCount = symbolHashTable.activeCount();
	ClearScreen();
	cout<<"Enter number of random elements to delete: (1-"<<symbolCount<<") ";
	cin>>numSymbolsToDelete;
	if (numSymbolsToDelete < 1 or numSymbolsToDelete > symbolCount)
	{
		cout<<endl<<"Invalid value.";
		getch();
	}
	else
	{
		std::list<string>  symbolsToDelete;
		Symbol tempSymbol;
		SetConsoleBufferHeight(15 + numSymbolsToDelete);
		cout<<endl<<"Symbols to delete: "<<endl;
		for (int i = 0; i < numSymbolsToDelete; i++)
		{
			do {
				tempSymbol = symbolHashTable[GetRandomNumber() % symbolHashTable.Size()];
			} while(tempSymbol.Name == "");
			symbolsToDelete.push_back(tempSymbol.Name);
			cout<<"   "<<tempSymbol.Name<<endl;
		}

		int numDeletes = 0;
		SimpleTimer stopwatch(true);
		for (std::list<string>::iterator it=symbolsToDelete.begin(); it != symbolsToDelete.end(); ++it)
		{
			if (symbolHashTable.Delete(*it))
				numDeletes++;
		}
		stopwatch.Pause();
		if (numDeletes == numSymbolsToDelete)
			cout<<endl<<"Successfully deleted "<<numSymbolsToDelete<<" elements.";
		else
			cout<<endl<<"Found duplicates on the delete list. Only deleted "<<numDeletes<<" elements. ";
		_PrintElapsedTime(stopwatch);
		cout<<endl<<"Updated total number of elements: "<<symbolHashTable.activeCount();

		char choice;
		cout<<endl<<endl<<"Print symbol table? (y/n) ";
		cin>>choice;
		if (choice == 'y')
			PrintHashTable(symbolHashTable);
	}
}

// ----------------------- Misc. helper methods -----------------------
void _PrintBanner()
{
	cout<<endl;
	cout<<" ========================================="<<endl<<endl
        <<"  SYMBOL TABLE SIMULATOR (Benchmark mode) "<<endl<<endl
        <<" ========================================="<<endl<<endl;
}

void _PrintElapsedTime(SimpleTimer & stopwatch)
{
	cout<<"\nTime taken: "<<stopwatch.Elapsed_ms_str();
}

bool _PreloadDataset(char* fileName, std::list<Symbol> & table)
{
	int tableSize;
	ClearScreen();
	cout<<"Enter desired table size: (1-"<<MAX_TABLE_SIZE<<") ";
	cin>>tableSize;
	if (tableSize < 1 or tableSize > MAX_TABLE_SIZE)
	{
		cout<<endl<<"Invalid table size. Exiting program...";
		getch();
		return false;
	}
	else
	{
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
			myfile.close();
			cout<<"success! Operation took "<<stopwatch.Elapsed_ms_str();

			char * outputFileName = "varlist_raw.csv";
			if (_SaveListToFile(outputFileName,table))
				cout<<endl<<"\nSymbol table exported to "<<outputFileName;
			else
				cout<<"\nSymbol table not successfully exported to file.";
			getch();
			return true;
		}
		else
		{
			cout <<"Unable to open dataset file "<<fileName;
			getch();
			return false;
		}
	}
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





