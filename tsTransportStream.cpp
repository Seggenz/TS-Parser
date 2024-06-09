#include "tsTransportStream.h"
#include <iostream>
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

    uint32_t E_mask   = 0b00000000100000000000000000000000;
    uint32_t S_mask   = 0b00000000010000000000000000000000;
    uint32_t T_mask   = 0b00000000001000000000000000000000;
    uint32_t PID_mask = 0b00000000000111111111111100000000;
    uint32_t TSC_mask = 0b00000000000000000000000011000000;
    uint32_t AFC_mask = 0b00000000000000000000000000110000;
    uint32_t CC_mask  = 0b00000000000000000000000000001111;

    m_SB = HeaderData >> 24;
    m_E = (HeaderData & E_mask) >> 23;
    m_S = (HeaderData & S_mask) >> 22;
    m_T = (HeaderData & T_mask) >> 21;
    m_PID = (HeaderData & PID_mask) >> 8;
    m_TSC = (HeaderData & TSC_mask) >> 6;
    m_AFC = (HeaderData & AFC_mask) >> 4;
    m_CC = HeaderData & CC_mask;

    return NOT_VALID;
}

/// @brief Print all TS packet header fields
void xTS_PacketHeader::Print() const {
    printf("TS: SB=%d E=%d S=%d P=%d PID=%4d TSC=%d AF=%d CC=%2d ", m_SB, m_E, m_S, m_T, m_PID, m_TSC, m_AFC, m_CC);

}

//=============================================================================================================================================================================
