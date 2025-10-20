#include <stdio.h>
#include <stdint.h>
#include "bitops.c"  // since you’re not using .h files

int main() {
    uint64_t board = 0;

    printf("Initial board:\n");
    print_board(board);

    // Test SetBit
    board = set_bit(board, 3);
    printf("\nAfter setting bit 3:\n");
    print_board(board);

    // Test ClearBit
    board = clear_bit(board, 3);
    printf("\nAfter clearing bit 3:\n");
    print_board(board);

    // Test ToggleBit
    board = toggle_bit(board, 2);
    printf("\nAfter toggling bit 2:\n");
    print_board(board);
    board = toggle_bit(board, 2);
    printf("\nAfter toggling bit 2 again:\n");
    print_board(board);

    // Test CountBits
    board = set_bit(board, 1);
    board = set_bit(board, 5);
    printf("\nNumber of bits set: %d\n", count_bits(board));

    return 0;
}
