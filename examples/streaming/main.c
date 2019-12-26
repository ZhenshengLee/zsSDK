// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <stdio.h>
#include <stdlib.h>
#include <zsa/zsa.h>

int main(int argc, char **argv)
{
    int returnCode = 1;

    uint32_t device_count = zsa_device_get_installed_count();

    // device是机器人本体中的所有视觉传感设备
    // zsa_device_t 就是一个指针
    zsa_device_t device = NULL;
    // 初始化硬件和通信，目前没有硬件
    if (ZSA_RESULT_SUCCEEDED != zsa_device_open(ZSA_DEVICE_DEFAULT, &device))
    {
        printf("Failed to open device\n");
        goto Exit;
    }

    returnCode = 0;
Exit:
    if (device != NULL)
    {
        zsa_device_close(device);
    }

    return returnCode;
}
