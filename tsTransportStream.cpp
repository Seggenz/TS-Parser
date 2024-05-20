#include <iostream>
#include <cmath>
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
    uint32_t *HeaderPtr = (uint32_t *) Input; //
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
    printf("TS: ");
    printf("SB=%d ", m_SB);
    printf("E=%d ", m_E);
    printf("S=%d ", m_S);
    printf("T=%d ", m_T);
    printf("PID=%d ", m_PID);
    printf("TSC=%d ", m_TSC);
    printf("AFC=%d ", m_AFC);
    printf("CC=%d ", m_CC);
}


/// @brief Reset - reset all TS packet header fields
void xTS_AdaptationField::Reset() {
    m_AFL = 0;
    m_AdaptationFieldControl = 0;
}

/**
@brief Parse adaptation field
@param PacketBuffer is pointer to buffer containing TS packet
@param AdaptationFieldControl is value of Adaptation Field Control field of
corresponding TS packet header
@return Number of parsed bytes (length of AF or -1 on failure)
*/
int32_t xTS_AdaptationField::Parse(const uint8_t *PacketBuffer) {
    uint32_t *AFCPtr = (uint32_t *) (PacketBuffer +
                                     4); // bo długość headera to 4 bajty, więc żeby go ominąć to dodajemy 4 i przesuwamy się o 4 bajty tym samym
    uint32_t AFCData = xSwapBytes32(*AFCPtr);

    uint32_t DC_mask = 0b00000000100000000000000000000000;
    uint32_t RA_mask = 0b00000000010000000000000000000000;
    uint32_t SP_mask = 0b00000000000100000000000000000000;
    uint32_t PR_mask = 0b00000000000010000000000000000000;
    uint32_t OR_mask = 0b00000000000001000000000000000000;
    uint32_t SF_mask = 0b00000000000000100000000000000000;
    uint32_t TP_mask = 0b00000000000000010000000000000000;
    uint32_t EX_mask = 0b00000000000000001000000000000000;


    m_AFL = AFCData >> 24;
    m_DC = (AFCData bitand DC_mask) >> 23;
    m_RA = (AFCData bitand RA_mask) >> 22;
    m_SP = (AFCData bitand SP_mask) >> 21;
    m_PR = (AFCData bitand PR_mask) >> 20;
    m_OR = (AFCData bitand OR_mask) >> 19;
    m_SF = (AFCData bitand SF_mask) >> 18;
    m_TP = (AFCData bitand TP_mask) >> 17;
    m_EX = (AFCData bitand EX_mask) >> 16;

    return 0;
}

/// @brief Print all TS packet header fields
void xTS_AdaptationField::Print() const {
    printf("AF: L=%d ", m_AFL);
    printf("DC=%d ", m_DC);
    printf("RA=%d ", m_RA);
    printf("SP=%d ", m_SP);
    printf("PR=%d ", m_PR);
    printf("OR=%d ", m_OR);
    printf("SF=%d ", m_SF);
    printf("TP=%d ", m_TP);
    printf("EX=%d ", m_EX);
}


//=============================================================================================================================================================================
xPES_Assembler::xPES_Assembler() {
    m_Buffer = nullptr;
    m_BufferSize = 0;
    m_DataOffset = 0;
}

xPES_Assembler::~xPES_Assembler() {
    delete[] m_Buffer;
}

void xPES_Assembler::Init(int32_t PID) {
    m_PID = PID;
    m_BufferSize = static_cast<uint32_t>(m_PESH.getPacketLength());
    m_Buffer = new uint8_t[m_BufferSize];
}

xPES_Assembler::eResult
xPES_Assembler::AbsorbPacket(const uint8_t *TransportStreamPacket, const xTS_PacketHeader *PacketHeader,
                             const xTS_AdaptationField *AdaptationField) {

    if (PacketHeader->get_mPID() != 136) {
        return eResult::UnexpectedPID;
    }

    if (PacketHeader->getMS() == 1) { //payload unit starter
        xBufferReset();
        m_PESH.Reset();
        m_PESH.Parse(TransportStreamPacket + xTS::TS_HeaderLength + AdaptationField->getAdaptationFieldLength() + 1);
        Init(PacketHeader->get_mPID());
        xBufferAppend(TransportStreamPacket + xTS::TS_HeaderLength + AdaptationField->getAdaptationFieldLength() + 1,
                      188 - (4 + AdaptationField->getAdaptationFieldLength() + 1));
        return eResult::AssemblingStarted;
    } else {
        if (PacketHeader->hasAdaptationField()) {
            xBufferAppend(
                    TransportStreamPacket + xTS::TS_HeaderLength + AdaptationField->getAdaptationFieldLength() + 1,
                    188 - (xTS::TS_HeaderLength + AdaptationField->getAdaptationFieldLength() + 1));
        } else {
            xBufferAppend(TransportStreamPacket + 4, 184);
        }
        if ((m_DataOffset > m_BufferSize) || (PacketHeader->getMCc() == floor(m_BufferSize / 188))) {
            return eResult::AssemblingFinished;
        }
        return eResult::AssemblingContinue;
    }
}


void xPES_Assembler::xBufferReset() {
    m_Buffer = nullptr;
    m_BufferSize = 0;
    m_DataOffset = 0;
}

void xPES_Assembler::xBufferAppend(const uint8_t *Data, int32_t Size) {
    if (m_DataOffset + Size > m_BufferSize) {
        uint8_t *newBuffer = new uint8_t[m_DataOffset + Size];
        memcpy(newBuffer, m_Buffer, m_DataOffset);
        delete[] m_Buffer;
        m_Buffer = newBuffer;
        m_BufferSize = m_DataOffset + Size;
    }
    memcpy(m_Buffer + m_DataOffset, Data, Size);
    m_DataOffset += Size;

}

void xPES_PacketHeader::Reset() {
    m_PacketStartCodePrefix = 0;
    m_StreamId = 0;
    m_PacketLength = 0;
}

int32_t xPES_PacketHeader::Parse(const uint8_t *Input) {
    m_PacketStartCodePrefix = (Input[0] << 16) | (Input[1] << 8) |
                              Input[2]; //packet_start_code_prefix ma 3 bajty (24bity) i wartość 0x000001
    m_StreamId = Input[3];
    m_PacketLength = (Input[4] << 8) | Input[5]; //2 bajty
    return 0;

}

void xPES_PacketHeader::Print() const {
    printf("PSCP=%d SID=%d L=%d", m_PacketStartCodePrefix, m_StreamId, m_PacketLength);
}


