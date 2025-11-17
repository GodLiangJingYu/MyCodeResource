#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <algorithm>

using namespace std;

// The grid to store the base numbers.
vector<vector<int>> board;
// A counter for assigning unique numbers to each L-shaped base.
int base_id_counter = 1;

// Function to remap the IDs to achieve left-to-right, top-to-bottom ordering
void remapIds(int size) {
    // Create a mapping from old IDs to new IDs
    vector<pair<int, pair<int, int>>> id_positions; // {old_id, {row, col}}
    
    // Collect positions of each unique ID (excluding 0)
    for (int i = 1; i <= size; ++i) {
        for (int j = 1; j <= size; ++j) {
            if (board[i][j] != 0) {
                id_positions.push_back({board[i][j], {i, j}});
            }
        }
    }
    
    // Sort by position (row first, then column)
    sort(id_positions.begin(), id_positions.end(), 
         [](const auto& a, const auto& b) {
             if (a.second.first != b.second.first) {
                 return a.second.first < b.second.first;
             }
             return a.second.second < b.second.second;
         });
    
    // Create mapping from old ID to new ID
    map<int, int> old_to_new;
    int new_id = 1;
    
    for (const auto& item : id_positions) {
        int old_id = item.first;
        if (old_to_new.find(old_id) == old_to_new.end()) {
            old_to_new[old_id] = new_id++;
        }
    }
    
    // Apply the mapping to the board
    for (int i = 1; i <= size; ++i) {
        for (int j = 1; j <= size; ++j) {
            if (board[i][j] != 0) {
                board[i][j] = old_to_new[board[i][j]];
            }
        }
    }
}

void tileBoard(int tr, int tc, int pr, int pc, int size) {
    // Base Case: When we have a 2x2 grid, there's nothing more to divide.
    // The calling function has already placed the L-tromino that covers 3 of its squares.
    if (size == 1) {
        return;
    }

    // --- Divide Step ---
    int sub_size = size / 2;
    int current_id = base_id_counter++;

    // Determine which quadrant the special square is in and place the central L-tromino.

    // 1. Special square is in the Top-Left quadrant
    if (pr < tr + sub_size && pc < tc + sub_size) {
        // Place the central L-tromino to occupy one corner of the other three quadrants.
        board[tr + sub_size - 1][tc + sub_size] = current_id; // Corner of Top-Right
        board[tr + sub_size][tc + sub_size - 1] = current_id; // Corner of Bottom-Left
        board[tr + sub_size][tc + sub_size]     = current_id; // Corner of Bottom-Right

        // --- Conquer Step (Recurse on the four sub-grids) ---
        // The hole for each sub-problem is now defined. We pass their ABSOLUTE coordinates.
        tileBoard(tr, tc, pr, pc, sub_size);                                             // Top-Left (original hole)
        tileBoard(tr, tc + sub_size, tr + sub_size - 1, tc + sub_size, sub_size);         // Top-Right (new hole)
        tileBoard(tr + sub_size, tc, tr + sub_size, tc + sub_size - 1, sub_size);         // Bottom-Left (new hole)
        tileBoard(tr + sub_size, tc + sub_size, tr + sub_size, tc + sub_size, sub_size); // Bottom-Right (new hole)
    }
    // 2. Special square is in the Top-Right quadrant
    else if (pr < tr + sub_size && pc >= tc + sub_size) {
        board[tr + sub_size - 1][tc + sub_size - 1] = current_id; // Top-Left
        board[tr + sub_size][tc + sub_size - 1]     = current_id; // Bottom-Left
        board[tr + sub_size][tc + sub_size]         = current_id; // Bottom-Right

        tileBoard(tr, tc, tr + sub_size - 1, tc + sub_size - 1, sub_size);     // Top-Left (new hole)
        tileBoard(tr, tc + sub_size, pr, pc, sub_size);                        // Top-Right (original hole)
        tileBoard(tr + sub_size, tc, tr + sub_size, tc + sub_size - 1, sub_size); // Bottom-Left (new hole)
        tileBoard(tr + sub_size, tc + sub_size, tr + sub_size, tc + sub_size, sub_size); // Bottom-Right (new hole)
    }
    // 3. Special square is in the Bottom-Left quadrant
    else if (pr >= tr + sub_size && pc < tc + sub_size) {
        board[tr + sub_size - 1][tc + sub_size - 1] = current_id; // Top-Left
        board[tr + sub_size - 1][tc + sub_size]     = current_id; // Top-Right
        board[tr + sub_size][tc + sub_size]         = current_id; // Bottom-Right

        tileBoard(tr, tc, tr + sub_size - 1, tc + sub_size - 1, sub_size);     // Top-Left (new hole)
        tileBoard(tr, tc + sub_size, tr + sub_size - 1, tc + sub_size, sub_size); // Top-Right (new hole)
        tileBoard(tr + sub_size, tc, pr, pc, sub_size);                        // Bottom-Left (original hole)
        tileBoard(tr + sub_size, tc + sub_size, tr + sub_size, tc + sub_size, sub_size); // Bottom-Right (new hole)
    }
    // 4. Special square is in the Bottom-Right quadrant
    else {
        board[tr + sub_size - 1][tc + sub_size - 1] = current_id; // Top-Left
        board[tr + sub_size - 1][tc + sub_size]     = current_id; // Top-Right
        board[tr + sub_size][tc + sub_size - 1]     = current_id; // Bottom-Left

        tileBoard(tr, tc, tr + sub_size - 1, tc + sub_size - 1, sub_size);         // Top-Left (new hole)
        tileBoard(tr, tc + sub_size, tr + sub_size - 1, tc + sub_size, sub_size);     // Top-Right (new hole)
        tileBoard(tr + sub_size, tc, tr + sub_size, tc + sub_size - 1, sub_size);     // Bottom-Left (new hole)
        tileBoard(tr + sub_size, tc + sub_size, pr, pc, sub_size);                // Bottom-Right (original hole)
    }
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int k, p, q;
    cin >> k >> p >> q;

    int board_size = 1 << k; // Equivalent to pow(2, k)

    // Initialize the board with all zeros.
    board.resize(board_size + 1, vector<int>(board_size + 1, 0));

    // The problem uses 1-based indexing, so we use it directly.
    int headquarters_row = p;
    int headquarters_col = q;
    board[headquarters_row][headquarters_col] = 0; // Mark the HQ

    // Start the recursive tiling process on the whole board (using 1-based indices).
    tileBoard(1, 1, headquarters_row, headquarters_col, board_size);
    
    // Reassign IDs in the desired order (left-to-right, top-to-bottom)
    remapIds(board_size);

    // Print the final board layout.
    for (int i = 1; i <= board_size; ++i) {
        for (int j = 1; j <= board_size; ++j) {
            cout << board[i][j] << (j == board_size ? "" : " ");
        }
        cout << endl;
    }

    return 0;
}
