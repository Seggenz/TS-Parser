#include "tsCommon.h"
#include "tsTransportStream.h"
//=============================================================================================================================================================================

int main(int argc, char *argv[ ], char *envp[ ])
{
  // DONE -> TODO - open file

  FILE* fp = std::fopen("../example_new.ts", "rb");

  // DONE -> TODO - check if file if opened

  if (!fp) {
      std:perror("File opening failed");
      return EXIT_FAILURE;
  }

  xTS_PacketHeader TS_PacketHeader;
  xTS_AdaptationField TS_AdaptationField;
  xPES_Assembler PES_Assembler;

  const uint8_t  TS_Buffer_Size = 188;
  uint8_t  TS_PacketBuffer[TS_Buffer_Size];
  int32_t TS_PacketId = 0;

  while(!std::feof(fp))
  {
      size_t NumRead = fread(TS_PacketBuffer, 1, xTS::TS_PacketLength, fp);
      if(NumRead != xTS::TS_PacketLength) { break; }
      TS_PacketHeader.Reset();
      TS_PacketHeader.Parse(TS_PacketBuffer);
      TS_AdaptationField.Reset();
      if(TS_PacketHeader.getSyncByte() == 'G' && TS_PacketHeader.get_mPID() == 136)
      {
          if(TS_PacketHeader.hasAdaptationField())
          {
              TS_AdaptationField.Parse(TS_PacketBuffer);
          }
          printf("%010d ", TS_PacketId);
          TS_PacketHeader.Print();
          if(TS_PacketHeader.hasAdaptationField()) { TS_AdaptationField.Print(); }
          xPES_Assembler::eResult Result = PES_Assembler.AbsorbPacket(TS_PacketBuffer, &TS_PacketHeader, &TS_AdaptationField);
          switch(Result)
          {
              case xPES_Assembler::eResult::StreamPackedLost : printf("PcktLost "); break;
              case xPES_Assembler::eResult::AssemblingStarted : printf("Started "); PES_Assembler.PrintPESH(); break;
              case xPES_Assembler::eResult::AssemblingContinue: printf("Continue "); break;
              case xPES_Assembler::eResult::AssemblingFinished: printf("Finished "); printf("PES: Len=%d", PES_Assembler.getNumPacketBytes()); break;
              default: break;
          }
          printf("\n");
      }


//      //
//    fread(TS_PacketBuffer, 1, 188, fp);
//
//    TS_PacketHeader.Reset();
//    TS_PacketHeader.Parse(TS_PacketBuffer);
//    printf("%010d ", TS_PacketId);
//    TS_PacketHeader.Print();
//
//
//    if(TS_PacketHeader.hasAdaptationField()) {
//        TS_AdaptationField.Reset();
//        TS_AdaptationField.Parse(TS_PacketBuffer);
//        TS_AdaptationField.Print();
//    }




//    printf("\n");

    TS_PacketId++;
  }



  // DONE -> TODO - close file
  std::fclose(fp);

  return EXIT_SUCCESS;
}

//=============================================================================================================================================================================
