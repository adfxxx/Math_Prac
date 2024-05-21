#ifndef TP_H
#define TP_H

typedef enum Syntax{
    LEFT,
    RIGHT,
    MIDDLE
}Syntax;

typedef enum Type{
    BINARY,
    UNARY
}Type;

static char *operations[] = {"not", "input", "output", "add", "mult", "sub", "pow", "div", "rem", "xor", "and", "or", "="};
static Syntax syntax[] = {LEFT, LEFT, LEFT, MIDDLE, MIDDLE, MIDDLE, MIDDLE, MIDDLE, MIDDLE, MIDDLE, MIDDLE, MIDDLE, MIDDLE};
static const Type types[] = {UNARY, UNARY, UNARY, BINARY, BINARY, BINARY, BINARY, BINARY, BINARY, BINARY, BINARY, BINARY, BINARY};

typedef enum Operations{
    NOT,
    INPUT,
    OUTPUT,
    ADD,
    MULT,
    SUB,
    POW,
    DIV,
    REM,
    XOR,
    AND,
    OR,
    EQUAL
}Operations;

#endif