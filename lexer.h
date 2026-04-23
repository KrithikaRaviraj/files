#ifndef LEXER_H
#define LEXER_H

/* Token types */
typedef enum {
    /* Keywords */
    TOK_INT = 0,
    TOK_MAIN,
    TOK_BEGIN,
    TOK_END,
    TOK_WHILE,
    TOK_RETURN,
    /* Identifiers & Numbers */
    TOK_ID,
    TOK_NUM,
    /* Operators */
    TOK_ASSIGN,   /* = */
    TOK_GT,       /* > */
    TOK_PLUS,     /* + */
    TOK_DIV,      /* / */
    TOK_SEMI,     /* ; */
    /* Delimiters */
    TOK_LPAREN,   /* ( */
    TOK_RPAREN,   /* ) */
    /* End marker */
    TOK_DOLLAR,   /* $ */
    /* Special */
    TOK_EOF,
    TOK_ERROR,
    TOK_COUNT     /* total number of terminals (used for table sizing) */
} TokenType;

typedef struct {
    TokenType type;
    char      lexeme[64];
} Token;

/* Token stream (filled by run_lexer) */
#define MAX_TOKENS 512
extern Token token_stream[];
extern int   token_count;
extern int   token_pos;   /* current read position for parser */

/* Human-readable name for each token type */
const char *token_name(TokenType t);

/* Run lexer on file; returns 0 on success */
int run_lexer(const char *filename);

/* Peek / advance in token stream */
Token peek_token(void);
Token next_token(void);

#endif /* LEXER_H */