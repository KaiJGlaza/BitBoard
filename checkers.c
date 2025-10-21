#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "bitops.c" // for bit functions

// use "-> for the game to point to red and black"

// basically a constructor
typedef struct {
    uint64_t red;
    uint64_t black;
    uint64_t red_kings;
    uint64_t black_kings;
    int turn;  // 0 = red, 1 = black
} GameState;

// ---------- HELPER FUNCTIONS ----------

/**
 * Count total pieces for a player by adding regular pieces and kings
 * Uses count_bits from bitops.c to count set bits in each bitboard
 */
int total_pieces(uint64_t a, uint64_t b) {
    return count_bits(a) + count_bits(b);
}

/**
 * Print the current game state as an 8x8 board
 * Shows: r = red piece, b = black piece, R = red king, B = black king, . = empty
 * Displays row numbers on left, column numbers on top
 */
void print_game(GameState *g) {
    printf("\n    0 1 2 3 4 5 6 7\n");
    for (int row = 7; row >= 0; row--) {
        printf("%d | ", row);
        for (int col = 0; col < 8; col++) {
            int pos = row * 8 + col;
            char ch = '.';
            if (get_bit(g->red, pos)) ch = 'r';
            if (get_bit(g->black, pos)) ch = 'b';
            if (get_bit(g->red_kings, pos)) ch = 'R';
            if (get_bit(g->black_kings, pos)) ch = 'B';
            printf("%c ", ch);
        }
        printf("\n");
    }
    printf("Turn: %s\n\n", g->turn == 0 ? "Red" : "Black");
}

/**
 * Initialize the board to starting checkers position
 * Red pieces on rows 0-2 (bottom), black pieces on rows 5-7 (top)
 * Only dark squares (where row + col is odd) get pieces
 */
void init_board(GameState *g) {
    g->red = g->black = g->red_kings = g->black_kings = 0;
    g->turn = 0;

    // rows 0–2 red
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 8; c++) {
            if ((r + c) % 2 == 1)
                g->red = set_bit(g->red, r * 8 + c);
        }
    }

    // rows 5–7 black
    for (int r = 5; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if ((r + c) % 2 == 1)
                g->black = set_bit(g->black, r * 8 + c);
        }
    }
}

/**
 * Check if position is on the board (0-63)
 */
int on_board(int pos) { return pos >= 0 && pos < 64; }

/**
 * Check if a position is a dark square (playable in checkers)
 * Dark squares have odd sum of row + col
 */
int dark_square(int pos) { 
    int r = pos / 8, c = pos % 8; 
    return ((r + c) % 2) == 1; 
}

/**
 * Check if any piece (red, black, or kings) occupies this position
 * Uses get_bit to check all four bitboards
 */
int occupied(GameState *g, int pos) {
    return get_bit(g->red, pos) || get_bit(g->black, pos) || get_bit(g->red_kings, pos) || get_bit(g->black_kings, pos);
}

/**
 * Check if the game is over and return the winner
 * Returns: 0 = game continues, 1 = black wins, 2 = red wins
 * Player loses when they have no pieces left
 */
int check_winner(GameState *g) {
    int red_total = total_pieces(g->red, g->red_kings);
    int black_total = total_pieces(g->black, g->black_kings);
    if (red_total == 0) return 1;
    if (black_total == 0) return 2;
    return 0;
}

/**
 * Promote pieces to kings when they reach the opposite end
 * Red pieces on top row (row 7) become red kings
 * Black pieces on bottom row (row 0) become black kings
 * Moves promoted pieces from regular bitboard to king bitboard
 */
void promote(GameState *g) {
    uint64_t top_row = 0xFF00000000000000ULL; // Hexa for 56 - 63: THE TOP ROW
    uint64_t bottom_row = 0x00000000000000FFULL; // Hexa for the 0 0 - 7: THE BOTTOM ROW
    // Mash all the boards together basically
    uint64_t new_red_kings = g->red & top_row;
    g->red_kings |= new_red_kings; g->red &= ~new_red_kings; // mash together
    uint64_t new_black_kings = g->black & bottom_row;
    g->black_kings |= new_black_kings; g->black &= ~new_black_kings; // mash together
}

/**
 * Check if there's an opponent piece at the given position
 * red_turn = 1: look for black pieces, red_turn = 0: look for red pieces
 */
int opposite_piece(GameState *g, int pos, int red_turn) {
    if (!on_board(pos)) return 0;
    if (red_turn)
        return get_bit(g->black, pos) || get_bit(g->black_kings, pos);
    else
        return get_bit(g->red, pos) || get_bit(g->red_kings, pos);
}

/**
 * Execute a move on the board (assumes move is valid)
 * 1. Determine if this is a capture move by checking jump distance (14 or 18) as only diagonals possible
 * 2. Move the piece from 'from' to 'to' using clear_bit and set_bit
 * 3. If capture, remove the jumped opponent piece at the midpoint
 * 4. Check for and apply promotions
 */
void move_piece(GameState *g, int from, int to) {
    int is_red_turn = g->turn == 0;
    int captured = (to - from == 14) || (to - from == 18) || (to - from == -14) || (to - from == -18);

    if (is_red_turn) {
        if (get_bit(g->red, from)) { 
            g->red = clear_bit(g->red, from); g->red = set_bit(g->red, to); 
        } else if (get_bit(g->red_kings, from)) { 
            g->red_kings = clear_bit(g->red_kings, from); g->red_kings = set_bit(g->red_kings, to); 
        }
    } else {
        if (get_bit(g->black, from)) { 
            g->black = clear_bit(g->black, from); g->black = set_bit(g->black, to); 
        } else if (get_bit(g->black_kings, from)) { 
            g->black_kings = clear_bit(g->black_kings, from); g->black_kings = set_bit(g->black_kings, to); 
        }
    }

    if (captured) {
        int jumped = (from + to) / 2;
        if (is_red_turn) {
            if (get_bit(g->black, jumped)) {
                g->black = clear_bit(g->black, jumped);
            } else if (get_bit(g->black_kings, jumped)) {
                g->black_kings = clear_bit(g->black_kings, jumped);
            }
        } else {
            if (get_bit(g->red, jumped)) {
                g->red = clear_bit(g->red, jumped);
            }
            else if (get_bit(g->red_kings, jumped)) {
                 g->red_kings = clear_bit(g->red_kings, jumped);
            }
        }
    }

    promote(g);
}

/**
 * Check if a piece at 'pos' can make another capture move
 * Used to enforce mandatory consecutive jumps in checkers
 * Checks all four diagonal jump directions (14, 18, -14, -18)
 * Kings can jump in all directions, regular pieces only forward
 */
int can_capture_again(GameState *g, int pos) {
    int king = get_bit(g->red_kings, pos) || get_bit(g->black_kings, pos); // King check
    int is_red_turn = g->turn == 0;

    int directions[4] = {14, 18, -14, -18};
    for (int i = 0; i < 4; i++) {
        int diff = directions[i];
        int to = pos + diff;
        int over = (pos + to) / 2;

        if (!on_board(to) || !dark_square(to)) continue;
        if (occupied(g, to)) continue;

        if (is_red_turn && !king && diff < 0) continue;   // red men only go up
        if (!is_red_turn && !king && diff > 0) continue;  // black men only go down

        if (opposite_piece(g, over, is_red_turn)) return 1;
    }
    return 0;
}

/**
 * Validate that a move is a legal capture from 'from' to 'to'
 * Used specifically for consecutive jump validation
 * Checks: position validity, dark square, not occupied, correct jump distance,
 * direction rules for non-kings, and presence of opponent piece to jump
 */
int is_capture_move_valid(GameState *g, int from, int to) {
    if (!on_board(from) || !on_board(to)) return 0;
    if (!dark_square(to) || occupied(g, to)) return 0;

    int diff = to - from;
    if (!(abs(diff) == 14 || abs(diff) == 18)) return 0;

    int red_turn = g->turn == 0;
    int king = get_bit(g->red_kings, from) || get_bit(g->black_kings, from);

    if (red_turn && !king && diff < 0) return 0;   // red men only go up
    if (!red_turn && !king && diff > 0) return 0;  // black men only go down

    int over = (from + to) / 2;
    if (!opposite_piece(g, over, red_turn)) return 0;

    return 1;
}

/**
 * Check if a move from 'from' to 'to' is valid
 * Validates: board bounds, dark square, destination empty
 * Simple moves: diff of 7 or 9 (diagonal one square)
 * Capture moves: diff of 14 or 18 (diagonal two squares, jumping opponent)
 * Kings can move/capture in all directions, regular pieces only forward
 * 
 * -- Does NOT validate presence of opponent piece for captures --
 */
int valid_move(GameState *g, int from, int to) {
    if (!on_board(from) || !on_board(to)) return 0;
    if (!dark_square(to) || occupied(g, to)) return 0;

    int diff = to - from;
    int red_turn = g->turn == 0;
    int king = get_bit(g->red_kings, from) || get_bit(g->black_kings, from);

    // simple steps and captures
    if (red_turn) {
        if (diff == 7 || diff == 9 || diff == 14 || diff == 18) return 1;
        if (king && (diff == -7 || diff == -9 || diff == -14 || diff == -18)) return 1;
    } else {
        if (diff == -7 || diff == -9 || diff == -14 || diff == -18) return 1;
        if (king && (diff == 7 || diff == 9 || diff == 14 || diff == 18)) return 1;
    }
    return 0;
}

// ---------- MAIN GAME LOOP ----------

/**
 * Main game loop that handles all gameplay
 * 1. Display board and check for winner each turn
 * 2. Get move input from current player (format: from_row from_col to_row to_col)
 * 3. Validate and execute the move
 * 4. If capture was made, check for and handle consecutive jumps with same piece
 * 5. Switch turns after move is complete
 * Loop continues until game ends or player quits with -1
 * ---------------------------------------------------------------------------------
 * ---- SORRY FOR LITIGOUS COMMENTING IT HELPS WITH UNDERSTANDING PROCESS A LOT ----
 * ---------------------------------------------------------------------------------
 */
void play_game() {
    GameState g;
    init_board(&g);

    while (1) {
        print_game(&g); // Print the entire board each time before the move (lets player see updates)

        int win = check_winner(&g); // Quick check to see if either side has no pieces left
        if (win == 1) { printf("Black wins!\n"); break; } // Don't waste time - black has all the pieces
        if (win == 2) { printf("Red wins!\n"); break; }   // Don't waste time - red has all the pieces

        int fr, fc, tr, tc; // Variables for FROM row/col and TO row/col

        // Print whose turn it is and ask for coordinates (example: 2 1 3 2)
        printf("%s move (row col row col, or JUST -1 to quit): ",
               g.turn == 0 ? "Red" : "Black");

        // Input section – read the "from row" first
        if (scanf("%d", &fr) != 1) break; // If input fails (like EOF), stop the game
        if (fr == -1) break;              // If they enter -1, quit right away
        if (scanf("%d %d %d", &fc, &tr, &tc) != 3) break; // Read the other three ints (from col, to row, to col)

        // Make sure input is actually inside the board (0–7 range for both row and column)
        if (fr < 0 || fr > 7 || fc < 0 || fc > 7 ||
            tr < 0 || tr > 7 || tc < 0 || tc > 7) {
            printf("Coordinates must be 0–7.\n"); // Error catch - outside board bounds
            continue; // Go back to start of loop and let them try again
        }

        // Convert the 2D row/col coordinates to a 1D bitboard indexing (row * 8 + col)
        int from = fr * 8 + fc;
        int to = tr * 8 + tc;

        // Validate move using rules - checks direction, distance, kings, etc.
        if (!valid_move(&g, from, to)) {
            printf("Invalid move. Try again.\n"); // Invalid - not a legal checkers move
            continue; // Skip to next loop iteration
        }

        // Determine if the move is a capture by checking distance (14 or 18 = jump)
        int was_capture = (abs(to - from) == 14 || abs(to - from) == 18);

        // Actually perform the move (updates board, removes captured piece, promotes kings)
        move_piece(&g, from, to);

        // --- MULTI-JUMP HANDLING ---
        // If the player just made a capture, check if the same piece can jump again
        if (was_capture) {
            int cur_pos = to; // Track where that piece ended up
            while (can_capture_again(&g, cur_pos)) { // While another jump is possible...
                print_game(&g); // Reprint board so they can see the new position
                printf("You can capture again with the same piece at (%d,%d). Enter next jump (row col): ",
                       cur_pos / 8, cur_pos % 8);

                int ntr, ntc; // new TO row/col
                if (scanf("%d %d", &ntr, &ntc) != 2) { // If input fails, stop safely
                    printf("Input ended.\n");
                    return;
                }

                // Validate input bounds again
                if (ntr < 0 || ntr > 7 || ntc < 0 || ntc > 7) {
                    printf("Coordinates must be 0–7.\n");
                    continue;
                }

                // Convert new row/col to bit position
                int next_to = ntr * 8 + ntc;

                // Make sure the move is actually a valid *capture* from the same piece
                if (!is_capture_move_valid(&g, cur_pos, next_to)) {
                    printf("That is not a valid capture from (%d,%d). Try again.\n",
                           cur_pos/8, cur_pos%8);
                    continue;
                }

                // Perform this next capture move
                move_piece(&g, cur_pos, next_to);

                // Update the piece’s current position in case another jump is available
                cur_pos = next_to;
            }
        }

        // Once no more jumps are available, or it wasn’t a capture move,
        // switch to the other player's turn (0 ↔ 1)
        g.turn = 1 - g.turn;
    }
}

/**
 * Entry point - just prints intro messages and starts play_game()
 * (User instructions are printed here once at startup)
 */
int main() {
    printf("Welcome to BitBoard Checkers!\n"); // Friendly intro
    printf("Move format: from_row from_col to_row to_col (0–7).\n"); // Explain how to move
    printf("Example: 1 0 2 1 moves piece from row1 col0 → row2 col1.\n"); // Example move
    printf("After a capture, the same piece must continue jumping if possible.\n"); // Explain consecutive jump rule
    play_game(); // Run the full game loop above
    return 0; // Exit cleanly
}