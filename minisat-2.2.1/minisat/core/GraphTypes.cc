#include "GraphTypes.h"

namespace Minisat
{

edge_kindt str_to_kind(const std::string& str)
{
    if(str == "epo")
        return OCLT_EPO;
    if(str == "po")
        return OCLT_PO;
    if(str == "rfi")
        return OCLT_RFI;
    if(str == "rfe")
        return OCLT_RFE;
    if(str == "coi")
        return OCLT_COI;
    if(str == "coe")
        return OCLT_COE;
    if(str == "fr")
        return OCLT_FR;

    return OCLT_NA;
}

std::string kind_to_str(edge_kindt kind)
{
    switch(kind)
    {
        case OCLT_NA:
            return "na";
        case OCLT_EPO:
            return "epo";
        case OCLT_PO:
            return "po";
        case OCLT_RFI:
            return "rfi";
        case OCLT_RFE:
            return "rfe";
        case OCLT_COI:
            return "coi";
        case OCLT_COE:
            return "coe";
        case OCLT_FR:
            return "fr";
    }
    return "unknown";
}

}