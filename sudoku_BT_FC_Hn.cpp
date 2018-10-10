#include <iostream>
#include <climits>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;

const int row = 9;
const int col = 9;
const int subgrid = 3;
const int domain = 9;

int Count = 0;

struct point {
	int c;
	int r;
};


point MaxDegree(int g[col][row], vector<point>& a) {
	point p;
	if(a.size() == 1) {
		p = a[0];
		a.pop_back();
		return p;
	}

	int deg = -1;
	for(int i = 0; i < a.size(); i++) {
		int count_constraint = 0;
		for(int j = 0; j < col; j++) {
			 if(g[a[i].c][j] == 0) count_constraint++;
			 if(g[j][a[i].r] == 0) count_constraint++;
		}

		int subCol = a[i].c - a[i].c % subgrid;
		int subRow = a[i].r - a[i].r % subgrid;

		for(int k = subCol; k < subCol + subgrid; k++) {
			for(int l = subRow; l < subRow + subgrid; l++) {
				if(g[k][l] == 0) count_constraint++;
			}
		}
		if(count_constraint > deg) {
			deg = count_constraint;
			p = a[i];
		}
	}

	return p;
}

point MinimunRemainingValueArray(int D[col][row][domain], int g[col][row]) {
	int min = 0;
	vector<point> array;
	for(int k = 0; k < col; k++) {
	    for(int l = 0; l < row; l++) {
	    	//unassigned variable
	        if(g[k][l] == 0) {
	        	int count_neg_one = 0;
	        	for(int m = 0; m < domain; m++) {
	            	if(D[k][l][m] == -1) count_neg_one++;
	        	}
	        	if(count_neg_one == min) {
	            	point p = {k, l};
	        		array.push_back(p);
	            	continue;
	        	}
	        	if(count_neg_one > min) {
	        	    min = count_neg_one;
	        		array.clear();
	        		point p = {k, l};
	        		array.push_back(p);
	        	}
	        }
	    }
	}
	return MaxDegree(g, array);
}

bool comparison(const pair<int,int> &a,const pair<int,int> &b){
    return a.second>b.second;
}

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
						allDomains[i][j][k] = -1;
				    }
				    for(int m = 0; m < col; m++) {			
					    allDomains[m][j][num - 1] = -1;
				    }
				    
				    for(int n = 0; n < row; n++) {
					    allDomains[i][n][num - 1] = -1;
				    }
				    int subCol = i - i % subgrid;
				    int subRow = j - j % subgrid;

				    for(int p = subCol; p < subCol + subgrid; p++) {
					    for(int q = subRow; q < subRow + subgrid; q++) {
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
					Count++;
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
    
	vector<int>  LeastConstraintValue(int Domains[col][row][domain], int c, int r) {
		vector< pair <int, int> > values;
		for(int i = 0; i < domain; i++) {
			int val = Domains[c][r][i];
			int count_val = 0;
			if(val == -1) continue;
			for(int j = 0; j < col; j++) {
			 	if(Domains[c][j][val - 1] == -1) count_val++;
			 	if(Domains[j][r][val - 1] == -1) count_val++;
			}

			int subCol = c - c % subgrid;
			int subRow = r - r % subgrid;

			for(int k = subCol; k < subCol + subgrid; k++) {
				for(int l = subRow; l < subRow + subgrid; l++) {
					if(Domains[k][l][val -1] == -1) count_val++;
				}
			}

			values.push_back(std::make_pair(val, count_val));
		}
		sort(values.begin(), values.end(), comparison);
		vector<int> a;
		for(int q = 0; q < values.size(); q++) {
		    int v = values[q].first;
		    a.push_back(v);
		}
		return a;
	}

	bool check_complete() {
        for(int i = 0; i < col; i++) {
            for(int j = 0; j < row; j++) {
                if(grid[i][j]  == 0) return false;
            }
        }
        return true;
    }

	bool forwardchecking(int c, int r, int Domains[col][row][domain]) {
		//check if assignment is complete
		if(check_complete()) return true;
		//deal with assigned variable
		if(grid[c][r] != 0) {
			point p = MinimunRemainingValueArray(Domains, grid);
			return forwardchecking(p.c, p.r, Domains);
		} else {
		//select value frrom it's domain
		    vector<int> values;
		    values = LeastConstraintValue(Domains, c, r);
			for(int i = 0; i < values.size(); i++) {
				int value = values[i];
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
					Domains[m][r][value - 1] = -1;
				}
				
				for(int n = 0; n < row; n++) {
	                if(n == r) continue;
					Domains[c][n][value - 1] = -1;
				}

				int subCol = c - c % subgrid;
				int subRow = r - r % subgrid;

				for(int p = subCol; p < subCol + subgrid; p++) {
					for(int q = subRow; q < subRow + subgrid; q++) {
					    if(p == c && q == r) continue;
						Domains[p][q][value - 1] = -1;
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
 				    	Count++;
					//check if forwardchecking results true
 				    	point p = MinimunRemainingValueArray(Domains, grid);
					    if(forwardchecking(p.c, p.r, Domains)) {
							return true;
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
	point p = MinimunRemainingValueArray(sb.allDomains, sb.grid);
	bool ok = sb.forwardchecking(p.c, p.r, sb.allDomains);
	///////////////print!//////////////
	if(ok) {
	for(int i = 0; i < col; i ++) {
		for(int j = 0; j < row; j++) {
			cout << sb.grid[i][j] << " ";
		}
		cout << endl;
	}
	cout << "Total numbder of variable assignments: " << Count << endl;
	} else {
		cout << "forwardchecking not ok" << endl;
	}
	finish = clock();
    cout << "The total execution time: " << (double)(finish - start)/CLOCKS_PER_SEC << endl;
	return 0;
}

