#include "PES_Assembler.h"
#include <iostream>

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

    if (PacketHeader->getStartByte() == 1) {
        xBufferReset();
        m_PESH.Reset();
        m_PESH.Parse(TransportStreamPacket + xTS::TS_HeaderLength + AdaptationField->getAdaptationFieldLength() + 1);
        Init(PacketHeader->getPID());
        xBufferAppend(TransportStreamPacket + xTS::TS_HeaderLength + AdaptationField->getAdaptationFieldLength() + 1,
                      xTS::TS_PacketLength - (xTS::TS_HeaderLength + AdaptationField->getAdaptationFieldLength() + 1));
        return eResult::AssemblingStarted;
    } else {
        if (PacketHeader->hasAdaptationField()) {
            xBufferAppend(
                    TransportStreamPacket + xTS::TS_HeaderLength + AdaptationField->getAdaptationFieldLength() + 1,
                    xTS::TS_PacketLength - (xTS::TS_HeaderLength + AdaptationField->getAdaptationFieldLength() + 1));

        } else {
            xBufferAppend(TransportStreamPacket + xTS::TS_HeaderLength, xTS::TS_PacketLength - xTS::TS_HeaderLength);
        }

        if ((PacketHeader->getCC() == 15)) {
            SaveToFile();
            return eResult::AssemblingFinished;
        }

        return eResult::AssemblingContinue;

    }
}

void xPES_Assembler::SaveToFile() {
    FILE *file = std::fopen("../PID136.mp2", "ab");
    if (file != nullptr) {
        fwrite(m_Buffer, sizeof(uint8_t), m_DataOffset, file);
        fclose(file);
    }
}

void xPES_Assembler::xBufferReset() {
    delete[] m_Buffer;
    m_Buffer = nullptr;
    m_BufferSize = 0;
    m_DataOffset = 0;
}

void xPES_Assembler::xBufferAppend(const uint8_t *Data, int32_t Size) {
    if (m_DataOffset + Size > m_BufferSize) {
        uint32_t new_BufferSize = m_DataOffset + Size;
        uint8_t *new_Buffer = new uint8_t[new_BufferSize];
        memcpy(new_Buffer, m_Buffer, m_DataOffset);
        delete[] m_Buffer;
        m_Buffer = new_Buffer;
        m_BufferSize = new_BufferSize;
    }
    memcpy(m_Buffer + m_DataOffset, Data, Size);
    m_DataOffset += Size;
}
