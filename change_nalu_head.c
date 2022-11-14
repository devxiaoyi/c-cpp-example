#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

#define CHK(condition, errRet)    \
    do                            \
    {                             \
        if (!(condition))         \
        {                         \
            retStatus = (errRet); \
            goto CleanUp;         \
        }                         \
    } while (0)

int readFile(char *filePath, uint8_t *pBuffer, int32_t *pSize)
{
    int32_t fileLen;
    int retStatus = 0;
    FILE *fp = NULL;

    CHK(filePath != NULL && pSize != NULL, -1);

    errno = 0;
    fp = fopen(filePath, "rb");
    if (fp == NULL)
    {
        printf("Error no.%d: %s\n", errno, strerror(errno));
    }

    CHK(fp != NULL, -2);

    // Get the size of the file
    fseek(fp, 0, SEEK_END);
    fileLen = ftell(fp);

    if (pBuffer == NULL)
    {
        // requested the length - set and early return
        CHK(0, 0);
    }
    *pSize = fileLen;

    // Validate the buffer size
    CHK(fileLen <= *pSize, -4);

    // Read the file into memory buffer
    fseek(fp, 0, SEEK_SET);
    CHK(fread(pBuffer, (size_t)fileLen, 1, fp) == 1, -5);

CleanUp:

    if (fp != NULL)
    {
        fclose(fp);
        fp = NULL;
    }

    return retStatus;
}


int main()
{
    const int buffLen = 200 * 1024 * 1024;
    uint8_t *tmpbuff = (uint8_t *)calloc(1, buffLen);
    uint8_t *outbuff = (uint8_t *)calloc(1, buffLen);
    if (tmpbuff == NULL || outbuff == NULL)
    {
        exit(0);
    }
    int32_t tmpsize = 0;

    readFile("outfile.h264", tmpbuff, &tmpsize);

    FILE *fp = fopen("outchd.h264","w+");

    int j = 0;
    for (int i = 0; i < tmpsize; i++)
    {
        if(tmpbuff[i] == 0 && tmpbuff[i+1] == 0 && tmpbuff[i+2] == 0 && tmpbuff[i+3] == 1)
        {
            tmpbuff[i+4] += 0x40;
        }
        outbuff[j++] = tmpbuff[i];
    }

    fwrite(outbuff, j, 1, fp);
    return 0;
}
