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

  const uint8_t  TS_Buffer_Size = 188;
  uint8_t  TS_PacketBuffer[TS_Buffer_Size];
  int32_t TS_PacketId = 0;



  while(!std::feof(fp))
  {
    // DONE -> TODO - read from file
    fread(TS_PacketBuffer, 1, 188, fp);

    TS_PacketHeader.Reset();
    TS_PacketHeader.Parse(TS_PacketBuffer);
    printf("%010d ", TS_PacketId);
    TS_PacketHeader.Print();

    if(TS_PacketHeader.hasAdaptationField()) {
        TS_AdaptationField.Reset();
        TS_AdaptationField.Parse(TS_PacketBuffer);
        TS_AdaptationField.Print();
    }




    printf("\n");

    TS_PacketId++;
  }



  // DONE -> TODO - close file
  std::fclose(fp);

  return EXIT_SUCCESS;
}

//=============================================================================================================================================================================
