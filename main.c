#include <stdio.h>
#include "lexer.h"
#include "slr_parser.h"
#include "semantic.h"

int main(void) {
    printf("==============================================================================\n");
    printf("     Q7: SLR PARSER - Number of Digits in Binary Representation               \n");
    printf("==============================================================================\n");

    if (run_lexer("input.txt") != 0) {
        fprintf(stderr, "Lexer failed. Exiting.\n");
        return 1;
    }

    slr_init();

    print_first_follow();
    print_lr0_items();
    print_slr_table();

    if (slr_parse()) {
        execute_program();
    } else {
        printf("\nSemantic execution skipped due to syntax error.\n");
        return 1;
    }

    printf("\n==============================================================================\n");
    return 0;
}
