//============================================================================
// Name        : SymbolTableSimulator.cpp
// Author      : chester.tan, paul.rigor
// Version     : 0.1
// Description : Simulates symbol table operations
//				 using hashing and binary search.
//============================================================================

#include <iostream>
#include <fstream>
#include <conio.h>
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
void CreateHashTable();
void AddSymbolToList();
void AddSymbolToHashTable();
void SearchFromList(std::list<Symbol> &);
void SearchFromHashTable();
void PrintSymbolTable(std::list<Symbol> &);
void PrintHashTable();
void _PrintBanner();
bool _PreloadDataset(char *, std::list<Symbol> &);
Symbol _ExtractSymbol(string);

// -------------------------------------------------------------
// ------------------------    MAIN    -------------------------
// -------------------------------------------------------------
int main()
{
	std::list<Symbol> symbolTableRaw;

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
			case '2': CreateHashTable(); break;
			case '3': AddSymbolToList(); break;
			case '4': AddSymbolToHashTable(); break;
			case '5': SearchFromList(symbolTableCopy); break;
			case '6': SearchFromHashTable(); break;
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
					case 'c': PrintHashTable(); break;
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

void CreateHashTable()
{
	ClearScreen();
	//TODO
	cout<<"LoadDatasetToHashtable -- work in progress...";
	getch();
}

void AddSymbolToList()
{
	ClearScreen();
	//TODO
	cout<<"AddSymbolToList -- work in progress...";
	getch();
}

void AddSymbolToHashTable()
{
	ClearScreen();
	//TODO
	cout<<"AddSymbolToHashtable -- work in progress...";
	getch();
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

void SearchFromHashTable()
{
	ClearScreen();
	//TODO

	cout<<"SearchFromHashtable -- work in progress...";
	getch();
}

void PrintSymbolTable(std::list<Symbol> & symbolTable)
{
	ClearScreen();
	unsigned int symbolCount = symbolTable.size();
	cout<<"Total number of symbols: "<<symbolCount<<endl<<endl;
	if (symbolCount > 0)
	{
		SetConsoleBufferHeight(5 + symbolCount);

		//TODO: align table columns properly
		cout<<"Symbol Name"<<"\t"<<"Type"<<"\t"<<"Scope"<<endl<<endl;
		for (std::list<Symbol>::iterator it=symbolTable.begin(); it != symbolTable.end(); ++it)
			cout<<it->ToString()<<"\n";
	}
	getch();
}

void PrintHashTable()
{
	ClearScreen();
	//TODO
	cout<<"PrintHashTable -- work in progress...";
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
//		for (line; std::getline(myfile, line); )
//		{
//			table.push_back(_ExtractSymbol(line));
//		}
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


