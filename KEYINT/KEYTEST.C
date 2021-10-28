#include <stdio.h>
#include "key.h"

main()
{
    printf("Multiple key test program by Patch\n");
    printf("컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴\n");
    printf("Read KEY.DOC for more info.\n");
    printf("The magic key is LEFTSHIFT and KEYPAD8.\n\n");
    printf("Hit any key to start the test (ESC quits) ...\n");
    getch();

    Set_New_Int9();

    while (1)
    {
        if (keys[kESC] == 1)
            break;

        if (keys[kLEFTSHIFT] == 1 && keys[kKEYPAD8] == 1)
            printf("You pressed LEFTSHIFT and KEYPAD8!\n");
    }

    Set_Old_Int9();
}
