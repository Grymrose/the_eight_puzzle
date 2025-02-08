#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <queue>
#include <map>
using namespace std;

const int PUZZLE_SIDE_LENGTH = 3;   // For a 3x3 puzzle. Can be changed for different puzzle sizes.

// Goal state; 0 is the blank tile. Can be changed for different puzzle sizes.
const vector<vector<int>> PUZZLE_GOAL = {{1, 2, 3},
                                         {4, 5, 6},
                                         {7, 8, 0}};

struct PuzzleNode {
    vector<vector<int>> puzzle;
    int x_blank, y_blank;   // Coordinates of the blank tile.
    int g = 0, h = 0;   // g = cost expended; h = heuristic value (distance to goal);
    PuzzleNode* parent; // Will be used to find the root node for solution tracing.

    PuzzleNode(vector<vector<int>> _p, int _x, int _y, int _g, int _h, PuzzleNode* _parent = nullptr)
        : puzzle(_p), x_blank(_x), y_blank(_y), g(_g), h(_h), parent(_parent) {}

    int f = g + h;  // Estimated cost of cheapest solution.

    // Overloads the > operator based on the f variable for the functionality of the queue.
    bool operator>(const PuzzleNode &_p_) const {
        return f > _p_.f;
    }
};

// Functions declarations.
void print_puzzle_prompt(vector<vector<int>>&, string&);
vector<vector<int>> init_premade_initial_puzzle(int);
int misplaced_tile(vector<vector<int>>);
int manhattan_distance(vector<vector<int>>);
void print_puzzle(vector<vector<int>>, int, int);
void print_puzzle_path(PuzzleNode*);
void print_summary(int, int, int);
void print_failure();
void general_search(vector<vector<int>>, string);

int main() {
    ios::sync_with_stdio(0);    // Fast input and output.

    // These two variables will get their respective values from print_puzzle_prompt().
    vector<vector<int>> initial_puzzle(PUZZLE_SIDE_LENGTH, vector<int>(PUZZLE_SIDE_LENGTH));
    string puzzle_heuristic_type;

    print_puzzle_prompt(initial_puzzle, puzzle_heuristic_type);

    // Keeps track of how long it takes to solve the puzzle.
    auto start = chrono::high_resolution_clock::now();
    general_search(initial_puzzle, puzzle_heuristic_type);
    auto end = chrono::high_resolution_clock::now();

    // Converts the time from microseconds to milliseconds.
    auto micro_duration = chrono::duration_cast<chrono::microseconds>(end - start);
    chrono::duration<double, milli> milli_duration = micro_duration;
    cout << "Time: " << milli_duration.count() << " milliseconds" << '\n' << '\n';

    return 0;
}

void print_puzzle_prompt(vector<vector<int>> &_puzzle, string &_heuristic_type) {
    cout << "Welcome! This is a program customized to solve an Eight Puzzle." << '\n'
         << "To get started, please do either one of the following:" << '\n'
         << "> Type \"1\" to choose a premade initial puzzle." << '\n' 
         << "> Type \"2\" to create your own initial puzzle." << '\n' 
         << "Press ENTER once you have typed your choice." << '\n';
    int puzzle_mode;
    cin >> puzzle_mode;
    cout << '\n';

    // Reasks the user if input was invalid.
    while (!(puzzle_mode == 1 || puzzle_mode == 2)) {
        cout << "Invalid input. Please try again." << '\n'
             << "Welcome! This is a program customized to solve an Eight Puzzle." << '\n'
             << "To get started, please do either one of the following:" << '\n'
             << "> Type \"1\" to choose a premade initial puzzle." << '\n' 
             << "> Type \"2\" to create your own initial puzzle." << '\n' 
             << "Press ENTER once you have typed your choice." << '\n';
        cin >> puzzle_mode;
        cout << '\n';
    }
    
    switch(puzzle_mode) {
        case 1:
            cout << "You have chosen to use a premade initial puzzle." << '\n'
                 << "Please type in an integer from 0 to 9." << '\n'
                 << "The number inputted will determine the DIFFICULTY to solve the puzzle." << '\n'
                 << "Press ENTER once you have typed your choice." << '\n';
            int puzzle_difficulty;
            cin >> puzzle_difficulty;
            cout << '\n';

            // Reasks the user if input was invalid.
            while (!(puzzle_difficulty >= 0 && puzzle_difficulty <= 9)) {
                cout << "Invalid input. Please try again." << '\n'
                     << "You have chosen to use a premade initial puzzle." << '\n'
                     << "Please type in an integer from 0 to 9." << '\n'
                     << "The number inputted will determine the DIFFICULTY to solve the puzzle." << '\n'
                     << "Press ENTER once you have typed your choice." << '\n';
                cin >> puzzle_difficulty;
                cout << '\n';
            }

            _puzzle = init_premade_initial_puzzle(puzzle_difficulty);
            break;
        case 2:
            cout << "You have chosen to create your own initial puzzle." << '\n'
                 << "Enter your puzzle, separating each number with a space in between." << '\n'
                 << "The blank tile should be represented with \"0\"." << '\n'
                 << "Please ensure that your puzzle is a legit 8-puzzle." << '\n'
                 << "Press ENTER when you finish. " << '\n' << '\n';
            
            for (int i = 0; i < PUZZLE_SIDE_LENGTH; i++) {  // Row and column size depends on PUZZLE_SIDE_LENGTH.
                cout << "Enter the numbers for row " << i+1 << ": ";
                for (int j = 0; j < PUZZLE_SIDE_LENGTH; j++) {
                    cin >> _puzzle[i][j];
                }
            }
            cout << '\n';
            break;
        default:
            break;  // Invalid input.
    }

    cout << "Please select an ALGORITHM by typing the number corresponding to the following: " << '\n'
         << "\"1\" for Uniform Cost Search" << '\n'
         << "\"2\" for Misplaced Tile Heuristic" << '\n'
         << "\"3\" for Manhattan Distance Heuristic" << '\n';
    int algorithm_type;
    cin >> algorithm_type;
    cout << '\n';

    // Reasks the user if input was invalid.
    while (!(algorithm_type == 1 || algorithm_type == 2 || algorithm_type == 3)) {
        cout << "Invalid input. Please try again." << '\n'
             << "Please select an ALGORITHM by typing the number corresponding to the following: " << '\n'
             << "\"1\" for Uniform Cost Search" << '\n'
             << "\"2\" for A* Misplaced Tile Heuristic" << '\n'
             << "\"3\" for A* Manhattan Distance Heuristic" << '\n';
        cin >> algorithm_type;
        cout << '\n';
    }

    switch (algorithm_type) {
        case 1:
            _heuristic_type = "Uniform Cost Search";
            break;
        case 2:
            _heuristic_type = "A* Misplaced";
            break;
        case 3:
            _heuristic_type = "A* Manhattan";
            break;
        default:
            break;  // Invalid input.
    }
    return;
}

vector<vector<int>> init_premade_initial_puzzle(int _difficulty) {  // Premade puzzles for testing algorithm.
    switch(_difficulty) {
        case 0:
            return {{1, 2, 3},  // Depth 0
                    {4, 5, 6},
                    {7, 8, 0}};
            break;
        case 1:
            return {{1, 2, 3},  // Depth 1
                    {4, 5, 0},
                    {7, 8, 6}}; 
            ;
        case 2:
            return {{1, 0, 3},  // Depth 3
                    {4, 2, 6},
                    {7, 5, 8}};
            break;
        case 3:
            return {{2, 3, 0},  // Depth 6
                    {1, 4, 5},
                    {7, 8, 6}};
            break;
        case 4:
            return {{1, 2, 3},  // Depth 9
                    {0, 5, 7},
                    {4, 8, 6}};
            break;
        case 5:
            return {{1, 2, 4},  // Depth 12
                    {7, 5, 3},
                    {0, 8, 6}};
            break;
        case 6:
            return {{7, 4, 3},  // Depth 15
                    {0, 2, 1},
                    {8, 5, 6}};
            break;
        case 7:
            return {{0, 6, 3},  // Depth 18
                    {1, 2, 7},
                    {5, 4, 8}};
            break;
        case 8:
            return {{8, 2, 6},  // Depth 21
                    {1, 7, 0},
                    {5, 4, 3}};
            break;
        case 9:
            return {{4, 6, 0},  // Depth 24
                    {1, 5, 8},
                    {7, 2, 3}};
            break;
        default:
            return {{1, 2, 3},  // Invalid input.
                    {4, 5, 6},
                    {7, 8, 0}};
            break;
    }
}

int misplaced_tile(vector<vector<int>> _p) {
    int dist = 0;
    for (int i = 0; i < PUZZLE_SIDE_LENGTH; i++) {
        for (int j = 0; j < PUZZLE_SIDE_LENGTH; j++) {
            if (_p[i][j] == 0) {
                continue; // Skips the blank tile.
            }
            if (_p[i][j] != PUZZLE_GOAL[i][j]) {
                dist++;
            }
        }
    }
    return dist;
}

int manhattan_distance(vector<vector<int>> _p) {
    int dist = 0;
    for (int i = 0; i < PUZZLE_SIDE_LENGTH; i++) {
        for (int j = 0; j < PUZZLE_SIDE_LENGTH; j++) {
            if (_p[i][j] == 0) {
                continue; // Skips the blank tile.
            }
            for (int x = 0; x < PUZZLE_SIDE_LENGTH; x++) {
                for (int y = 0; y < PUZZLE_SIDE_LENGTH; y++) {
                    if (_p[i][j] == PUZZLE_GOAL[x][y]) {
                        dist += abs(i-x) + abs(j-y);    // Distance between matching tiles (can't be negative).
                    }
                }
            }
        }
    }
    return dist;
}

void print_puzzle(vector<vector<int>> _p, int _g, int _h) {
    cout << "The best node to expand with a g(n) = " << _g << " and h(n) = " << _h << " is:" << '\n';
    for (int i = 0; i < PUZZLE_SIDE_LENGTH; i++) {
        cout << "[";    
        for (int j = 0; j < PUZZLE_SIDE_LENGTH; j++) {
            cout << _p[i][j];
            if (j < PUZZLE_SIDE_LENGTH-1) {
                cout << ", ";
            }
        }
        cout << "]" << '\n';
    }
}

void print_puzzle_path(PuzzleNode* _goal) {
    vector<PuzzleNode*> puzzle_path;
    PuzzleNode* curr = _goal;

    while (curr != nullptr) {   // Traces back to root node.
        puzzle_path.push_back(curr);
        curr = curr->parent;
    }
    
    // Makes the path start with the root and end with the goal.
    reverse(puzzle_path.begin(), puzzle_path.end());

    for (PuzzleNode* node : puzzle_path) {
        print_puzzle(node->puzzle, node->g, node->h);
    }
}

void print_summary(int _g, int _n, int _s) {
    cout << "Goal state!" << '\n' << '\n'
         << "Solution depth was " << _g << '\n'
         << "Number of nodes expanded: " << _n << '\n'
         << "Max queue size: " << _s << '\n' << '\n';
}

void print_failure() {
    cout << "Unfortunately, your puzzle is impossible to solve." << '\n' << '\n';
}

void general_search(vector<vector<int>> problem, string heuristic_type) {
    int x_blank, y_blank;   // Coordinates for the blank tile.
    for (int i = 0; i < PUZZLE_SIDE_LENGTH; i++) {  // Locate the blank tile.
        for (int j = 0; j < PUZZLE_SIDE_LENGTH; j++) {
            if (problem[i][j] == 0) {
                x_blank = i;
                y_blank = j;
                break;
            }
        }
    }

    // Set heuristic type for root_node (original problem/puzzle).
    int heuristic = (heuristic_type == "A* Manhattan") ? manhattan_distance(problem)
                  : (heuristic_type == "A* Misplaced") ? misplaced_tile(problem)
                  : 0;  // Uniform Cost Search has a heuristic value of 0.
    
    // Creates a queue that prioritizes SMALLER costs of cheapest solutions first for the puzzle's nodes.
    priority_queue<PuzzleNode, vector<PuzzleNode>, greater<PuzzleNode>> q;
    PuzzleNode root_node = PuzzleNode(problem, x_blank, y_blank, 0, heuristic, nullptr);
    q.push(root_node);

    // Remembers visited puzzle nodes as keys and booleans as values.
    map<vector<vector<int>>, bool> puzzle_visits;

    const vector<int> move_x = {-1, 1, 0, 0};  // "Up" and "Down" moves.
    const vector<int> move_y = {0, 0, -1, 1};  // "Left" and "Right" moves.

    int nodes_expanded = 0, q_max_size = 1;
    while (!q.empty()) {
        q_max_size = max(q_max_size, (int)q.size());    // Track largest queue size.

        PuzzleNode curr = q.top();
        q.pop();
        nodes_expanded++;

        if (puzzle_visits[curr.puzzle]) {   // Skip already visited nodes.
            continue;
        }
        puzzle_visits[curr.puzzle] = true;  // Remembers this node.

        if (curr.puzzle == PUZZLE_GOAL) {   // Successful search.
            print_puzzle_path(&curr);
            print_summary(curr.g, nodes_expanded, q_max_size);
            return;
        }

        for (int m = 0; m < 4; m++) {   // Considers all possible moves for the blank tile.
            int new_x_blank = curr.x_blank + move_x[m];
            int new_y_blank = curr.y_blank + move_y[m];
            
            if (new_x_blank < 0 || new_x_blank >= PUZZLE_SIDE_LENGTH || new_y_blank < 0 || new_y_blank >= PUZZLE_SIDE_LENGTH) {
                continue;   // Ignore tiles that are out-of-bounds.
            }

            vector<vector<int>> new_puzzle = curr.puzzle;
            swap(new_puzzle[curr.x_blank][curr.y_blank], new_puzzle[new_x_blank][new_y_blank]);

            // Makes sure new_puzzle has matching heuristic type.
            heuristic = (heuristic_type == "A* Manhattan") ? manhattan_distance(new_puzzle)
                      : (heuristic_type == "A* Misplaced") ? misplaced_tile(new_puzzle)
                      : 0;  // Uniform Cost Search has a heuristic value of 0.
            
            q.push(PuzzleNode(new_puzzle, new_x_blank, new_y_blank, curr.g+1, heuristic, new PuzzleNode(curr)));
        }
    }
    print_failure();    // Unsuccessful search.
    return;
}