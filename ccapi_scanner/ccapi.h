/**
*
* CCAPI C Wrapper made by Enstone
* Compatible with CCAPI 2.60, CCAPI 2.70, CCAPI 2.80, +
* Requires CCAPI.dll
* V1.00
*
**/

#ifndef CCAPI_H
#define CCAPI_H

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "ccapi_types.h"

#define LITTLE_ENDIAN
#define CCAPI_OK 0
#define CCAPI_ERROR -1

//structs
struct ConsoleId
{
    u8 value[16];
};
struct ProcessName
{
    char value[512];
};
struct ConsoleName
{
    char value[256];
};
struct ConsoleIp
{
    char value[256];
};

//enums
enum ConsoleIdType
{
    Idps = 0,
    Psid = 1,
};
enum ShutdownMode
{
    ActionShutdown = 1,
    ActionSoftReboot = 2,
    ActionHardReboot = 3,
};
enum BuzzerType
{
    BuzzerContinious = 0,
    BuzzerSingle = 1,
    BuzzerDouble = 2,
    BuzzerTriple = 3,
};
enum ColorLed
{
    LedGreen = 1,
    LedRed = 2,
};
enum StatusLed
{
    LedOff = 0,
    LedOn = 1,
    LedBlink = 2,
};
enum NotifyIcon
{
    NotifyInfo = 0,
    NotifyCaution = 1,
    NotifyFriend = 2,
    NotifySlider = 3,
    NotifyWrongWay = 4,
    NotifyDialog = 5,
    NotifyDalogShadow = 6,
    NotifyText = 7,
    NotifyPointer = 8,
    NotifyGrab = 9,
    NotifyHand = 10,
    NotifyPen = 11,
    NotifyFinger = 12,
    NotifyArrow = 13,
    NotifyArrowRight = 14,
    NotifyProgress = 15,
    NotifyTrophy1 = 16,
    NotifyTrophy2 = 17,
    NotifyTrophy3 = 18,
    NotifyTrophy4 = 19
};
enum ConsoleType
{
    UNK = 0,
    CEX = 1,
    DEX = 2,
    TOOL = 3,
};

typedef struct ConsoleId ConsoleId;
typedef struct ProcessName ProcessName;
typedef struct ConsoleName ConsoleName;
typedef struct ConsoleIp ConsoleIp;

//enums
typedef enum ConsoleIdType ConsoleIdType;
typedef enum ShutdownMode ShutdownMode;
typedef enum BuzzerType BuzzerType;
typedef enum ColorLed ColorLed;
typedef enum StatusLed StatusLed;
typedef enum NotifyIcon NotifyIcon;
typedef enum ConsoleType ConsoleType;

/**functions**/
int CCAPIInit(const char* path);
int CCAPIEnd();
int CCAPIGetLibraryState();
u32 CCAPIGetAttachedProcess();
int CCAPIAttachProcess(u32 pid);
int CCAPIAttachGameProcess(int* found, u32* processId);
int CCAPIConnect(const char* ip);
int CCAPIDisconnect();
int CCAPISetBootConsoleIds (ConsoleIdType idType, const ConsoleId* id);
int CCAPISetBootConsoleIdsString(ConsoleIdType idType, const char* id);
int CCAPIResetBootConsoleIds (ConsoleIdType idType);
int CCAPISetConsoleIds (ConsoleIdType idType, const ConsoleId* id);
int CCAPISetConsoleIdsString(ConsoleIdType idType, const char* id);
int CCAPIWriteMemory (u64 address, u32 size, const void* data);
int CCAPIWriteMemoryI8 (u64 address, u8 data);
int CCAPIWriteMemoryI32 (u64 address, u32 data);
int CCAPIWriteMemoryF32 (u64 address, f32 data);
int CCAPIWriteMemoryI64 (u64 address, u64 data);
int CCAPIWriteMemoryF64 (u64 address, f64 data);
int CCAPIReadMemory (u64 address, u32 size, void* data);
u8  CCAPIReadMemoryI8 (u64 address, int* ret);
u32 CCAPIReadMemoryI32 (u64 address, int* ret);
f32 CCAPIReadMemoryF32 (u64 address, int* ret);
u64 CCAPIReadMemoryI64 (u64 address, int* ret);
f64 CCAPIReadMemoryF64 (u64 address, int* ret);
int CCAPIGetProcessList (u32* npid, u32* pids);
int CCAPIGetProcessName (u32 pid, ProcessName* name);
int CCAPIGetTemperature (int* cell, int* rsx);
int CCAPIShutdown (ShutdownMode mode);
int CCAPIRingBuzzer (BuzzerType type);
int CCAPISetConsoleLed (ColorLed color, StatusLed status);
int CCAPIGetVersion(u32* version);
int CCAPIGetFirmware(u32* firmware);
int CCAPIGetConsoleType(ConsoleType* ctype);
int CCAPIVshNotify (NotifyIcon icon, const char* msg);
int CCAPIGetNumberOfConsoles();
void CCAPIGetConsoleInfo(int index, ConsoleName* name, ConsoleIp* ip);
int CCAPIGetDllVersion();
char* CCAPIFirmwareToString(u32 firmware, char* s, int size);
const char* CCAPIConsoleTypeToString(ConsoleType cType);
int CCAPIWriteString(u64 address, const char* str);
int CCAPIReadString(u64 address, char* str, int size);
u8* CCAPIStringToArray(const char* s, u8* id);
#endif

