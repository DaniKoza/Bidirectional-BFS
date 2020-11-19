/*
# AI First Assignment - Bidirectional BFS
#
#	Liel Titelbaum,
#	Daniel Kozachkevich.
*/

#pragma once
class Cell
{
private:
	int curr_row_start, col;
	Cell* parent;
	bool isVisited;
public:
	Cell(int r, int c, Cell* pc);
	int GetRow();
	int GetCol();
	Cell* GetParent();
};

