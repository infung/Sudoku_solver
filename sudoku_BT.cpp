#include <iostream>
#include <climits>
#include <time.h>
#include <stdlib.h>

using namespace std;

const int row = 9;
const int col = 9;
const int subgrid = 3;
const int domain = 9;

int count = 0;

class SudokuBT {
	public:
	int grid[col][row];
	
	SudokuBT() {
		int num;
		for(int i = 0; i < col; i++) {
			for(int j = 0; j < row; j++) {
				cin >> num;
				grid[i][j] = num;
			}
		}
	}

	bool satisfiesContraint(int c, int r, int v) {
		//check for alldifferentCol
		for(int i = 0; i < col; i++) {
			if(grid[i][r] == v) return false;
		}

		//check for alldifferentRow
		for(int j = 0; j < row; j++) {
			if(grid[c][j] == v) return false;
		}

		//check for subgrid
		int subCol = c - c % subgrid;
		int subRow = r - r % subgrid;

		for(int k = subCol; k < subCol + subgrid; k++) {
			for(int l = subRow; l < subRow + subgrid; l++) {
				if(grid[k][l] == v) return false;
			}
		}

		return true;
	}

	//pass assignment to backtrack function
	bool backtracking(int c, int r) {
		//check if assignment is complete
		if(c == col) return true;
		//deal with assigned variable
		if(grid[c][r] != 0) {
			//all banks at current column have been reviewed 
			if(r == row - 1) {
				return backtracking(c+1, 0);
			} else {
			//some blanks at current column needed to be reveiewed
				return backtracking(c, r+1);
			}
		} else {
		//select unassigned variable
			//for all value in domain {1, .., 9}
			for(int value = 1; value <= domain; value++) {
				//check if current unassigned variable = value satisfies constraint
				if(satisfiesContraint(c, r, value)) {
					//add variable = value to assignment
					grid[c][r] = value;
					count++;
					//check if backtracking results true
					if(r == row - 1) {
						if(backtracking(c+1, 0)) {
							return true;
						}
					} else {
						if(backtracking(c, r+1)) {
							return true;
						}
					}
				}

				//remove variable = value from assignment. we dont need to remove any since we did not add it to assignment. just assign 0 in case.
				grid[c][r] = 0;
			}	
		}
		//discard the changes to assignment if current sub tracking failed.
		grid[c][r] = 0;
		return false;
	}

};

int main() {
	clock_t start;
    clock_t finish;
    start = clock();
	SudokuBT sb;
	bool ok = sb.backtracking(0, 0);
	///////////////print!//////////////
	if(ok) {
	for(int i = 0; i < col; i ++) {
		for(int j = 0; j < row; j++) {
			cout << sb.grid[i][j] << " ";
		}
		cout << endl;
	}
	cout << "Total numbder of variable assignments: " << count << endl;
	} else {
		cout << "backtracking not ok" << endl;
	}
	finish = clock();
    cout << "The total execution time: " << (double)(finish - start)/CLOCKS_PER_SEC << endl;
	return 0;
}