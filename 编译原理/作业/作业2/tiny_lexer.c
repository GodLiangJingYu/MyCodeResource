/**
 * TINY语言词法分析器
 * 基于DFA实现
 * 
 * 编译原理作业2
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ==================== Token类型定义 ==================== */

typedef enum {
    /* 保留字/关键字 */
    IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE,
    /* 标识符和数字 */
    ID, NUM,
    /* 专用符号 */
    PLUS,       /* + */
    MINUS,      /* - */
    TIMES,      /* * */
    OVER,       /* / */
    LT,         /* < */
    EQ,         /* = */
    LPAREN,     /* ( */
    RPAREN,     /* ) */
    ASSIGN,     /* := */
    SEMI,       /* ; (分号，虽然图中未显示，但TINY通常需要) */
    /* 特殊标记 */
    ENDFILE,    /* 文件结束 */
    ERROR       /* 错误标记 */
} TokenType;

/* DFA状态定义 */
typedef enum {
    START,      /* 起始状态 */
    INNUM,      /* 读取数字中 */
    INID,       /* 读取标识符中 */
    INASSIGN,   /* 读取:=中，已读取: */
    INCOMMENT,  /* 读取注释中 */
    DONE        /* 完成状态 */
} StateType;

/* Token最大长度 */
#define MAXTOKENLEN 256

/* 当前Token的字符串值 */
char tokenString[MAXTOKENLEN + 1];

/* ==================== 全局变量 ==================== */

/* 源文件 */
FILE* source;
/* 输出文件 */
FILE* listing;

/* 行缓冲区 */
#define BUFLEN 256
char lineBuf[BUFLEN];
int linepos = 0;        /* 当前在行缓冲区中的位置 */
int bufsize = 0;        /* 行缓冲区中的字符数 */
int lineno = 0;         /* 当前行号 */
int EOF_flag = 0;       /* 是否到达文件末尾 */

/* 是否打印源代码 */
int EchoSource = 1;
/* 是否打印Token */
int TraceScan = 1;

/* ==================== 关键字表 ==================== */

/* 保留字结构 */
typedef struct {
    char* str;
    TokenType tok;
} ReservedWord;

/* 保留字表 */
static ReservedWord reservedWords[] = {
    {"if", IF},
    {"then", THEN},
    {"else", ELSE},
    {"end", END},
    {"repeat", REPEAT},
    {"until", UNTIL},
    {"read", READ},
    {"write", WRITE}
};

#define RESERVED_WORDS_COUNT (sizeof(reservedWords) / sizeof(ReservedWord))

/* 查找保留字，返回对应的TokenType，若不是保留字则返回ID */
static TokenType reservedLookup(char* s) {
    for (int i = 0; i < RESERVED_WORDS_COUNT; i++) {
        if (strcmp(s, reservedWords[i].str) == 0) {
            return reservedWords[i].tok;
        }
    }
    return ID;
}

/* ==================== 字符获取函数 ==================== */

/* 获取下一个字符 */
static int getNextChar(void) {
    if (linepos >= bufsize) {
        /* 需要读取新行 */
        lineno++;
        if (fgets(lineBuf, BUFLEN - 1, source)) {
            if (EchoSource) {
                fprintf(listing, "%4d: %s", lineno, lineBuf);
            }
            bufsize = strlen(lineBuf);
            linepos = 0;
            return lineBuf[linepos++];
        } else {
            EOF_flag = 1;
            return EOF;
        }
    } else {
        return lineBuf[linepos++];
    }
}

/* 回退一个字符 */
static void ungetNextChar(void) {
    if (!EOF_flag) {
        linepos--;
    }
}

/* ==================== Token类型转字符串 ==================== */

const char* getTokenName(TokenType token) {
    switch (token) {
        case IF: return "IF";
        case THEN: return "THEN";
        case ELSE: return "ELSE";
        case END: return "END";
        case REPEAT: return "REPEAT";
        case UNTIL: return "UNTIL";
        case READ: return "READ";
        case WRITE: return "WRITE";
        case ID: return "ID";
        case NUM: return "NUM";
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case TIMES: return "TIMES";
        case OVER: return "OVER";
        case LT: return "LT";
        case EQ: return "EQ";
        case LPAREN: return "LPAREN";
        case RPAREN: return "RPAREN";
        case ASSIGN: return "ASSIGN";
        case SEMI: return "SEMI";
        case ENDFILE: return "ENDFILE";
        case ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

/* ==================== 核心：getToken函数 ==================== */

/**
 * 词法分析主函数
 * 基于DFA实现，返回下一个Token
 */
TokenType getToken(void) {
    /* tokenString的索引 */
    int tokenStringIndex = 0;
    /* 当前Token类型 */
    TokenType currentToken;
    /* 当前DFA状态 */
    StateType state = START;
    /* 是否保存当前字符到tokenString */
    int save;

    while (state != DONE) {
        int c = getNextChar();
        save = 1;  /* 默认保存字符 */

        switch (state) {
            case START:
                if (isdigit(c)) {
                    /* 数字开头 -> INNUM状态 */
                    state = INNUM;
                } else if (isalpha(c)) {
                    /* 字母开头 -> INID状态 */
                    state = INID;
                } else if (c == ':') {
                    /* 可能是:= */
                    state = INASSIGN;
                } else if (c == ' ' || c == '\t' || c == '\n') {
                    /* 空白符，跳过，不保存 */
                    save = 0;
                } else if (c == '{') {
                    /* 注释开始，不保存 */
                    save = 0;
                    state = INCOMMENT;
                } else {
                    /* 单字符Token或错误 */
                    state = DONE;
                    switch (c) {
                        case EOF:
                            save = 0;
                            currentToken = ENDFILE;
                            break;
                        case '+':
                            currentToken = PLUS;
                            break;
                        case '-':
                            currentToken = MINUS;
                            break;
                        case '*':
                            currentToken = TIMES;
                            break;
                        case '/':
                            currentToken = OVER;
                            break;
                        case '<':
                            currentToken = LT;
                            break;
                        case '=':
                            currentToken = EQ;
                            break;
                        case '(':
                            currentToken = LPAREN;
                            break;
                        case ')':
                            currentToken = RPAREN;
                            break;
                        case ';':
                            currentToken = SEMI;
                            break;
                        default:
                            currentToken = ERROR;
                            break;
                    }
                }
                break;  /* END case START */

            case INNUM:
                if (!isdigit(c)) {
                    /* 不是数字，回退并完成 */
                    ungetNextChar();
                    save = 0;
                    state = DONE;
                    currentToken = NUM;
                }
                /* 是数字则继续读取 */
                break;

            case INID:
                if (!isalpha(c)) {
                    /* 不是字母，回退并完成 */
                    /* 注意：TINY的ID只包含字母，不包含数字 */
                    ungetNextChar();
                    save = 0;
                    state = DONE;
                    currentToken = ID;
                }
                /* 是字母则继续读取 */
                break;

            case INASSIGN:
                state = DONE;
                if (c == '=') {
                    currentToken = ASSIGN;
                } else {
                    /* 只有':'不跟'='是错误 */
                    ungetNextChar();
                    save = 0;
                    currentToken = ERROR;
                }
                break;

            case INCOMMENT:
                save = 0;  /* 注释内容不保存 */
                if (c == EOF) {
                    state = DONE;
                    currentToken = ENDFILE;
                } else if (c == '}') {
                    /* 注释结束，回到START */
                    state = START;
                }
                /* 否则继续在注释中 */
                break;

            case DONE:
                /* 不应该到达这里 */
                break;

            default:
                fprintf(listing, "Scanner Bug: state= %d\n", state);
                state = DONE;
                currentToken = ERROR;
                break;
        }

        /* 保存字符到tokenString */
        if (save && tokenStringIndex < MAXTOKENLEN) {
            tokenString[tokenStringIndex++] = (char)c;
        }
    }

    /* 添加字符串结束符 */
    tokenString[tokenStringIndex] = '\0';

    /* 如果是标识符，检查是否为保留字 */
    if (currentToken == ID) {
        currentToken = reservedLookup(tokenString);
    }

    /* 打印Token信息 */
    if (TraceScan) {
        fprintf(listing, "\t%d: ", lineno);
        fprintf(listing, "%-10s", getTokenName(currentToken));
        if (currentToken == ID || currentToken == NUM || currentToken == ERROR) {
            fprintf(listing, ", val= %s", tokenString);
        }
        fprintf(listing, "\n");
    }

    return currentToken;
}

/* ==================== 主函数 ==================== */

int main(int argc, char* argv[]) {
    char sourceFile[256];

    if (argc < 2) {
        printf("TINY词法分析器\n");
        printf("用法: %s <源文件>\n", argv[0]);
        printf("\n请输入TINY源文件名: ");
        if (scanf("%255s", sourceFile) != 1) {
            printf("输入错误!\n");
            return 1;
        }
    } else {
        strncpy(sourceFile, argv[1], 255);
        sourceFile[255] = '\0';
    }

    source = fopen(sourceFile, "r");
    if (source == NULL) {
        fprintf(stderr, "错误: 无法打开文件 '%s'\n", sourceFile);
        return 1;
    }

    listing = stdout;  /* 输出到标准输出 */

    printf("\n========== TINY词法分析器 ==========\n");
    printf("源文件: %s\n", sourceFile);
    printf("====================================\n\n");

    /* 词法分析主循环 */
    TokenType token;
    do {
        token = getToken();
    } while (token != ENDFILE);

    printf("\n====================================\n");
    printf("词法分析完成!\n");

    fclose(source);
    return 0;
}
