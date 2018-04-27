
#include "ccapi.h"
#include <stdio.h>

int getProcs()
{
    u32 npid;
    CCAPIGePro
}

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;


    if (CCAPIInit("CCAPI.dll") != CCAPI_OK)
    {
        printf ("CCAPI Library couldn't be loaded.\n");
        printf ("Check that you are using the correct CCAPI.dll.\n");
    }
    else
    {
        int c;
        for (c = 0; c < CCAPIGetNumberOfConsoles(); ++c)
        {
            ConsoleName name;
            ConsoleIp ip;
            CCAPIGetConsoleInfo(c,&name,&ip);
            printf("Console: %s Ip: %s\n", name.value, ip.value);
        }

        if (CCAPIConnect("192.168.0.2") != CCAPI_OK) //_PS3_IP_HERE_
        {
            printf("Couldn't establish a connection with your PS3.\n");
            printf("Verify your ps3 ip.\n");
        }
        else
        {
            CCAPIConnect("192.168.0.2");
            CCAPIDisconnect();
            CCAPIConnect("192.168.0.2");
            printf("Connected to your PS3.\n");
            CCAPIVshNotify(NotifyTrophy2, "Connected to CCAPI");
            CCAPIRingBuzzer(BuzzerTriple);

            u32 firmware, version;
            ConsoleType cType;
            int cell, rsx;
            int ret = CCAPIGetFirmware(&firmware) |
                      CCAPIGetConsoleType(&cType) |
                      CCAPIGetVersion(&version)   |
                      CCAPIGetTemperature(&cell,&rsx);

            if (ret != CCAPI_OK)
            {
                printf("An error occured while retrieving console details.\n");
            }
            else
            {
                printf("CCAPI Version: %d\n",version);
                printf("CCAPI Library version: %d\n",CCAPIGetDllVersion());
                char firmware_s[20];
                printf("Firmware: %s\n", CCAPIFirmwareToString(firmware,firmware_s,sizeof(firmware_s)));
                printf("Console Type: %s\n",CCAPIConsoleTypeToString(cType));
                printf("CELL Temp: %d\n",cell);
                printf("RSX Temp: %d\n",rsx);

                int found = 0;
                u32 pid = ~0;

                if (CCAPIAttachGameProcess(&found,&pid) != CCAPI_OK)
                {
                    printf("An error occured while attaching game process.\n");
                }
                else if (!found)
                {
                    printf("Couldn't find a game process.\n");
                }
                else
                {
                    printf("Process %x attached with success.\n",pid);

                    u8 i = CCAPIReadMemoryI8(0x50000,NULL);
                    printf("Read at 0x50000: %x\n",(u32)i);
                }
            }
        }
    }
    CCAPIDisconnect();
    CCAPIEnd();

    printf("Press any key to exit\n");
    getchar();

    return 0;
}
