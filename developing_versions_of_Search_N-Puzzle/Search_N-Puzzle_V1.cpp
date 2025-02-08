// Includes the entire standard (std) library for C++.
#include <bits/stdc++.h>
using namespace std;


const int PUZZLE_SIDE_LENGTH = 3;   // For a 3x3 puzzle. Can be changed if needed.


// The goal state of the 8-puzzle where 0 is the blank tile. Can be changed if needed.
const vector<vector<int>> PUZZLE_GOAL = {{1, 2, 3},
                                         {4, 5, 6},
                                         {7, 8, 0}};


struct PuzzleNode {
    vector<vector<int>> puzzle;
    int x_blank, y_blank;   // Coodinates of the blank tile.
    int g = 0, h = 0;   // g = cost expended; h = distance to goal;


    // The constructor for the PuzzleNode.
    PuzzleNode(vector<vector<int>> _p, int _x, int _y, int _g, int _h)
        : puzzle(_p), x_blank(_x), y_blank(_y), g(_g), h(_h) {}


    int f = g + h;  // Estimated cost of cheapest solution.


    // Overloads the > operator for priority queue functionality.
    bool operator>(const PuzzleNode &_p_) const {return f > _p_.f;}
};


void general_search(vector<vector<int>>, int);
int misplaced_distance(vector<vector<int>>);
int manhattan_distance(vector<vector<int>>);


int main() {
    // Fast input and output.
    ios::sync_with_stdio(0);


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


void general_search(vector<vector<int>> problem, int heuristic) {
    int p_x_blank, p_y_blank;
    for (int i = 0; i < PUZZLE_SIDE_LENGTH; i++)
        for (int j = 0; j < PUZZLE_SIDE_LENGTH; j++)
            if (problem[i][j] == 0) {   // Locate the blank tile.
                p_x_blank = i; p_y_blank = j; break;
            }
   
    // Creates a min-heap priority queue for the puzzle's nodes.
    priority_queue<PuzzleNode, vector<PuzzleNode>, greater<PuzzleNode>> q;
    PuzzleNode root_node = PuzzleNode(problem, p_x_blank, p_y_blank, 0, heuristic);
    q.push(root_node);


    while (!q.empty()) {


    }
   
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