#ifndef CAT_TYPE_H
#define CAT_TYPE_H

#include <string>
#include <vector>

enum rel_opt
{
    // operators
    TERMINAL,
    ALT, // |, unary + unary -> unary, also binary + binary -> binary
    SEQ, // ;, binary + binary -> binary
    AND, // &, unary + unary -> unary, also binary + binary -> binary
    SUB, // \\, unary + unary -> unary, also binary + binary -> binary
    PROD, // *, unary + unary -> binary
    BRACKET, // [], unary -> binary
    FLIP, // ^-1, binary -> binary

    // this will be converted later in function replace_plus
    PLUS, // d = r+ is converted to d = r | (d ; d)

    // the following will be converted upon discovered
    // STAR, // d = r* is converted to d = id | r+
    // QMARK, // d = r? is converted to d = id | r
    // FENCEREL, // d = fencerel(l) is converted to d = po ; [l] ; po
};

enum rel_axiomt
{
    NO_AXIOM,
    EMPTY,
    NOT_EMPTY,
    IRREFLEXIVE,
    ACYCLIC,
};

class cat_relationt
{
public:
    std::string name;
    rel_opt op_type;
    std::vector<std::string> operands;

    cat_relationt() : name (""), op_type(TERMINAL) {} // hopefully this won't be used

    cat_relationt(std::string _name, rel_opt _op_type, std::vector<std::string> _operands) :
        name (_name), op_type(_op_type), operands(_operands) {}
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
        link_type(_type), link_position(LEFT), another_kind("invalid") {}
    cat_linkt(rel_opt _type, link_post _pos) : 
        link_type(_type), link_position(_pos), another_kind("invalid") {}
    cat_linkt(rel_opt _type, std::string _rel) :
        link_type(_type), another_kind(_rel) {}
    cat_linkt(rel_opt _type, link_post _pos, std::string _rel) :
        link_type(_type), link_position(_pos), another_kind(_rel) {}
    bool operator==(const cat_linkt& other) const
    {
        return link_type == other.link_type && link_position == other.link_position && another_kind == other.another_kind;
    }
    bool operator!=(const cat_linkt& other) const { return !(*this == other); }
    bool operator<(const cat_linkt& other) const
    {
        if(link_type != other.link_type)
            return link_type < other.link_type;
        if(link_position != other.link_position)
            return link_position < other.link_position;
        return another_kind < other.another_kind;
    }
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
        from(_from.name), link(_link), to(_to.name) {}
    
    bool operator<(const cat_edget& other) const
    {
        if(from != other.from)
            return from < other.from;
        if(link != other.link)
            return link < other.link;
        return to < other.to;
    }
};

#endif // CAT_TYPE_H