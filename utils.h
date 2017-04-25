/*
 * utils.h
 *
 *  Created on: Apr 15, 2017
 *      Author: paul.rigor
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <windows.h>


// Source copied from: http://www.cplusplus.com/articles/4z18T05o/#Windows
void ClearScreen()
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
			hStdOut,
			(TCHAR) ' ',
			cellCount,
			homeCoords,
			&count
	)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
			hStdOut,
			csbi.wAttributes,
			cellCount,
			homeCoords,
			&count
	)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition( hStdOut, homeCoords );
}

void SetConsoleBufferHeight(int height)
{
	COORD coord;
	coord.X = 80;
	coord.Y = height;
	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleScreenBufferSize(Handle, coord);
}

bool IsKeyDown(int vKey)
{
	return (GetAsyncKeyState(vKey) & 0x8000);
}


#endif /* UTILS_H_ */
