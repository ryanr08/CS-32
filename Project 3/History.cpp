#include "History.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
{
	m_rows = nRows;
	m_cols = nCols;
	for (int r = 0; r < m_rows; r++)
		for (int c = 0; c < m_cols; c++)
			recorder[r][c] = 0;
}

bool History::record(int r, int c)
{
	if (r > m_rows || c > m_cols || r < 0 || c < 0)
		return false;
	else
		recorder[r][c]++;
	return true;
}

void History::display() const
{
	char grid[MAXROWS][MAXCOLS];
	clearScreen();
	for (int r = 0; r < m_rows; r++)
	{
		for (int c = 0; c < m_cols; c++)
		{
			if (recorder[r][c] >= 26)
				grid[r][c] = 'Z';
			else if (recorder[r][c] > 0)
				grid[r][c] = (char)((recorder[r][c] + 64));
			else
				grid[r][c] = '.';
		}
	}

	for (int r = 0; r < m_rows; r++)
	{
		for (int c = 0; c < m_cols; c++)
			cout << grid[r][c];
		cout << endl;
	}
	cout << endl;

}