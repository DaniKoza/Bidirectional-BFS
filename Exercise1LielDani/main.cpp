/*
# AI First Assignment - Bidirectional BFS
# 
#	Liel Titelbaum, ID: 208042523
#	Daniel Kozachkevich, ID: 203690359

*/

#include <time.h>
#include <vector>
#include <iostream>
#include "freeglut_std.h"
#include "Cell.h"

using namespace std;

const int MSZ = 100;

const int SPACE = 0;
const int WALL = 1;
const int START = 2;
const int TARGET = 3;
const int BLACK = 4;
const int GRAY = 5;
const int PATH = 6;

int maze[MSZ][MSZ] = { 0 }; // 0 is SPACE

bool bfs_is_on = false;

vector <Cell*> graysStart;
vector <Cell*> graysTarget;

vector<vector<int>> s_visited(MSZ, vector<int>(MSZ, 0));
vector<vector<int>> t_visited(MSZ, vector<int>(MSZ, 0));



void InitMaze();

void init()
{
	glClearColor(0.8, 0.7, 0.5, 0);// color of window background
	glOrtho(-1, 1, -1, 1, 1, -1);

	srand(time(0));

	InitMaze();
}

void InitMaze()
{
	int i, j;

	// set margins
	for (i = 0; i < MSZ; i++)
	{
		maze[0][i] = WALL;
		maze[MSZ-1][i] = WALL;
		maze[i][0] = WALL;
		maze[i][MSZ - 1] = WALL;
	}

	for(i=1;i<MSZ-1;i++)
		for (j = 1; j < MSZ - 1; j++)
		{
			if (i % 2 == 1)  // this is mostly SPACE
			{
				if (rand() % 100 < 10) // 10% WALLs
					maze[i][j] = WALL;
				else
					maze[i][j] = SPACE;
			}
			else // this is mostly WALL
			{
				if (rand() % 100 < 40) // 40% SPACEs
					maze[i][j] = SPACE;
				else
					maze[i][j] = WALL;
			}
		}

	maze[MSZ / 2][MSZ / 2] = START;

	int targetCol = rand() % MSZ;
	int targetRow = rand() % MSZ;
	maze[targetRow][targetCol] = TARGET;

	Cell* startCell = new Cell(MSZ / 2, MSZ / 2, nullptr); // this is the START cell, parent is null
	Cell* targetCell = new Cell(targetRow, targetCol, nullptr);  // this is the TARGET cell, parent is null

	if (!graysStart.empty() && !graysTarget.empty())
	{
		graysStart.clear();
		graysTarget.clear();
	}
	graysStart.push_back(startCell); // add first cell to grays of start 
	graysTarget.push_back(targetCell); // add first cell to grays of start 

}

void DrawMaze()
{
	int i, j;
	double sx, sy; // cell size
	double x, y;

	sx = 2.0 / MSZ;
	sy = 2.0 / MSZ;

	for(i=0;i<MSZ;i++)
		for (j = 0; j < MSZ; j++)
		{
			switch (maze[i][j])
			{
			case SPACE: 
				glColor3d(1, 1, 1);   // white
				break;
			case WALL:
				glColor3d(0.29, 0.46, 0.43);   // Green-Copper
				break;
			case START:
				glColor3d(1, 1, 0);   // yellow
				break;
			case TARGET:
				glColor3d(1, 0,0);   // red
				break;
			case GRAY:
				glColor3d(0.917647, 0.678431, 0.917647);   // Plum
				break;
			case BLACK:
				glColor3d(0.8, 1, 0.8);   // green
				break;
			case PATH:
				glColor3d(0.8, 0.4, 1);   // magenta
				break;
			}
			// draw square maze[i][j]
			x = 2 * (j / (double)MSZ) - 1;
			y = 2 * (i / (double)MSZ) - 1;

			glBegin(GL_POLYGON);  // fill up
			glVertex2d(x, y);
			glVertex2d(x, y + sy);
			glVertex2d(x + sx, y + sy);
			glVertex2d(x + sx, y);
			glEnd();
			glColor3d(0, 0, 0);

			//glBegin(GL_LINE_LOOP);  // cell boundaries, optional
			//glVertex2d(x, y);
			//glVertex2d(x, y + sy);
			//glVertex2d(x + sx, y + sy);
			//glVertex2d(x + sx, y);
			//glEnd();

		}
}

void RestorePath(Cell* current)
{
	while (current->GetParent() != nullptr)
	{
		maze[current->GetRow()][current->GetCol()] = PATH;
		current = current->GetParent();
	}
}

Cell* findTheIntersectionCell(int row, int col, bool isGraysStart) {
	int counter = 0;
	if (isGraysStart) 
	{
		for (auto& cell : graysTarget) {
			if (cell->GetRow() == row && cell->GetCol() == col)
				return graysTarget[counter];
			++counter;
		}
	}
	else
	{
		counter = 0;
		for (auto& cell : graysStart) {
			if (cell->GetRow() == row && cell->GetCol() == col)
				return graysStart[counter];
			++counter;
		}
	}
}


void CheckNeighbor(Cell* currentCellOfStart, Cell* currentCellOfTarget, int row, int col, bool isStart)
{
	Cell* next;

	switch (isStart)
	{
	case true:
		if (maze[row][col] == SPACE) // paint it gray
		{
			maze[row][col] = GRAY;
			next = new Cell(row, col, currentCellOfStart);
			graysStart.push_back(next);
			s_visited[row][col] = 1;
		} 
		else if (maze[row][col] == GRAY && t_visited[row][col] == 1) {
			// cout << "i came from starts's BFS" << "row" << row << " col = " << col << endl;
			RestorePath(currentCellOfStart);
			RestorePath(findTheIntersectionCell(row,col, true));
			bfs_is_on = false;
		}
		break;

	case false: // check neighbors of target bfs 
		if (maze[row][col] == SPACE) // paint it gray
		{
			maze[row][col] = GRAY;
			next = new Cell(row, col, currentCellOfTarget);
			graysTarget.push_back(next);
			t_visited[row][col] = 1;
		}
		else if (maze[row][col] == GRAY && s_visited[row][col] == 1) { 
		//	cout << "i came from target's BFS" << "row" << row << " col = " << col << endl;
			RestorePath(currentCellOfTarget);
			RestorePath(findTheIntersectionCell(row, col, false));
			bfs_is_on = false;

		}
		break;
	default:
		break;
	}
}

void helperCallToCheckNeighbor(Cell* currentCellOfStart, Cell* currentCellOfTarget, int row, int col, bool isStart)
{
	// up
	if (bfs_is_on)
		CheckNeighbor(currentCellOfStart, currentCellOfTarget, row + 1, col, isStart);
	// down 
	if (bfs_is_on)
		CheckNeighbor(currentCellOfStart, currentCellOfTarget, row - 1, col, isStart);
	// left
	if (bfs_is_on)
		CheckNeighbor(currentCellOfStart, currentCellOfTarget, row, col - 1, isStart);
	// right
	if (bfs_is_on)
		CheckNeighbor(currentCellOfStart, currentCellOfTarget, row, col + 1, isStart);
}

void BFSIteration()
{
	Cell* currentCellOfStart;
	Cell* currentCellOfTarget;
	int curr_row_start, curr_col_start;
	int curr_row_target, curr_col_target;
	

	if (graysStart.empty() || graysTarget.empty()) // No solution...
	{
		cout << "No solution\n";
		bfs_is_on = false;
	}
	else
	{
		// take the first element, paint it black and add to grays its white neighbors
		currentCellOfStart = graysStart.front();
		currentCellOfTarget = graysTarget.front();

		graysStart.erase(graysStart.begin()); // remove the front element of start grays vector
		curr_row_start = currentCellOfStart->GetRow();
		curr_col_start = currentCellOfStart->GetCol();

		graysTarget.erase(graysTarget.begin()); // remove the front element of target gray vector
		curr_row_target = currentCellOfTarget->GetRow();
		curr_col_target = currentCellOfTarget->GetCol();

		// paint it black
		if (maze[curr_row_start][curr_col_start] != START && maze[curr_row_target][curr_col_target] != TARGET)
		{
			maze[curr_row_start][curr_col_start] = BLACK;
			maze[curr_row_target][curr_col_target] = BLACK;
		}

		// look for white neighbors of the vectors start
		helperCallToCheckNeighbor(currentCellOfStart, currentCellOfTarget, curr_row_start, curr_col_start, true);

		// now check for target's bfs
		helperCallToCheckNeighbor(currentCellOfStart, currentCellOfTarget, curr_row_target, curr_col_target, false);	
	}
}



void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	DrawMaze();

	glutSwapBuffers(); // show all
}

void idle()
{

	if (bfs_is_on)
		BFSIteration();

	glutPostRedisplay(); // indirect call to display
}

void clearBoard()
{
	for (auto& v1 : t_visited) {
		std::fill(v1.begin(), v1.end(), 0);
	}

	for (auto& v2 : s_visited) {
		std::fill(v2.begin(), v2.end(), 0);
	}
	
	init();
}

void menu(int choice)
{
	switch (choice)
	{
	case 1: // BFS
		bfs_is_on = true;
		break;

	case 2: // Clear solution
		clearBoard();
		break;
	}
}
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(200, 100);
	glutCreateWindow(" Bidirectional BFS ");

	glutDisplayFunc(display);
	glutIdleFunc(idle);

	// menu
	glutCreateMenu(menu);
	glutAddMenuEntry("Run Bidirectional BFS", 1);
	glutAddMenuEntry("Clear solution ", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	init();

	glutMainLoop();

	return EXIT_SUCCESS;
}