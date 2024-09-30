#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define POOLSIZE 1024 * 128

typedef struct TreeNode {
    struct TreeNode *next[12];
    int nextCount;
    char Name[20];
    int depth;
} treeNode;

treeNode *head;

treeNode *createNode(treeNode *cur, char str[]) {
    treeNode *Now = (treeNode *) malloc(sizeof(treeNode));
    Now->nextCount = 0;
    strcpy(Now->Name, str);
    Now->depth = cur->depth + 1;
    cur->next[cur->nextCount++] = Now;
    return Now;
}

void printTree(treeNode *cur) {
    int num = cur->nextCount;
    for (int i = 0; i < cur->depth; i++) {
        printf("  ");
    }
    printf("%s\n", cur->Name);
    for (int i = 0; i < num; i++) {
        printTree(cur->next[i]);
    }
}

void clearTree(treeNode *cur) {
    int num = cur->nextCount;
    for (int i = 0; i < num; i++) {
        clearTree(cur->next[i]);
        cur->nextCount--;
    }
    if (!cur->nextCount)
        free(cur);
}

int lineCount = 1;
char *src, *Asrc;
int token;
int hash;
int token_val; // value of current token
int *current_id; // current parsed ID
void compound_stmt(treeNode *now);

void type_specifier(treeNode *now);

void expression(treeNode *now);

void var(treeNode *now);

void statement(treeNode *now);

enum {
    Global,
    Local
};

enum {
    Var,
    Func
};

enum {
    Token,
    Hash,
    Name,
    Type,
    Class,
    Value
};

typedef enum {
    Num = 128,
    Fun,
    Id,
    Else,
    If,
    Int,
    Return,
    Continue,
    While,
    Break,
    Void,
    Assign,
    Eq,
    Ne,
    Lt,
    Gt,
    Le,
    Ge,
    Add,
    Sub,
    Mul,
    Div,
    Null
} TokenType;

int HashFunc(char *str) {
    int hash = 5381;
    while (*str) {
        hash += (hash << 5) + (*str++);
    }
    return (hash & 0x7FFFFFFF) % POOLSIZE;
}

char *TokenTypeStr[] = {
    "Num", "Fun", "Id", "Else", "If", "Int", "Return", "Continue", "While", "Break", "Void",
    "Assign", "Eq", "Ne", "Lt", "Gt", "Le", "Ge", "Add", "Sub", "Mul", "Div"
};

typedef struct {
    int token;
    int hash;
    char name[64];
    int class;
    int type;
    int value;
    int flag;
} identifier;

identifier SymbolTable[POOLSIZE];

void error(char *msg) {
    printf("%s\n", msg);
    exit(1);
}

void assignTable(int HashValue, char *str, int num) {
    SymbolTable[HashValue].hash = HashValue;
    strcpy(SymbolTable[HashValue].name, str);
    SymbolTable[HashValue].flag = 1;
    SymbolTable[HashValue].token = num;
}

void nextToken() {
    char position[64], *last_position;

    while (token = *src) {
        ++src;
        // parse token here
        if (token == '\n') {
            lineCount++;
        } else if (token == '/') {
            if (*src == '*') {
                // skip comments
                while (!(*(src - 1) == '*' && *src == '/')) {
                    src++;
                }
                src++;
            } else {
                // divide operator
                token = Div;
                return;
            }
        } else if (token == '=') {
            // parse '==' and '='
            if (*src == '=') {
                src++;
                token = Eq;
            } else {
                token = Assign;
            }
            return;
        } else if (token == '+') {
            // parse '+'
            token = Add;
            return;
        } else if (token == '-') {
            // parse '-'
            token = Sub;
            return;
        } else if (token == '!') {
            // parse '!='
            if (*src == '=') {
                src++;
                token = Ne;
            }
            return;
        } else if (token == '<') {
            if (*src == '=') {
                src++;
                token = Le;
            } else {
                token = Lt;
            }
            return;
        } else if (token == '>') {
            // parse '>=', '>>' or '>'
            if (*src == '=') {
                src++;
                token = Ge;
            } else {
                token = Gt;
            }
            return;
        } else if (token == '*') {
            token = Mul;
            return;
        } else if (token == ';' || token == '{' || token == '}' || token == '(' || token == ')' || token == ',' || token
                   == ':') {
            // directly return the character as token;
            return;
        } else if (token >= '0' && token <= '9') {
            // parse number, three kinds: dec(123) hex(0x123) oct(017)
            token_val = token - '0';
            if (token_val > 0) {
                // dec, starts with [1-9]
                while (*src >= '0' && *src <= '9') {
                    token_val = token_val * 10 + *src++ - '0';
                }
            } else {
                // starts with number 0
                if (*src == 'x' || *src == 'X') {
                    // hex
                    token = *++src;
                    while ((token >= '0' && token <= '9') || (token >= 'a' && token <= 'f') || (
                               token >= 'A' && token <= 'F')) {
                        token_val = token_val * 16 + (token & 15) + (token >= 'A' ? 9 : 0);
                        token = *++src;
                    }
                } else {
                    // oct
                    while (*src >= '0' && *src <= '7') {
                        token_val = token_val * 8 + *src++ - '0';
                    }
                }
            }
            // printf("get TokenValue: %d ", token_val);
            token = Num;
            return;
        } else if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z') || (token == '_')) {
            // parse identifier
            int IDsize = 1;
            last_position = src - 1;

            while ((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || (
                       *src == '_')) {
                IDsize++;
                src++;
            }
            // print ID value
            // printf("get Tokenvalue: ");
            for (int i = 0; i < IDsize; i++)
                position[i] = last_position[i];
            position[IDsize] = '\0';
            // printf("%s ", position);

            hash = HashFunc(position);

            if (!SymbolTable[hash].flag) {
                SymbolTable[hash].hash = hash;
                strcpy(SymbolTable[hash].name, position);
                SymbolTable[hash].flag = 1;
                SymbolTable[hash].token = Id;
                token = Id;
            } else {
                token = SymbolTable[hash].token;
            }

            return;
        }
    }
    return;
}

void lex_scanner() {
    nextToken();
    while (token > 0) {
        if (token >= 128)
            printf("token is: %s (in line %d)\n", TokenTypeStr[token - 128], lineCount);
        else
            printf("token is: %c (in line %d)\n", token, lineCount);
        nextToken();
    }
}

int match(int expected, treeNode *now) {
    if (token == expected) {
        if (token >= 128) {
            if (token == Id) {
                char string[128];
                strcpy(string, "Id: ");
                strcat(string, SymbolTable[hash].name);
                createNode(now, string);
            } else
                createNode(now, TokenTypeStr[token - 128]);
        } else {
            char string[2];
            string[0] = token;
            string[1] = '\0';
            createNode(now, string);
        }
        nextToken();
        return 1;
    } else {
        if (token >= 128) {
            printf("%d: Token UnExpected is %s\n", lineCount, TokenTypeStr[token - 128]);
            exit(1);
        } else {
            printf("%d: Token UnExpected is %c\n", lineCount, token);
            exit(1);
        }
    }
}

void arg_list_(treeNode *now) {
    char string[] = {"arg_list_:"};
    now = createNode(now, string);
    if (token == ',') {
        match(',', now);
        expression(now);
        arg_list_(now);
    } else {
        return;
    }
}

void arg_list(treeNode *now) {
    char string[] = {"arg_list:"};
    now = createNode(now, string);
    expression(now);
    arg_list_(now);
}

void args(treeNode *now) {
    char string[] = {"args:"};
    now = createNode(now, string);
    if (token == ')')
        return;
    else {
        arg_list(now);
        return;
    }
}

void call(treeNode *now) {
    char string[] = {"call:"};
    now = createNode(now, string);
    match('(', now);
    args(now);
    match(')', now);
}

void factor(treeNode *now) {
    char string[] = {"factor:"};
    now = createNode(now, string);
    switch (token) {
        case Num:
            match(Num, now);
            break;
        case '(':
            match('(', now);
            expression(now);
            match(')', now);
            break;
        default:
            match(Id, now);
            if (token == '(') {
                call(now);
            } else {
                break;
            }
    }
}

void term_(treeNode *now) {
    char string[] = {"term_:"};
    now = createNode(now, string);
    if (token == Mul) {
        match(Mul, now);
        term_(now);
        factor(now);
    } else if (token == Div) {
        match(Div, now);
        term_(now);
        factor(now);
    } else {
        return;
    }
}

void term(treeNode *now) {
    char string[] = {"term:"};
    now = createNode(now, string);
    factor(now);
    term_(now);
}

void additive_expression_(treeNode *now) {
    char string[] = {"additive_expr_:"};
    now = createNode(now, string);
    if (token == Add) {
        match(Add, now);
        additive_expression_(now);
        term(now);
    } else if (token == Sub) {
        match(Sub, now);
        additive_expression_(now);
        term(now);
    } else {
        return;
    }
}

void additive_expression(treeNode *now) {
    char string[] = {"additive_expr:"};
    now = createNode(now, string);
    term(now);
    additive_expression_(now);
}

void var(treeNode *now) {
    char string[] = {"var:"};
    now = createNode(now, string);
    match(Id, now);
    return;
}

void relop(treeNode *now) {
    char string[] = {"relop:"};
    now = createNode(now, string);
    match(token, now);
}

void simple_expression(treeNode *now) {
    char string[] = {"simple_expression:"};
    now = createNode(now, string);
    additive_expression(now);
    if (token == Lt || token == Gt || token == Le || token == Ge || token == Ne || token == Eq) {
        relop(now);
        additive_expression(now);
    }
    return;
}

void expression(treeNode *now) {
    char string[] = {"expression:"};
    now = createNode(now, string);
    int flag = 0;
    for (int i = 0;; i++) {
        if ((*(src + i)) != ' ' && (*(src + i)) != '\n' && (*(src + i)) != '\t') {
            if (*(src + i) == '=' && *(src + i + 1) != '=') {
                flag = 1;
                break;
            } else {
                break;
            }
        }
    }
    if (flag) {
        flag = 0;
        var(now);
        match(Assign, now);
        expression(now);
    } else {
        simple_expression(now);
    }
}

void expression_stmt(treeNode *now) {
    char string[] = {"expression_stmt:"};
    now = createNode(now, string);
    if (token == ';') {
        match(';', now);
        return;
    } else {
        expression(now);
        match(';', now);
        return;
    }
}

void selection_stmt(treeNode *now) {
    char string[] = {"selection_stmt:"};
    now = createNode(now, string);
    match(If, now);
    match('(', now);
    expression(now);
    match(')', now);
    statement(now);
    if (token == Else) {
        match(Else, now);
        statement(now);
    }
    return;
}

void return_stmt(treeNode *now) {
    char string[] = {"return_stmt:"};
    now = createNode(now, string);
    match(Return, now);
    if (token == ';') {
        match(';', now);
        return;
    } else {
        expression(now);
        match(';', now);
        return;
    }
}

void iteration_stmt(treeNode *now) {
    char string[] = {"iteration_stmt:"};
    now = createNode(now, string);
    match(While, now);
    match('(', now);
    expression(now);
    match(')', now);
    statement(now);
}

void statement(treeNode *now) {
    char string[] = {"statement:"};
    now = createNode(now, string);
    switch (token) {
        case '{':
            compound_stmt(now);
            break;
        case If:
            selection_stmt(now);
            break;
        case Return:
            return_stmt(now);
            break;
        case While:
            iteration_stmt(now);
            break;
        default:
            expression_stmt(now);
            break;
    }
}

void statement_list(treeNode *now) {
    char string[] = {"statement_list:"};
    now = createNode(now, string);
    if (token == '}') {
        return;
    }
    statement(now);
    statement_list(now);
}

void local_declarations(treeNode *now) {
    char string[] = {"local_declaration:"};
    now = createNode(now, string);
    if (token != Int)
        return;
    else {
        match(Int, now);
        match(Id, now);
        SymbolTable[hash].class = Local;
        SymbolTable[hash].type = Var;
        match(';', now);
        local_declarations(now);
    }
}

void compound_stmt(treeNode *now) {
    char string[] = {"compound_stmt:"};
    now = createNode(now, string);
    match('{', now);
    local_declarations(now);
    statement_list(now);
    match('}', now);
    return;
}

void param(treeNode *now) {
    char string[] = {"param:"};
    now = createNode(now, string);
    if (token == Int) {
        type_specifier(now);
        match(Id, now);
        SymbolTable[hash].class = Local;
        SymbolTable[hash].type = Var;
        return;
    }
}

void params_list_(treeNode *now) {
    char string[] = {"params_list_:"};
    now = createNode(now, string);
    if (token == ')') {
        return;
    }
    match(',', now);
    param(now);
    params_list_(now);
}

void params_list(treeNode *now) {
    char string[] = {"params_list:"};
    now = createNode(now, string);
    param(now);
    params_list_(now);
}

void params(treeNode *now) {
    char string[] = {"params:"};
    now = createNode(now, string);
    if (token == Void)
        match(Void, now);
    else if (token == ')')
        return;
    else
        params_list(now);
}

void type_specifier(treeNode *now) {
    char string[] = {"type_specifier:"};
    createNode(now, string);
    if (token == Int || token == Void)
        match(token, now);
    else
        error("Wrong Type-Specifier!");
}

void var_declaration(treeNode *now) {
    char string[] = {"var_declaration:"};
    now = createNode(now, string);
    type_specifier(now);
    match(Id, now);
    SymbolTable[hash].class = Global;
    SymbolTable[hash].type = Var;
    match(';', now);
    return;
}

void fun_declaration(treeNode *now) {
    char string[] = {"fun_declaration:"};
    now = createNode(now, string);
    type_specifier(now);
    match(Id, now);
    SymbolTable[hash].class = Global;
    SymbolTable[hash].type = Func;
    match('(', now);
    params(now);
    match(')', now);
    if (token == ';')
        match(';', now);
    else {
        compound_stmt(now);
    }
    return;
}

int HashBackup;

void declaration(treeNode *now) {
    char string[] = {"declaration:"};
    now = createNode(now, string);
    nextToken();
    nextToken();
    if (token == ';') {
        src = Asrc;
        token = SymbolTable[HashBackup].token;
        var_declaration(now);
    } else {
        src = Asrc;
        token = SymbolTable[HashBackup].token;
        fun_declaration(now);
    }
}

void program() {
    treeNode *now;
    char string[] = {"program:"};
    nextToken();
    Asrc = src;
    HashBackup = hash;
    while (token > 0) {
        HashBackup = hash;
        Asrc = src;
        now = head;
        declaration(now);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2)
        error("Wrong Arguments!");
    FILE *file = fopen(argv[1], "r");
    if (!file)
        error("Fail To Open!");

    src = malloc(POOLSIZE);
    char *backup = src;
    if (!src)
        error("Cannot Malloc POOLSIZE Bits");

    int i = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        src[i++] = ch;
    }
    src[i] = 0;
    assignTable(HashFunc("int"), "int", Int);
    assignTable(HashFunc("void"), "void", Void);
    assignTable(HashFunc("break"), "break", Break);
    assignTable(HashFunc("continue"), "continue", Continue);
    assignTable(HashFunc("if"), "if", If);
    assignTable(HashFunc("else"), "else", Else);
    assignTable(HashFunc("return"), "return", Return);
    assignTable(HashFunc("while"), "while", While);

    head = (treeNode *) malloc(sizeof(treeNode));
    head->depth = 0;
    strcpy(head->Name, "program");
    head->nextCount = 0;
    program();
    printTree(head);

    fclose(file);
    free(backup);
    clearTree(head);
    return 0;
}
