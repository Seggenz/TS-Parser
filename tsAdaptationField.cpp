#include "tsAdaptationField.h"
#include <iostream>

/// @brief Reset - reset all TS packet header fields
void xTS_AdaptationField::Reset() {
    m_AFL = 0;
    m_DC = 0;
    m_RA = 0;
    m_SP = 0;
    m_PR = 0;
    m_OR = 0;
    m_SF = 0;
    m_TP = 0;
    m_EX = 0;
}

/**
@brief Parse adaptation field
@param PacketBuffer is pointer to buffer containing TS packet
@param AdaptationFieldControl is value of Adaptation Field Control field of
corresponding TS packet header
@return Number of parsed bytes (length of AF or -1 on failure)
*/
int32_t xTS_AdaptationField::Parse(const uint8_t *PacketBuffer, uint8_t AdaptationFieldControl) {
    uint32_t *HeaderPtr = (uint32_t *) (PacketBuffer + 4);
    uint32_t HeaderData = xSwapBytes32(*HeaderPtr);

    uint32_t DC_mask   = 0b00000000100000000000000000000000;
    uint32_t RA_mask   = 0b00000000010000000000000000000000;
    uint32_t SP_mask   = 0b00000000001000000000000000000000;
    uint32_t PR_mask   = 0b00000000000100000000000000000000;
    uint32_t OR_mask   = 0b00000000000010000000000000000000;
    uint32_t SF_mask   = 0b00000000000001000000000000000000;
    uint32_t TP_mask   = 0b00000000000000100000000000000000;
    uint32_t EX_mask   = 0b00000000000000010000000000000000;

    m_AFL = HeaderData >> 24;
    m_DC = (HeaderData & DC_mask) >> 23;
    m_RA = (HeaderData & RA_mask) >> 22;
    m_SP = (HeaderData & SP_mask) >> 21;
    m_PR = (HeaderData & PR_mask) >> 20;
    m_OR = (HeaderData & OR_mask) >> 19;
    m_SF = (HeaderData & SF_mask) >> 18;
    m_TP = (HeaderData & TP_mask) >> 17;
    m_EX = (HeaderData & EX_mask) >> 16;

    return 0;
}

/// @brief Print all TS packet header fields
void xTS_AdaptationField::Print() const {
    printf("AF: L=%3d DC=%d RA=%d SP=%d PR=%d OR=%d SF=%d TP=%d EX=%d ", m_AFL, m_DC, m_RA, m_SP, m_PR, m_OR, m_SF,
           m_TP, m_EX);

}