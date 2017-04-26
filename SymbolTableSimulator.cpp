//============================================================================
// Name        : SymbolTableSimulator.cpp
// Author      : chester.tan, paul.rigor
// Version     : 0.1
// Description : Simulates symbol table operations
//				 using hashing and binary search.
//============================================================================

#include <iostream>
#include <conio.h>
#include "search.h"
#include "utils.h"
#include "hash.h"

using std::cout;
using std::cin;
using std::endl;
using namespace BinarySearch;


void PrintBanner();
void LoadDatasetToList();
void LoadDatasetToHashtable();
void AddSymbolToList();
void AddSymbolToHashtable();
void SearchFromList();
void SearchFromHashtable();

int main()
{
	char choice;
	do
	{
		ClearScreen();
		PrintBanner();
		cout<<"  [1] Load dataset to list\n"
			<<"  [2] Load dataset to hashtable\n"
			<<"  [3] Add new symbol to list\n"
			<<"  [4] Add new symbol to hashtable\n"
			<<"  [5] Search symbol from list using binary search\n"
			<<"  [6] Search symbol from hash table\n"
			<<"  [7] Exit\n"
			<<"  Enter your choice: ";
		cin>>choice;

		switch(choice)
		{
		case '1':
			LoadDatasetToList();
			break;
		case '2':
			LoadDatasetToHashtable();
			break;
		case '3':
			AddSymbolToList();
			break;
		case '4':
			AddSymbolToHashtable();
			break;
		case '5':
			SearchFromList();
			break;
		case '6':
			SearchFromHashtable();
			break;
		}
	}
	while(choice != '7');
	return 0;
}

void LoadDatasetToList()
{
	ClearScreen();
	//TODO
	cout<<"LoadDatasetToList -- work in progress...";
	getch();
}

void LoadDatasetToHashtable()
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

void AddSymbolToHashtable()
{
	ClearScreen();
	//TODO
	cout<<"AddSymbolToHashtable -- work in progress...";
	getch();
}

void SearchFromList()
{
	ClearScreen();
	//TODO
	cout<<"SearchFromList -- work in progress...";
	getch();
}

void SearchFromHashtable()
{
	ClearScreen();
	//TODO
	cout<<"SearchFromHashtable -- work in progress...";
	getch();
}

void PrintBanner()
{
	cout<<endl<<endl;
	cout<<" =========================="<<endl<<endl
        <<"   SYMBOL TABLE SIMULATOR  "<<endl<<endl
        <<" =========================="<<endl<<endl;
}

