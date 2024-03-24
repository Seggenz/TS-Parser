#include <iostream>
#include "tsTransportStream.h"

//=============================================================================================================================================================================
// xTS_PacketHeader
//=============================================================================================================================================================================


/// @brief Reset - reset all TS packet header fields
void xTS_PacketHeader::Reset() {
    m_SB = 0;
    m_E = 0;
    m_S = 0;
    m_T = 0;
    m_PID = 0;
    m_TSC = 0;
    m_AFC = 0;
    m_CC = 0;
}

/**
  @brief Parse all TS packet header fields
  @param Input is pointer to buffer containing TS packet
  @return Number of parsed bytes (4 on success, -1 on failure) 
 */
int32_t xTS_PacketHeader::Parse(const uint8_t *Input) {
    uint32_t *HeaderPtr = (uint32_t *) Input;
    uint32_t HeaderData = xSwapBytes32(*HeaderPtr);

    uint32_t E_mask = 0b00000000100000000000000000000000;
    uint32_t S_mask = 0b00000000010000000000000000000000;
    uint32_t T_mask = 0b00000000000100000000000000000000;
    uint32_t PID_mask = 0b00000000000111111111111100000000;
    uint32_t TSC_mask = 0b00000000000000000000000011000000;
    uint32_t AFC_mask = 0b00000000000000000000000000110000;
    uint32_t CC_mask = 0b00000000000000000000000000001111;

    m_SB = HeaderData >> 24; //11111111000000000000000000000000 >> 24 = 00000000000000000000000011111111
    // przesuwamy interesujace nas bity na koniec
    // a że jest to uint8_t to wartością zmiennej będą bity z prawej strony wiec mamy 11111111
    m_E = (HeaderData bitand E_mask) >> 23;
    m_S = (HeaderData bitand S_mask) >> 22;
    m_T = (HeaderData bitand T_mask) >> 21;
    m_PID = (HeaderData bitand PID_mask) >> 8;
    m_TSC = (HeaderData bitand TSC_mask) >> 6;
    m_AFC = (HeaderData bitand AFC_mask) >> 4;
    m_CC = HeaderData bitand CC_mask;

    return 4;
}

/// @brief Print all TS packet header fields
void xTS_PacketHeader::Print() const {
    printf("TS:");
    printf(" SB=%d", m_SB);
    printf(" E=%d", m_E);
    printf(" S=%d", m_S);
    printf(" T=%d", m_T);
    printf(" PID=%d", m_PID);
    printf(" TSC=%d", m_TSC);
    printf(" AFC=%d", m_AFC);
    printf(" CC=%d", m_CC);
}

//=============================================================================================================================================================================
