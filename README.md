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
$g++ -std=c++14 <cppfile> <br> 
