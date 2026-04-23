#ifndef SEMANTIC_H
#define SEMANTIC_H

/* Symbol table entry */
typedef struct {
    char name[32];
    int  value;
    int  defined;
} Symbol;

#define MAX_SYMBOLS 32

extern Symbol sym_table[];
extern int    sym_count;

/* Declare/set a symbol */
void sym_declare(const char *name);
void sym_set(const char *name, int value);
int  sym_get(const char *name);

/* Print current symbol table */
void sym_print(void);

/* Execute the binary digit count algorithm */
void execute_program(void);

#endif /* SEMANTIC_H */