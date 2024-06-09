#ifndef TS_PARSER_TSADAPTATIONFIELD_H
#define TS_PARSER_TSADAPTATIONFIELD_H
#endif //TS_PARSER_TSADAPTATIONFIELD_H
#include "tsCommon.h"

class xTS_AdaptationField
{
protected:
//setup
    uint8_t m_AdaptationFieldControl;
//mandatory fields
    uint8_t m_AFL;
    uint8_t m_DC;
    uint8_t m_RA;
    uint8_t m_SP;
    uint8_t m_PR;
    uint8_t m_OR;
    uint8_t m_SF;
    uint8_t m_TP;
    uint8_t m_EX;
//optional fields - PCR
public:
    void Reset();
    int32_t Parse(const uint8_t* PacketBuffer, uint8_t AdaptationFieldControl);
    void Print() const;
public:
//mandatory fields
    uint8_t getAdaptationFieldLength () const { return
                m_AFL ; }
//derived values
//uint32_t getNumBytes () const { }
};