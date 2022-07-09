#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <ctype.h>

#define BUFFER_SIZE                 256

static uint8_t _buf[BUFFER_SIZE];


void hexDump(void * buffer, uint32_t bufferLen)
{
    int                     i;
    int                     j = 0;
    uint8_t *               buf;
    static char             szASCIIBuf[17];
    static uint32_t         offset = 0;

    buf = (uint8_t *)buffer;

    for (i = 0;i < bufferLen;i++) {
        if ((i % 16) == 0) {
            if (i != 0) {
                szASCIIBuf[j] = 0;
                j = 0;

                printf("  |%s|\n", szASCIIBuf);
            }
                
            printf("%08X\t", offset);
            offset += 16;
        }

        if ((i % 2) == 0 && (i % 16) > 0) {
            printf(" ");
        }

        printf("%02X", buf[i]);
        szASCIIBuf[j++] = isprint(buf[i]) ? buf[i] : '.';
    }

    /*
    ** Print final ASCII block...
    */
    szASCIIBuf[j] = 0;
    printf("  |%s|\n", szASCIIBuf);
}

int main(int argc, char ** argv)
{
    FILE *          fptr;
    char *          pszFilename;
    uint8_t *       buffer;
    uint32_t        bytesRead;

    if (argc > 1) {
        pszFilename = strdup(&argv[1][0]);
    }
    else {
        printf("Usage: xdump <filename>\n\n");
        return 0;
    }

    buffer = _buf;

    fptr = fopen(pszFilename, "rb");

    if (fptr == NULL) {
        fprintf(stderr, "Failed to open file %s:%s\n", pszFilename, strerror(errno));
        exit(-1);
    }

    while (!feof(fptr)) {
        bytesRead = fread(buffer, 1, BUFFER_SIZE, fptr);

        if (bytesRead > 0) {
            hexDump(buffer, bytesRead);
        }
    }

    fclose(fptr);

    return 0;
}
