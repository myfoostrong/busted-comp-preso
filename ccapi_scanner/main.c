
#include "ccapi.h"
#include <stdio.h>

#define BLOCK_LENGTH=1024;

u32 chooseProc()
{
    u32 npid;
    CCAPIGeProcessList(&npid,0);

    u32* pids = (u32*) malloc(npid * sizeof(u32));
    if ( CCAPIGetProcessList(&npid,pids) == CCAPI_OK )
    {
        printf("Running Processes:\n__________________\n");
        int i;
        for (i=0;i<npid;i++)
        {
            printf("Process %d : id %x",i,pids[i]);
            ProcessName name;
            if (CCAPIGetProcessName(pids[i],&name) == CCAPI_OK)
            {
                printf(" Name: %s\n",name.value)
            }else
            {
                printf("Could not get Process Name\n")
            }
        }
        printf("\n");
    }else
    {
        printf("Could not find Process List\n")
    }

    printf("Which process # would you like to target? Enter #[1-%d]: ",npid)
    int p = getchar();

    u32 pid = pids[p];
    printf("Targetting PID %x",pid);
    return pid;
}


int certScan(pid)
{

    char data[BLOCK_LENGTH];
    u64 address = 0;
    bool match = false;

    static const char certStart[] = "-----BEGIN CERTIFICATE-----";
    const int startLen = 27;
    static const char firstChar = '-';

    static const char certEnd[] =   "-----END CERTIFICATE-----";
    const int endLen = 25;

    if ( CCAPIGetMemory(pid,address,BLOCK_LENGTH,&data) == CCAPI_OK )
    {
        int i;
        address = address + BLOCK_LENGTH;
        for (i=0;i<BLOCK_LENGTH;i++)
        {
            if(data[i] == firstChar)
            {
                if (i < BLOCK_LENGTH - startLen)
                {
                    int a = strncmp(certStart,data+i,startLen);
                    if (a == 0)
                    {
                        return address + i;
                    }else
                    {
                        continue;
                    }
                }else
                {
                    //Deal with it
                }
            }
        }
    }
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
            printf("Connected to your PS3.\n");
            CCAPIVshNotify(NotifyTrophy2, "Connected to CCAPI");
            CCAPIRingBuzzer(BuzzerTriple);

            u32 targetPid;
            u64 certificateAddress;
            targetPid = chooseProc();

            certificateAddress = certScan(targetPid);

            printf("Found an SSL Certificate at adress: %x\n",certificateAddress)
        }
    }
    CCAPIDisconnect();
    CCAPIEnd();

    printf("Press any key to exit\n");
    getchar();

    return 0;
}
