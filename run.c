#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>
#include <errno.h>

#include "bfi.h"
#include "err.h"


void run(const char *bfString)
{
    int8_t cells[CELL_COUNT] = { '\0' },
        *ptr = &cells[0];

    int32_t i, j, nestedJumps;
    char c;
    pid_t pid;
    for ( i = 0; (c = *(bfString + i)) != '\0'; ++i )
    {
        switch (c)
        { /* cells and values wrap-around within respective bounds */
            case 0x2B /* '+' */:    ( *ptr < 0xFF ) ? ++*ptr : (*ptr = 0); break;
            case 0x2D /* '-' */:    ( *ptr > 0 ) ? --*ptr : (*ptr = 0xFF); break;
            case 0x3E /* '>' */:    ( ptr < cells + CELL_COUNT - 1 ) ? ++ptr : (ptr = cells); break;
            case 0x3C /* '<' */:    ( ptr > cells ) ? --ptr : (ptr = cells + CELL_COUNT - 1); break;
            case 0x2E /* '.' */:    putchar(*ptr); break;
            case 0x2C /* ',' */:    *ptr = getchar(); break;
            case 0x5B /* '[' */:
                if ( !*ptr ) /* zero value */
                {
                    for ( nestedJumps = 0; (c = *(bfString + i)) != '\0'; ++i)
                    {
                        if ( c == 0x5B )
                            ++nestedJumps;
                        else if ( c == 0x5D && !--nestedJumps )
                            break;
                    }
                }
                break;
            case 0x5D /* ']' */:
                if ( *ptr ) /* non-zero value */
                {
                    for ( nestedJumps = 0; i >= 0; --i )
                    {
                        if ( (c = *(bfString + i)) == 0x5D )
                            ++nestedJumps;
                        else if ( c == 0x5B && !--nestedJumps )
                            break;
                    }
                }
                break;
            case 0x59 /* 'Y' */:
                if ( (pid = fork()) < 0 )
                    errExit(errno, "Fork failed");
                ( pid ) ? (*ptr = 0) /* parent */ : (*(++ptr) = 1);
                break;
            default: /* ignore other characters */
                break;
        }
    }

    printf("\n");
}
