#include <bits/stdc++.h>    // Includes the entire standard (std) library for C++.
using namespace std;


const int PUZZLE_SIDE_LENGTH = 3;   // For a 3x3 puzzle. Can be changed if needed.


// Goal state of the 8-puzzle where 0 is the blank tile. Can be changed if needed.
const vector<vector<int>> PUZZLE_GOAL = {{1, 2, 3},
                                         {4, 5, 6},
                                         {7, 8, 0}};


struct PuzzleNode {
    vector<vector<int>> puzzle;
    int x_blank, y_blank;   // Coordinates of the blank tile.
    int g = 0, h = 0;   // g = cost expended; h = distance to goal;


    PuzzleNode(vector<vector<int>> _p, int _x, int _y, int _g, int _h)
        : puzzle(_p), x_blank(_x), y_blank(_y), g(_g), h(_h) {}


    int f = g + h;  // Estimated cost of cheapest solution.


    // Overloads the > operator for priority queue functionality.
    bool operator>(const PuzzleNode &_p_) const {return f > _p_.f;}
};


// Global map that remembers a puzzle's path.
map<vector<vector<int>>, vector<vector<int>>> puzzle_path;


// Functions declarations.
vector<vector<int>> general_search(vector<vector<int>>, string);
int misplaced_distance(vector<vector<int>>);
int manhattan_distance(vector<vector<int>>);


int main() {
    ios::sync_with_stdio(0);    // Fast input and output.


    cout << "Welcome! This is a program customized to solve an Eight Puzzle. To get started, please "
            << "type \"1\" to choose a premade initial puzzle, or type \"2\" to create your own initial puzzle."
            << '\n';
    int puzzle_init_method; cin >> puzzle_init_method;


    switch(puzzle_init_method) {
        case 1:
            break;
        case 2:
            break;
        default:
            break;
    }


    return 0;
}


vector<vector<int>> general_search(vector<vector<int>> problem, string heuristic_type) {
    const vector<vector<int>> puzzle_fail = {{-1, -1, -1},
                                             {-1, -1, -1},
                                             {-1, -1, -1}};


    int p_x_blank, p_y_blank;   // Coordinates for the blank tile.
    for (int i = 0; i < PUZZLE_SIDE_LENGTH; i++)    // Locate the blank tile.
        for (int j = 0; j < PUZZLE_SIDE_LENGTH; j++)
            if (problem[i][j] == 0) {
                p_x_blank = i; p_y_blank = j; break;
            }


    int heuristic = (heuristic_type == "A* Manhattan") ? manhattan_distance(problem)
                  : (heuristic_type == "A* Misplaced") ? misplaced_distance(problem)
                  : 0;
   
    // Creates a min-heap priority queue for the puzzle's nodes.
    priority_queue<PuzzleNode, vector<PuzzleNode>, greater<PuzzleNode>> q;
    PuzzleNode root_node = PuzzleNode(problem, p_x_blank, p_y_blank, 0, heuristic);
    q.push(root_node);


    // Map that stores visited puzzles as keys and booleans as values.
    map<vector<vector<int>>, bool> puzzle_visits;


    const vector<int> m_x = {-1, 1, 0, 0};  // "Up" and "Down" moves.
    const vector<int> m_y = {0, 0, -1, 1};  // "Left" and "Right" moves.


    while (!q.empty()) {
        PuzzleNode curr = q.top();
        q.pop();


        if (puzzle_visits[curr.puzzle]) continue;
        puzzle_visits[curr.puzzle] = true;
       
        if (curr.puzzle == PUZZLE_GOAL) return curr.puzzle;


        for (int m = 0; m < 4; m++) {
            int n_x_blank = curr.x_blank + m_x[m], n_y_blank = curr.y_blank + m_y[m];
           
            // Ignore tiles that are out-of-bounds.
            if (n_x_blank < 0 || n_x_blank >= PUZZLE_SIDE_LENGTH
            || n_y_blank < 0 || n_y_blank >= PUZZLE_SIDE_LENGTH) continue;


            vector<vector<int>> new_puzzle = curr.puzzle;
            swap(new_puzzle[curr.x_blank][curr.y_blank], new_puzzle[n_x_blank][n_y_blank]);


            int new_heuristic = (heuristic_type == "A* Manhattan") ? manhattan_distance(new_puzzle)
                              : (heuristic_type == "A* Misplaced") ? misplaced_distance(new_puzzle)
                              : 0;


            q.push(PuzzleNode(new_puzzle, n_x_blank, n_y_blank, curr.g+1, new_heuristic));
        }
    }
   
    return puzzle_fail; // Unsuccessful search.
}


int misplaced_distance(vector<vector<int>> _p) {
    int dist = 0;
    for (int i = 0; i < PUZZLE_SIDE_LENGTH; i++)
        for (int j = 0; j < PUZZLE_SIDE_LENGTH; j++) {
            if (_p[i][j] == 0) continue; // Skips the blank tile.
            if (_p[i][j] != PUZZLE_GOAL[i][j])
                dist++;
        }
    return dist;
}


int manhattan_distance(vector<vector<int>> _p) {
    int dist = 0;
    for (int i = 0; i < PUZZLE_SIDE_LENGTH; i++)
        for (int j = 0; j < PUZZLE_SIDE_LENGTH; j++) {
            if (_p[i][j] == 0) continue; // Skips the blank tile.
            for (int x = 0; x < PUZZLE_SIDE_LENGTH; x++)
                for (int y = 0; y < PUZZLE_SIDE_LENGTH; y++)
                    if (_p[i][j] == PUZZLE_GOAL[x][y])
                        dist += abs(i-x) + abs(j-y);    // Distance between matching tiles (can't be negative).
        }
    return dist;
}