#include "PES_PacketHeader.h"
#include <iostream>

void xPES_PacketHeader::Reset() {
    m_PacketStartCodePrefix = 0;
    m_StreamId = 0;
    m_PacketLength = 0;
}

int32_t xPES_PacketHeader::Parse(const uint8_t *Input) {
    uint64_t *HeaderPtr = (uint64_t *) Input;
    uint64_t HeaderData = xSwapBytes64(*HeaderPtr);

    uint64_t PSCPrefix_mask    = 0b0000000000000000000000010000000000000000000000000000000000000000;
    uint64_t StreamId_mask     = 0b0000000000000000000000001111111100000000000000000000000000000000;
    uint64_t PacketLength_mask = 0b0000000000000000000000000000000011111111111111110000000000000000;

    m_PacketStartCodePrefix = (HeaderData & PSCPrefix_mask) >> 40;
    m_StreamId = (HeaderData & StreamId_mask) >> 32;
    m_PacketLength = (HeaderData & PacketLength_mask) >> 16;

    return 0;

}

void xPES_PacketHeader::Print() const {
    printf("PES: PSCP=%d SID=%3d L=%4d ", m_PacketStartCodePrefix, m_StreamId, m_PacketLength);

}
