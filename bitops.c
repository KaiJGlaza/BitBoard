#include <stdio.h>
#include <stdint.h>
// 1ULL - A 64 bit int that matches the board perfectly - -<@THIS IS A BLANK BOARD@>- use it

/**
 * Return the board with the bitshift performed, keeping the board the same plus the new bit change
 * Only capable of flipping 0 to 1
 * 
 * The | operator lets us keep the board so – return the board as it was before and with the new change
 */
uint64_t set_bit(uint64_t board, int pos) {
    return board | (1ULL << pos);
}

/**
 * Return the board with the exact opposite of set_bit, still keeping the board the same but instead
 * the blank board we generate puts a zero at the spot we want cleared and the & operator compares the original
 * and the new board and only keeps values where both are 1
 * Only capable of 1 to 0 
 * 
 * TLDR - put a zero at the spot we want cleared and we put them side by side and only keep spots as a 1 if both boards have a 1 there 
 */
uint64_t clear_bit(uint64_t board, int pos) {
    return board & ~(1ULL << pos);
}

/**
 * set_bit but with XOR, so we are garunteed to change the bit from either 1 to 0, or 0 to 1 
 */
uint64_t toggle_bit(uint64_t board, int pos) {
    return board ^ (1ULL << pos);
}

/**
 * Move the bit that we wanna get into the first position (0), so if we want slot 4 we move everything to the beginning s.t. slot 4
 * is now at slot 0. 1ULL has 1 at the 0 position by default, SO we end up returning just the value at 0 which if it was 1 in board we get 1
 * and the same for 0
 * 
 * TLDR - Force bit to 0 spot and then check it against a blank board with 1 at its 0 and you end up returning what the bit at that spot was
 */
int get_bit(uint64_t board, int pos) {
    return (board >> pos) & 1ULL;
}

// Count how many bits are set to 1 
int count_bits(uint64_t board) {
    int count = 0;
    while (board) {
        count += board & 1ULL;  // If it is a 1, then we can increase count
        board >>= 1;            // Move everything by 1 (This is okay because its a local reference)
    }
    return count;
}

// Print the board as an 8x8 grid
void print_board(uint64_t board) {
}