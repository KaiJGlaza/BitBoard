#include <stdio.h>
#include <stdint.h>
#include <string.h>

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
 * Shift the whole board to the left by @pos
 */
uint64_t shift_left(uint64_t board, int pos) {
    if (pos < 0 || pos >= 64) return board; // Error catch
    return board << pos;
}

/**
 * Shift the whole board to the right by @pos
 */
uint64_t shift_right(uint64_t value, int pos) {
    if (pos < 0 || pos >= 64) return value; // Error catch
    return value >> pos;
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
    for (int row = 7; row >= 0; row--) {        // Start at the top
        for (int col = 0; col < 8; col++) {     // to 8 because 7 * 8 = 63, last spot
            int pos = row * 8 + col;            // get position
            printf("%d ", get_bit(board, pos)); // Print the current bit using get_bit
        }
        printf("\n");                           // Clean line
    }
}

/**
 * Ripped from OwlTechA1 to convert binary to hexadecimal
 * 
 * modified for the 1ULL size
 */
void div_convert(uint64_t n, int base, char *out) {
    char temp [65]; // 64 bits
    int pos = 0;

    if (n == 0) { // Error catch if 0
        strcpy(out, "0"); 
        return;
    }

    while (n > 0) {
        int remainder = n % base; 
        n = n / base;

        if (remainder < 10)
            temp[pos++] = (char)('0' + remainder); // it kept giving errors so I wrapped
        else // Hexadecimal
            temp[pos++] = (char)('A' + (remainder - 10)); // Hexadecimal letters only start at 10
    }
    // Starting at the end (pos - 1) and working backwards
    for (int i = 0; i < pos; i++) {
        out[i] = temp[pos - i - 1];
    }

    out[pos] = '\0'; // Adds terminator at the end regardless of base
}

// Prints the board in binary (grouped every 4 bits)
void print_binary(uint64_t value) {
    for (int i = 63; i >= 0; i--) {
        printf("%d", get_bit(value, i));
        if (i % 4 == 0 && i != 0) printf(" ");
    }
    printf("\n");
}

/**
 * Print 64-bit board in hex, single line,
 * with 1 space per hex digit (4 bits per group)
 */
void print_hex(uint64_t board) {
    char hex[17];
    div_convert(board, 16, hex);

    // pad with leading zeros to make 16 digits
    int len = strlen(hex);
    int pad = 16 - len;

    for (int i = 0; i < pad; i++) {
        printf("0 ");
    }

    for (int i = 0; i < len; i++) {
        printf("%c ", hex[i]);
    }

    printf("\n");
}

