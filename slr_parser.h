#ifndef SLR_PARSER_H
#define SLR_PARSER_H

typedef enum {
    NT_SPRIME = 0,
    NT_PROGRAM,
    NT_BLOCK,
    NT_DECLS,
    NT_DECL,
    NT_STMTS,
    NT_STMT,
    NT_ASSIGN,
    NT_WHILE_STMT,
    NT_RETURN_STMT,
    NT_EXPR,
    NT_TERM,
    NT_FACTOR,
    NT_WHILEBLOCK,
    NT_COND,
    NT_COUNT
} NonTerminal;

void slr_init(void);
void print_first_follow(void);
void build_lr0_items(void);
void print_lr0_items(void);
void build_slr_table(void);
void print_slr_table(void);
int slr_parse(void);

#endif /* SLR_PARSER_H */
