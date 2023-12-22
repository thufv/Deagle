#include "GraphTypes.h"

namespace Minisat
{

edge_kindt str_to_kind(const std::string& str)
{
    if(str == "apo")
        return OC_APO;
    if(str == "po")
        return OC_PO;
    if(str == "rf")
        return OC_RF;
    if(str == "ws")
        return OC_WS;
    if(str == "fr")
        return OC_FR;
    if(str == "race")
        return OC_RACE;

    return OC_NA;
}

std::string kind_to_str(edge_kindt kind)
{
    switch(kind)
    {
        case OC_NA:
            return "na";
        case OC_APO:
            return "apo";
        case OC_PO:
            return "po";
        case OC_RF:
            return "rf";
        case OC_WS:
            return "ws";
        case OC_FR:
            return "fr";
        case OC_RACE:
            return "race";
    }
    return "unknown";
}

}