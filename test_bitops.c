#include <stdio.h>
#include <stdint.h>
#include "bitops.c" 

int main() {
    uint64_t board = 0;

    printf("Initial board:\n");
    print_board(board);

    // --- Test SetBit ---
    board = set_bit(board, 3);
    printf("\nAfter setting bit 3:\n");
    print_board(board);
    printf("Binary: ");
    print_binary(board);
    printf("Hex: ");
    print_hex(board);

    // --- Test ClearBit ---
    board = clear_bit(board, 3);
    printf("\nAfter clearing bit 3:\n");
    print_board(board);
    printf("Binary: ");
    print_binary(board);
    printf("Hex: ");
    print_hex(board);

    // --- Test ToggleBit ---
    board = toggle_bit(board, 2);
    printf("\nAfter toggling bit 2:\n");
    print_board(board);
    printf("Binary: ");
    print_binary(board);
    printf("Hex: ");
    print_hex(board);

    board = toggle_bit(board, 2);
    printf("\nAfter toggling bit 2 again:\n");
    print_board(board);
    printf("Binary: ");
    print_binary(board);
    printf("Hex: ");
    print_hex(board);

    // --- Test CountBits ---
    board = set_bit(board, 1);
    board = set_bit(board, 5);
    printf("\nNumber of bits set: %d\n", count_bits(board));
    printf("Binary: ");
    print_binary(board);
    printf("Hex: ");
    print_hex(board);

    // --- Test ShiftLeft ---
    board = shift_left(board, 2);
    printf("\nAfter shifting left 2 positions:\n");
    print_board(board);
    printf("Binary: ");
    print_binary(board);
    printf("Hex: ");
    print_hex(board);

    // --- Test ShiftRight ---
    board = shift_right(board, 1);
    printf("\nAfter shifting right 1 position:\n");
    print_board(board);
    printf("Binary: ");
    print_binary(board);
    printf("Hex: ");
    print_hex(board);

    return 0;
}
