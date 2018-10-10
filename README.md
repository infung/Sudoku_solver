# Sudoku_solver

# CSP
- Variables: xij is the value in row i and column j where i is in {0, …, 8} and j is in {0, … ,8}.<br>
- Domains: if the initial value of xij is k, then dom(xij) = {k}. Otherwise, dom(xij) = {1, …, 9}.<br>
- Constraints: - All numbers in each row are different:<br>
			 - All numbers in each column are different.<br>
			 - All numbers in each sub-grip are different.<br>

# Three versions:
- Version A : Standard backtracking search <br>
- Version B : Standard backtracking search + forward checking <br>
- Version C : Standard backtracking search + forward checking + heuristics
(minimum-remaining-values heuristic, degree heuristic and leastconstraining-value
heuristic). <br>

# how to run
simply, run command <br>
$g++ -std=c++14 <<cpp file>> -o <<executable file>executable file> <br>
$./<<executable file>> < input.txt <br>

## input.txt:
eg: <br>
6 7 9 0 0 0 0 3 0 <br>
8 0 0 2 4 0 1 0 0 <br>
4 1 2 0 0 0 0 0 0 <br>
9 0 0 0 0 4 0 6 1 <br>
5 2 0 0 1 0 9 0 0 <br>
1 0 0 0 0 3 0 0 0 <br>
3 0 0 1 7 8 0 0 9 <br>
7 0 0 0 9 2 5 0 4 <br>
0 9 0 0 6 5 0 0 8 <br>
