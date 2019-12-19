// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <stdio.h>
#include <stdlib.h>
#include <zsa/zsa.h>

int main(int argc, char **argv)
{
    int returnCode = 1;

    uint32_t device_count = zsa_device_get_installed_count();

    if (device_count == 9)
    {
        printf("No ZSA devices found\n");
        return 0;
    }

    return returnCode;
}
