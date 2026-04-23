#include <stdio.h>
#include <string.h>
#include "semantic.h"

Symbol sym_table[MAX_SYMBOLS];
int    sym_count = 0;

void sym_declare(const char *name) {
    for (int i = 0; i < sym_count; i++)
        if (strcmp(sym_table[i].name, name) == 0) return; /* already declared */
    strncpy(sym_table[sym_count].name, name, 31);
    sym_table[sym_count].value   = 0;
    sym_table[sym_count].defined = 0;
    sym_count++;
}

void sym_set(const char *name, int value) {
    for (int i = 0; i < sym_count; i++) {
        if (strcmp(sym_table[i].name, name) == 0) {
            sym_table[i].value   = value;
            sym_table[i].defined = 1;
            return;
        }
    }
    /* auto-declare if missing */
    sym_declare(name);
    sym_set(name, value);
}

int sym_get(const char *name) {
    for (int i = 0; i < sym_count; i++)
        if (strcmp(sym_table[i].name, name) == 0)
            return sym_table[i].value;
    return 0;
}

void sym_print(void) {
    printf("\n========================= SYMBOL TABLE =======================================\n");
    printf("%-20s  %-10s  %s\n", "NAME", "VALUE", "STATUS");
    printf("----------------------------------------------------------------------\n");
    for (int i = 0; i < sym_count; i++) {
        printf("%-20s  %-10d  %s\n",
               sym_table[i].name,
               sym_table[i].value,
               sym_table[i].defined ? "defined" : "declared");
    }
}

void execute_program(void) {
    printf("\n========================= SEMANTIC EXECUTION =================================\n");

    /* Initialise symbol table with declarations from the source */
    sym_declare("n");
    sym_declare("count");

    int n;
    printf("Enter a decimal number: ");
    if (scanf("%d", &n) != 1) { printf("Invalid input.\n"); return; }

    sym_set("n", n);

    /* Execution logic mirroring the parsed program:
     *   count = 1
     *   while (n > 1) { count = count + 1; n = n / 2; }
     */
    int count = 1;
    sym_set("count", count);

    printf("\nExecution trace:\n");
    printf("  Initial: n = %d, count = %d\n", sym_get("n"), sym_get("count"));

    int iter = 0;
    while (sym_get("n") > 1) {
        count = sym_get("count") + 1;
        n     = sym_get("n")     / 2;
        sym_set("count", count);
        sym_set("n",     n);
        iter++;
        printf("  Iter %2d : n = %d, count = %d\n", iter, n, count);
    }

    sym_print();

    printf("\nNumber of binary digits = %d\n", sym_get("count"));
}