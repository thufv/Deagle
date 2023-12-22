#ifndef CAT_TYPE_H
#define CAT_TYPE_H

#include <string>
#include <vector>

enum rel_opt
{
    // operators
    TERMINATE, //NO OPERATOR, 0-ary
    ALT, // |, unary + unary -> unary, also binary + binary -> binary
    SEQ, // ;, binary + binary -> binary
    AND, // &, unary + unary -> unary, also binary + binary -> binary
    MINUS, // \\, unary + unary -> unary, also binary + binary -> binary
    TIMES, // *, unary + unary -> binary
    BRACKET, // [], unary -> binary
    PLUS, // +, binary -> binary
    STAR, // *, binary -> binary
    QMARK, // ?, binary -> binary
    FLIP, // ^-1, binary -> binary
    // functions
    ROT, // rotation closure (unused?), binary -> binary
    INT, // in the same thread, binary -> binary
    EXT, // in different threads, binary -> binary
    // special for guard conditions
    GUARD_ENABLEMENT, // unary(guard) + binary + unary(guard) -> binary
};

enum rel_axiomt
{
    NO_AXIOM,
    EMPTY,
    IRREFLEXIVE,
    ACYCLIC,
};

/*
    cat_relationt: a tree structure for relations
*/

class cat_relationt
{
public:
    // for "let fr = rf^-1;co"
    // known name is "fr"
    // real name is "rf^-1;co" (no any space)
    // known names can be empty for intermediate relations
    std::string known_name;
    std::string real_name;
    rel_opt op;
    std::vector<cat_relationt> operands;

    std::string calc_real_name()
    {
        switch(op)
        {
        case TERMINATE:
            return "";
        case ALT:
            return "(" + operands[0].real_name + "|" + operands[1].real_name + ")";
        case SEQ:
            return "(" + operands[0].real_name + ";" + operands[1].real_name + ")";
        case AND:
            return "(" + operands[0].real_name + "&" + operands[1].real_name + ")";
        case MINUS:
            return "(" + operands[0].real_name + "\\" + operands[1].real_name + ")";
        case TIMES:
            return "(" + operands[0].real_name + "*" + operands[1].real_name + ")";
        case BRACKET:
            return "[" + operands[0].real_name + "]"; 
        case PLUS:
            return "(" + operands[0].real_name + ")+";
        case STAR:
            return "(" + operands[0].real_name + ")*";
        case QMARK:
            return "(" + operands[0].real_name + ")?"; 
        case FLIP:
            return "(" + operands[0].real_name + ")^-1"; 
        case ROT:
            return "rot(" + operands[0].real_name + ")"; 
        case INT:
            return "int(" + operands[0].real_name + ")"; 
        case EXT:
            return "ext(" + operands[0].real_name + ")";
        case GUARD_ENABLEMENT:
            return "unknown";
        }
        return "unknown";
    }

    cat_relationt(std::string _name = "") : 
        known_name (_name), real_name (_name), op(TERMINATE) {} // terminate
    cat_relationt(rel_opt _op, cat_relationt op1) : 
        known_name(""), op(_op), operands(std::vector<cat_relationt>{op1}) 
        {real_name = calc_real_name();} //unary
    cat_relationt(rel_opt _op, cat_relationt op1, cat_relationt op2) : 
        known_name(""), op(_op), operands(std::vector<cat_relationt>{op1, op2}) 
        {real_name = calc_real_name();} //binary
};

class cat_axiomt
{
public:
    rel_axiomt axiom_type;
    cat_relationt relation;

    cat_axiomt(){}
    cat_axiomt(rel_axiomt _type, cat_relationt _relation): axiom_type(_type), relation(_relation) {}
};

enum link_post
{
    LEFT,
    RIGHT,
};

class cat_linkt
{
public:
    rel_opt link_type;
    link_post link_position;
    std::string another_kind;
    cat_linkt(rel_opt _type) : 
        link_type(_type), another_kind("invalid") {}
    cat_linkt(rel_opt _type, link_post _pos) : 
        link_type(_type), link_position(_pos), another_kind("invalid") {}
    cat_linkt(rel_opt _type, std::string _rel) :
        link_type(_type), another_kind(_rel) {}
    cat_linkt(rel_opt _type, link_post _pos, std::string _rel) :
        link_type(_type), link_position(_pos), another_kind(_rel) {}
};

class cat_edget // like (co, link, fr)
{
public:
    std::string from;
    cat_linkt link;
    std::string to;
    cat_edget(std::string _from, cat_linkt _link, std::string _to) :
        from(_from), link(_link), to(_to) {}
    cat_edget(cat_relationt _from, cat_linkt _link, cat_relationt _to) :
        from(_from.real_name), link(_link), to(_to.real_name) {}
};

#endif // CAT_TYPE_H