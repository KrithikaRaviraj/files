/*
 * slr_parser.c
 *
 * Corrected grammar (augmented):
 *   0.  S'          -> Program
 *   1.  Program     -> int main ( ) Block
 *   2.  Block       -> begin Decls Stmts end
 *   3.  Decls       -> Decls Decl
 *   4.  Decls       -> Decl
 *   5.  Decl        -> int ID ;
 *   6.  Stmts       -> Stmts Stmt
 *   7.  Stmts       -> Stmt
 *   8.  Stmt        -> Assign
 *   9.  Stmt        -> While_stmt
 *  10.  Stmt        -> Return_stmt
 *  11.  Assign      -> ID = Expr ;
 *  12.  Expr        -> Expr + Term
 *  13.  Expr        -> Term
 *  14.  Term        -> Term / Factor
 *  15.  Term        -> Factor
 *  16.  Factor      -> ID
 *  17.  Factor      -> NUM
 *  18.  While_stmt  -> while ( Cond ) WhileBlock
 *  19.  WhileBlock  -> begin Stmts end
 *  20.  Cond        -> ID > NUM
 *  21.  Return_stmt -> return ID ;
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "slr_parser.h"

#define NUM_TERMS   ((int)TOK_COUNT)
#define NUM_NT      ((int)NT_COUNT)
#define SYM_NT(n)   (NUM_TERMS + (int)(n))
#define IS_NT(s)    ((s) >= NUM_TERMS)
#define GET_NT(s)   ((s) - NUM_TERMS)

/* ---- Productions ---- */
#define NUM_PRODS 22
typedef struct { int lhs; int rhs[10]; int rhs_len; const char *text; } Production;
static Production prods[NUM_PRODS];

static void init_productions(void){
#define T(x) ((int)(x))
#define N(x) SYM_NT(x)
#define P(i,L,txt,...) do{ int _r[]={__VA_ARGS__,-1}; prods[i].lhs=N(L); \
    prods[i].rhs_len=(int)(sizeof(_r)/sizeof(_r[0]))-1; \
    memcpy(prods[i].rhs,_r,sizeof(_r)); prods[i].text=txt; }while(0)

    P( 0,NT_SPRIME,     "S' -> Program",                   N(NT_PROGRAM));
    P( 1,NT_PROGRAM,    "Program -> int main ( ) Block",   T(TOK_INT),T(TOK_MAIN),T(TOK_LPAREN),T(TOK_RPAREN),N(NT_BLOCK));
    P( 2,NT_BLOCK,      "Block -> begin Decls Stmts end",  T(TOK_BEGIN),N(NT_DECLS),N(NT_STMTS),T(TOK_END));
    P( 3,NT_DECLS,      "Decls -> Decls Decl",             N(NT_DECLS),N(NT_DECL));
    P( 4,NT_DECLS,      "Decls -> Decl",                   N(NT_DECL));
    P( 5,NT_DECL,       "Decl -> int ID ;",                T(TOK_INT),T(TOK_ID),T(TOK_SEMI));
    P( 6,NT_STMTS,      "Stmts -> Stmts Stmt",             N(NT_STMTS),N(NT_STMT));
    P( 7,NT_STMTS,      "Stmts -> Stmt",                   N(NT_STMT));
    P( 8,NT_STMT,       "Stmt -> Assign",                  N(NT_ASSIGN));
    P( 9,NT_STMT,       "Stmt -> While_stmt",              N(NT_WHILE_STMT));
    P(10,NT_STMT,       "Stmt -> Return_stmt",             N(NT_RETURN_STMT));
    P(11,NT_ASSIGN,     "Assign -> ID = Expr ;",           T(TOK_ID),T(TOK_ASSIGN),N(NT_EXPR),T(TOK_SEMI));
    P(12,NT_EXPR,       "Expr -> Expr + Term",             N(NT_EXPR),T(TOK_PLUS),N(NT_TERM));
    P(13,NT_EXPR,       "Expr -> Term",                    N(NT_TERM));
    P(14,NT_TERM,       "Term -> Term / Factor",           N(NT_TERM),T(TOK_DIV),N(NT_FACTOR));
    P(15,NT_TERM,       "Term -> Factor",                  N(NT_FACTOR));
    P(16,NT_FACTOR,     "Factor -> ID",                    T(TOK_ID));
    P(17,NT_FACTOR,     "Factor -> NUM",                   T(TOK_NUM));
    P(18,NT_WHILE_STMT, "While_stmt -> while ( Cond ) WhileBlock", T(TOK_WHILE),T(TOK_LPAREN),N(NT_COND),T(TOK_RPAREN),N(NT_WHILEBLOCK));
    P(19,NT_WHILEBLOCK, "WhileBlock -> begin Stmts end",   T(TOK_BEGIN),N(NT_STMTS),T(TOK_END));
    P(20,NT_COND,       "Cond -> ID > NUM",                T(TOK_ID),T(TOK_GT),T(TOK_NUM));
    P(21,NT_RETURN_STMT,"Return_stmt -> return ID ;",      T(TOK_RETURN),T(TOK_ID),T(TOK_SEMI));
#undef P
#undef T
#undef N
}

/* ---- FIRST / FOLLOW ---- */
static int FIRST [NUM_NT][NUM_TERMS];
static int FOLLOW[NUM_NT][NUM_TERMS];
static int nullable_nt[NUM_NT];

static const char *nt_names[]={"S'","Program","Block","Decls","Decl","Stmts","Stmt",
    "Assign","While_stmt","Return_stmt","Expr","Term","Factor","WhileBlock","Cond"};

static void compute_first_follow(void){
    memset(FIRST,0,sizeof(FIRST));
    memset(FOLLOW,0,sizeof(FOLLOW));
    memset(nullable_nt,0,sizeof(nullable_nt));
    int ch=1;
    while(ch){
        ch=0;
        for(int p=0;p<NUM_PRODS;p++){
            int lhs=GET_NT(prods[p].lhs);
            for(int i=0;i<prods[p].rhs_len;i++){
                int sym=prods[p].rhs[i];
                if(!IS_NT(sym)){if(!FIRST[lhs][sym]){FIRST[lhs][sym]=1;ch=1;}break;}
                int nt=GET_NT(sym);
                for(int t=0;t<NUM_TERMS;t++) if(FIRST[nt][t]&&!FIRST[lhs][t]){FIRST[lhs][t]=1;ch=1;}
                if(!nullable_nt[nt])break;
            }
        }
    }
    FOLLOW[NT_SPRIME][TOK_DOLLAR]=1;
    ch=1;
    while(ch){
        ch=0;
        for(int p=0;p<NUM_PRODS;p++){
            int lhs=GET_NT(prods[p].lhs);
            for(int i=0;i<prods[p].rhs_len;i++){
                int sym=prods[p].rhs[i];
                if(!IS_NT(sym))continue;
                int B=GET_NT(sym);
                int an=1;
                for(int j=i+1;j<prods[p].rhs_len;j++){
                    int s2=prods[p].rhs[j];
                    if(!IS_NT(s2)){if(!FOLLOW[B][s2]){FOLLOW[B][s2]=1;ch=1;}an=0;break;}
                    int n2=GET_NT(s2);
                    for(int t=0;t<NUM_TERMS;t++) if(FIRST[n2][t]&&!FOLLOW[B][t]){FOLLOW[B][t]=1;ch=1;}
                    if(!nullable_nt[n2]){an=0;break;}
                }
                if(an) for(int t=0;t<NUM_TERMS;t++) if(FOLLOW[lhs][t]&&!FOLLOW[B][t]){FOLLOW[B][t]=1;ch=1;}
            }
        }
    }
}

void print_first_follow(void){
    printf("\n============================== FIRST SETS =====================================\n");
    for(int n=0;n<NUM_NT;n++){
        printf("FIRST(%-14s) = { ",nt_names[n]);
        int fi=1;
        for(int t=0;t<NUM_TERMS;t++) if(FIRST[n][t]){if(!fi)printf(", ");printf("%s",token_name(t));fi=0;}
        printf(" }\n");
    }
    printf("\n============================== FOLLOW SETS ====================================\n");
    for(int n=0;n<NUM_NT;n++){
        printf("FOLLOW(%-13s) = { ",nt_names[n]);
        int fi=1;
        for(int t=0;t<NUM_TERMS;t++) if(FOLLOW[n][t]){if(!fi)printf(", ");printf("%s",token_name(t));fi=0;}
        printf(" }\n");
    }
}

/* ---- LR(0) Items ---- */
typedef struct{int prod;int dot;}Item;
#define MAX_ITEMS 512
#define MAX_STATES 512

typedef struct{Item items[MAX_ITEMS];int n;}State;
static State states[MAX_STATES];
static int   n_states=0;
static int   goto_raw[MAX_STATES][NUM_TERMS+NUM_NT];

static int add_item(State*s,Item it){
    for(int i=0;i<s->n;i++) if(s->items[i].prod==it.prod&&s->items[i].dot==it.dot)return 0;
    s->items[s->n++]=it;return 1;
}
static void closure(State*s){
    int ch=1;
    while(ch){ch=0;
        for(int i=0;i<s->n;i++){
            int p=s->items[i].prod,dot=s->items[i].dot;
            if(dot>=prods[p].rhs_len)continue;
            int sym=prods[p].rhs[dot];
            if(!IS_NT(sym))continue;
            for(int q=0;q<NUM_PRODS;q++)
                if(prods[q].lhs==sym){Item it={q,0};if(add_item(s,it))ch=1;}
        }
    }
}
static int find_state(State*s){
    for(int si=0;si<n_states;si++){
        if(states[si].n!=s->n)continue;
        int ok=1;
        for(int i=0;i<s->n&&ok;i++){
            int f=0;
            for(int j=0;j<states[si].n&&!f;j++)
                if(s->items[i].prod==states[si].items[j].prod&&s->items[i].dot==states[si].items[j].dot)f=1;
            if(!f)ok=0;
        }
        if(ok)return si;
    }
    return -1;
}

void build_lr0_items(void){
    n_states = 0;
    memset(goto_raw,-1,sizeof(goto_raw));
    State s0;memset(&s0,0,sizeof(s0));
    Item st={0,0};add_item(&s0,st);closure(&s0);
    states[n_states++]=s0;
    int wl[MAX_STATES],wh=0,wt=0;
    wl[wt++]=0;
    while(wh<wt){
        int si=wl[wh++];
        for(int sym=0;sym<NUM_TERMS+NUM_NT;sym++){
            State ns;memset(&ns,0,sizeof(ns));
            for(int i=0;i<states[si].n;i++){
                int p=states[si].items[i].prod,dot=states[si].items[i].dot;
                if(dot<prods[p].rhs_len&&prods[p].rhs[dot]==sym){Item it={p,dot+1};add_item(&ns,it);}
            }
            if(!ns.n)continue;
            closure(&ns);
            int idx=find_state(&ns);
            if(idx==-1){idx=n_states;states[n_states++]=ns;wl[wt++]=idx;}
            goto_raw[si][sym]=idx;
        }
    }
}

void print_lr0_items(void){
    printf("\n====================== LR(0) CANONICAL COLLECTION ===========================\n");
    for(int si=0;si<n_states;si++){
        printf("\nI%d:\n",si);
        for(int i=0;i<states[si].n;i++){
            int p=states[si].items[i].prod,dot=states[si].items[i].dot;
            printf("  [ %-14s ->",nt_names[GET_NT(prods[p].lhs)]);
            for(int j=0;j<prods[p].rhs_len;j++){
                if(j==dot)printf(" .");
                int sym=prods[p].rhs[j];
                printf(" %s",IS_NT(sym)?nt_names[GET_NT(sym)]:token_name((TokenType)sym));
            }
            if(dot==prods[p].rhs_len)printf(" .");
            printf(" ]\n");
        }
    }
}

/* ---- SLR Table ---- */
#define ACCEPT_CODE 9999
static int action_tbl[MAX_STATES][NUM_TERMS];
static int goto_tbl  [MAX_STATES][NUM_NT];

static void set_action(int s,int t,int v){
    if(action_tbl[s][t]!=0&&action_tbl[s][t]!=v)
        fprintf(stderr,"SLR CONFLICT state %d terminal %s: %d vs %d\n",s,token_name(t),action_tbl[s][t],v);
    action_tbl[s][t]=v;
}

void build_slr_table(void){
    memset(action_tbl,0,sizeof(action_tbl));
    memset(goto_tbl,0,sizeof(goto_tbl));
    for(int si=0;si<n_states;si++){
        for(int i=0;i<states[si].n;i++){
            int p=states[si].items[i].prod,dot=states[si].items[i].dot;
            if(dot<prods[p].rhs_len){
                int sym=prods[p].rhs[dot],nxt=goto_raw[si][sym];
                if(nxt==-1)continue;
                if(!IS_NT(sym)) set_action(si,sym,nxt+1);
                else            goto_tbl[si][GET_NT(sym)]=nxt+1;
            } else {
                if(p==0) set_action(si,TOK_DOLLAR,ACCEPT_CODE);
                else{
                    int lnt=GET_NT(prods[p].lhs);
                    for(int t=0;t<NUM_TERMS;t++) if(FOLLOW[lnt][t]) set_action(si,t,-(p+1));
                }
            }
        }
    }
}

void print_slr_table(void){
    int shown[]={TOK_INT,TOK_MAIN,TOK_BEGIN,TOK_END,TOK_WHILE,TOK_RETURN,
                 TOK_ID,TOK_NUM,TOK_ASSIGN,TOK_GT,TOK_PLUS,TOK_DIV,TOK_SEMI,
                 TOK_LPAREN,TOK_RPAREN,TOK_DOLLAR};
    int ns=16;
    printf("\n========================= SLR PARSING TABLE ==================================\n");
    printf("\n--- ACTION TABLE ---\n");
    printf("%-6s","State");
    for(int j=0;j<ns;j++) printf(" %-7s",token_name(shown[j]));
    printf("\n");
    for(int j=0;j<6+ns*8;j++)printf("-");printf("\n");
    for(int si=0;si<n_states;si++){
        int any=0;
        for(int j=0;j<ns;j++) if(action_tbl[si][shown[j]])any=1;
        for(int n=0;n<NUM_NT;n++) if(goto_tbl[si][n])any=1;
        if(!any)continue;
        printf("%-6d",si);
        for(int j=0;j<ns;j++){
            int a=action_tbl[si][shown[j]];
            char buf[16]="";
            if(a==0)             strcpy(buf,"     ");
            else if(a==ACCEPT_CODE)strcpy(buf,"acc  ");
            else if(a>0)         sprintf(buf,"s%-4d",a-1);
            else                 sprintf(buf,"r%-4d",(-a)-1);
            printf(" %-7s",buf);
        }
        printf("\n");
    }
    printf("\n--- GOTO TABLE ---\n");
    printf("%-6s","State");
    for(int n=1;n<NUM_NT;n++) printf(" %-14s",nt_names[n]);
    printf("\n");
    for(int si=0;si<n_states;si++){
        int any=0;
        for(int n=1;n<NUM_NT;n++) if(goto_tbl[si][n])any=1;
        if(!any)continue;
        printf("%-6d",si);
        for(int n=1;n<NUM_NT;n++){
            char buf[16]="";
            if(goto_tbl[si][n])sprintf(buf,"%d",goto_tbl[si][n]-1);
            printf(" %-14s",buf);
        }
        printf("\n");
    }
}

/* ---- Parse Driver ---- */
#define STACK_SZ 1024

int slr_parse(void){
    int ss[STACK_SZ],top=0;
    ss[0]=0;
    token_pos=0;
    printf("\n========================= PARSE TRACE ========================================\n");
    printf("%-42s %-22s %s\n","STACK (states)","INPUT TOKEN","ACTION");
    printf("------------------------------------------------------------------------------\n");
    for(;;){
        int cur=ss[top];
        Token tok=peek_token();
        int t=(int)tok.type;
        char stk[512]="";
        for(int i=0;i<=top&&strlen(stk)<480;i++){char tmp[16];sprintf(tmp,"%d ",ss[i]);strcat(stk,tmp);}
        int a=action_tbl[cur][t];
        char act[128];
        if(a==0)            sprintf(act,"ERROR");
        else if(a==ACCEPT_CODE) sprintf(act,"ACCEPT");
        else if(a>0)        sprintf(act,"shift  s%d",a-1);
        else                sprintf(act,"reduce r%d  [%s]",(-a)-1,prods[(-a)-1].text);
        printf("%-42s %-22s %s\n",stk,tok.lexeme,act);
        if(a==0){printf("\nSyntax Error at token '%s' (state %d)\n",tok.lexeme,cur);return 0;}
        if(a==ACCEPT_CODE){printf("\nProgram Parsed Successfully\n");return 1;}
        if(a>0){if(top+1>=STACK_SZ){printf("Stack overflow\n");return 0;}ss[++top]=a-1;next_token();}
        else{int pn=(-a)-1,rlen=prods[pn].rhs_len;top-=rlen;int lnt=GET_NT(prods[pn].lhs);
             int g=goto_tbl[ss[top]][lnt];if(g==0){printf("GOTO ERROR r%d\n",pn);return 0;}ss[++top]=g-1;}
    }
}

void slr_init(void){
    init_productions();
    compute_first_follow();
    build_lr0_items();
    build_slr_table();
}
