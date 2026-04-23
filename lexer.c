#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

Token token_stream[MAX_TOKENS];
int   token_count = 0;
int   token_pos   = 0;

/* ------------------------------------------------------------------ */
/* Token name table                                                     */
/* ------------------------------------------------------------------ */
static const char *tok_names[] = {
    "int","main","begin","end","while","return",
    "ID","NUM",
    "=",">","+","/",";",
    "(",")",
    "$","EOF","ERROR"
};

const char *token_name(TokenType t) {
    if (t < 0 || t >= TOK_COUNT) return "?";
    return tok_names[t];
}

/* ------------------------------------------------------------------ */
/* Keyword lookup                                                        */
/* ------------------------------------------------------------------ */
static TokenType keyword_type(const char *s) {
    if (strcmp(s,"int")   ==0) return TOK_INT;
    if (strcmp(s,"main")  ==0) return TOK_MAIN;
    if (strcmp(s,"begin") ==0) return TOK_BEGIN;
    if (strcmp(s,"end")   ==0) return TOK_END;
    if (strcmp(s,"while") ==0) return TOK_WHILE;
    if (strcmp(s,"return")==0) return TOK_RETURN;
    return TOK_ID;
}

/* ------------------------------------------------------------------ */
/* run_lexer                                                             */
/* ------------------------------------------------------------------ */
int run_lexer(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { perror("Cannot open input.txt"); return -1; }

    int ch;
    token_count = 0;

    printf("\n============================== LEXICAL ANALYSIS ==============================\n");
    printf("%-20s  %s\n", "LEXEME", "TOKEN");
    printf("------------------------------------------------------------------------------\n");

    while ((ch = fgetc(fp)) != EOF) {
        if (isspace(ch)) continue;

        Token tok;
        memset(&tok, 0, sizeof(tok));

        /* --- identifier or keyword --- */
        if (isalpha(ch) || ch == '_') {
            int i = 0;
            tok.lexeme[i++] = (char)ch;
            while ((ch = fgetc(fp)) != EOF && (isalnum(ch) || ch == '_'))
                tok.lexeme[i++] = (char)ch;
            tok.lexeme[i] = '\0';
            if (ch != EOF) ungetc(ch, fp);
            tok.type = keyword_type(tok.lexeme);
        }
        /* --- number --- */
        else if (isdigit(ch)) {
            int i = 0;
            tok.lexeme[i++] = (char)ch;
            while ((ch = fgetc(fp)) != EOF && isdigit(ch))
                tok.lexeme[i++] = (char)ch;
            tok.lexeme[i] = '\0';
            if (ch != EOF) ungetc(ch, fp);
            tok.type = TOK_NUM;
        }
        /* --- single-char tokens --- */
        else {
            tok.lexeme[0] = (char)ch;
            tok.lexeme[1] = '\0';
            switch (ch) {
                case '=': tok.type = TOK_ASSIGN; break;
                case '>': tok.type = TOK_GT;     break;
                case '+': tok.type = TOK_PLUS;   break;
                case '/': tok.type = TOK_DIV;    break;
                case ';': tok.type = TOK_SEMI;   break;
                case '(': tok.type = TOK_LPAREN; break;
                case ')': tok.type = TOK_RPAREN; break;
                default:
                    tok.type = TOK_ERROR;
                    fprintf(stderr, "Unknown character '%c'\n", ch);
            }
        }

        printf("%-20s  %s\n", tok.lexeme, token_name(tok.type));

        if (token_count < MAX_TOKENS)
            token_stream[token_count++] = tok;
    }

    /* append $ */
    Token eof_tok = { TOK_DOLLAR, "$" };
    token_stream[token_count++] = eof_tok;
    printf("%-20s  %s\n", "$", "$");

    printf("------------------------------------------------------------------------------\n");
    printf("Total tokens (excluding $): %d\n", token_count - 1);

    fclose(fp);
    return 0;
}

/* ------------------------------------------------------------------ */
/* Token stream navigation                                              */
/* ------------------------------------------------------------------ */
Token peek_token(void) {
    if (token_pos < token_count) return token_stream[token_pos];
    Token t = { TOK_EOF, "" };
    return t;
}

Token next_token(void) {
    Token t = peek_token();
    if (token_pos < token_count) token_pos++;
    return t;
}