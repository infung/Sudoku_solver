#include <iostream>
#include <climits>
#include <time.h>
#include <stdlib.h>
#include <vector>
using namespace std;

const int row = 9;
const int col = 9;
const int subgrid = 3;
const int domain = 9;

int count = 0;
int index = 0;

class SudokuBT {
	public:
	int grid[col][row];
	int allDomains[col][row][domain] = {{{0}}};

	SudokuBT() {
		int num;
		for(int i = 0; i < col; i++) {
			for(int j = 0; j < row; j++) {
				cin >> num;
				grid[i][j] = num;
				if(num > 0) {
				    for(int k = 0; k < domain; k++) {
				        if(k == num - 1) continue;
						allDomains[i][j][k] = -1;
				    }
				    for(int m = 0; m < col; m++) {
				        if(m == i) continue;
					    allDomains[m][j][num - 1] = -1;
				    }
				    
				    for(int n = 0; n < row; n++) {
				        if(n == j) continue;
					    allDomains[i][n][num - 1] = -1;
				    }
				    int subCol = i - i % subgrid;
				    int subRow = j - j % subgrid;

				    for(int p = subCol; p < subCol + subgrid; p++) {
					    for(int q = subRow; q < subRow + subgrid; q++) {
					        if(p == i && q == j) continue;
						        allDomains[p][q][num - 1] = -1;
					   }
				    }
				   
				    allDomains[i][j][num - 1] = num;
				} else {
				    for(int k = 0; k < domain; k++) {
				            if(allDomains[i][j][k] == -1) continue;
					    	allDomains[i][j][k] = k + 1;
			    	}
				}
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

	bool forwardchecking(int c, int r, int Domains[col][row][domain]) {
		//check if assignment is complete
		if(c == col) return true;
		//deal with assigned variable
		if(grid[c][r] != 0) {
			//all banks at current column have been reviewed 
			if(r == row - 1) {
				return forwardchecking(c+1, 0, Domains);
			} else {
			//some blanks at current column needed to be reveiewed
				return forwardchecking(c, r+1, Domains);
			}
		} else {
		//select value frrom it's domain
			for(int i = 0; i < domain; i++) {
				int value = Domains[c][r][i];
				if( value == -1) {
					continue;
				} 
            
				//save copy
				int DomainsCopy[domain][domain][domain];
				for(int j = 0; j < domain; j++) {
					for(int k = 0; k < domain; k++) {
						for(int l = 0; l < domain; l++) {
							DomainsCopy[j][k][l] = Domains[j][k][l];
						}
					}
				}

				//update Domains
				for(int m = 0; m < col; m++) {
				    if(m == c) continue;
					Domains[m][r][i] = -1;
				}
				
				for(int n = 0; n < row; n++) {
	                if(n == r) continue;
					Domains[c][n][i] = -1;
				}

				int subCol = c - c % subgrid;
				int subRow = r - r % subgrid;

				for(int p = subCol; p < subCol + subgrid; p++) {
					for(int q = subRow; q < subRow + subgrid; q++) {
					    if(p == c && q == r) continue;
						Domains[p][q][i] = -1;
					}
				}

				//check no domain is empty
				bool empty = false;
				for(int r = 0; r < col; r++) {
					for(int s = 0; s < row; s++) {
						int count_negone = 0;
						for(int t = 0; t < domain; t++) {
							if(Domains[r][s][t] == -1) {
								count_negone++;
							}
						}
						if(count_negone == domain) empty = true;
					}
				}
                
				if(!empty) {
					//check if current unassigned variable = value satisfies constraint
					if(satisfiesContraint(c, r, value)) {
					//add variable = value to assignment
				    	grid[c][r] = value;
				    	
 				    	count++;
					//check if backtracking results true
				    	if(r == row - 1) {
					    	if(forwardchecking(c+1, 0, Domains)) {
							    return true;
					        	}
				    	} else {
					    	if(forwardchecking(c, r+1, Domains)) {
						    	return true;
						    }
					    }
				    }
				}
				
				//remove variable = value from assignment. 
				grid[c][r] = 0;
				//revert Domains back to saved copy
				for(int a = 0; a < domain; a++) {
					for(int b = 0; b < domain; b++) {
						for(int c = 0; c < domain; c++) {
							Domains[a][b][c] = DomainsCopy[a][b][c];
						}
					}
				}
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
	bool ok = sb.forwardchecking(0, 0, sb.allDomains);
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
		cout << "forwardchecking not ok" << endl;
	}
	finish = clock();
    cout << "The total execution time: " << (double)(finish - start)/CLOCKS_PER_SEC << endl;
	return 0;
}

