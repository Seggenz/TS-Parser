#include "tsCommon.h"
#include "tsTransportStream.h"
#include <iostream>
#include "PES_Assembler.h"
//=============================================================================================================================================================================

int main(int argc, char *argv[], char *envp[]) {
    FILE *fp = std::fopen("../example_new.ts", "rb");


    if (!fp) {
        std::perror("File opening failed");
        return EXIT_FAILURE;
    }

    xTS_PacketHeader TS_PacketHeader;
    uint8_t TS_PacketBuffer[188];
    int32_t TS_PacketId = 0;
    xTS_AdaptationField TS_AF;
    xPES_Assembler PES_Assembler;


    while (!feof(fp)) {
        fread(TS_PacketBuffer, 1, 188, fp);
        TS_PacketHeader.Reset();
        TS_PacketHeader.Parse(TS_PacketBuffer);
        TS_AF.Reset();
        if (TS_PacketHeader.getSyncByte() == 'G' && TS_PacketHeader.getPID() == 136) {
            if (TS_PacketHeader.hasAdaptationField()) {
                TS_AF.Parse(TS_PacketBuffer, TS_PacketHeader.getAFCByte());
            }

            printf("%010d ", TS_PacketId);
            TS_PacketHeader.Print();
//            if (TS_PacketHeader.hasAdaptationField())
//                TS_AF.Print();

            xPES_Assembler::eResult Result = PES_Assembler.AbsorbPacket(TS_PacketBuffer, &TS_PacketHeader, &TS_AF);
            switch(Result)
            {
                case xPES_Assembler::eResult::StreamPackedLost : printf("PcktLost "); break;
                case xPES_Assembler::eResult::AssemblingStarted : printf("Started  "); PES_Assembler.PrintPESH(); break;
                case xPES_Assembler::eResult::AssemblingContinue: printf("Continue "); break;
                case xPES_Assembler::eResult::AssemblingFinished: printf("Finished "); printf("PES: Len=%d", PES_Assembler.getNumPacketBytes()); break;
                default: break;
            }
            printf("\n");
        }
        TS_PacketId++;
    }

    std::fclose(fp);

    return EXIT_SUCCESS;
}

//=============================================================================================================================================================================
