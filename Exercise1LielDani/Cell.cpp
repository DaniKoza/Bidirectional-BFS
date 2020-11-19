/*
# AI First Assignment - Bidirectional BFS
#
#	Liel Titelbaum, ID: 208042523
#	Daniel Kozachkevich, ID: 203690359

*/
#include "Cell.h"

Cell::Cell(int r, int c, Cell* pc)
{
	curr_row_start = r;
	col = c;
	parent = pc;
}


int Cell::GetRow()
{
	return curr_row_start;
}

int Cell::GetCol()
{
	return col;
}

Cell* Cell::GetParent()
{
	return parent;
}
