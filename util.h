#include "typedefs.h"
#ifndef UTILH
#define UTILH

namespace util
{
    char* ConcatStrings(const char* s1, const char* s2)
    {
        u32 s1_length = strlen(s1);
        u32 s2_length = strlen(s2);
        u32 arraySize = sizeof(char) * (s1_length + s2_length);
        char* resultBuffer = (char*) malloc(arraySize);
        resultBuffer[0] = '\0';
        strcat(resultBuffer, s1);
        strcat(resultBuffer, s2);
        return resultBuffer;
    }

    char* ReadFile(const char* filePath, const char* mode)
    {
        FILE *fp = fopen(filePath, mode);
        char* buffer = {};
        printf("Reading file: %s...\n", filePath);
        if (fp)
        {
            fseek( fp, 0, SEEK_END );
            u32 bufferSize = ftell(fp);
            fseek( fp, 0, SEEK_SET );

            buffer = (char *)calloc(1, bufferSize);
            fread(buffer, bufferSize, 1, fp);
            fclose(fp);
            return buffer;
        }
        else
        {
            printf("Invalid handle: %s...\n", filePath);
        }
        return buffer;

    }
}

#endif UTILH
