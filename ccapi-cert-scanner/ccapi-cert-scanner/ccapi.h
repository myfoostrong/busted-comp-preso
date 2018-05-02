/**
*
* CCAPI C++ Wrapper made by Enstone
* Compatible with CCAPI 2.60, CCAPI 2.70, CCAPI 2.80, +
* Requires CCAPI.dll
* V1.00
*
**/

#ifndef CCAPI_H
#define CCAPI_H


#include <string>
#include <vector>
#include <algorithm>
#include <windows.h>
#include "ccapi_types.h"

#define LITTLE_ENDIAN
#define CCAPI_OK 0
#define CCAPI_ERROR -1

class CCAPI
{
public:
    struct ProcessInfo
    {
        u32 pid;
        std::string name;
    };
	struct ConsoleInfo
	{
		std::string name,ip;
	};
    struct ConsoleId
    {
        u8 value[16];
    };
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

protected:
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

	
public:
    CCAPI(const std::string& libpath = "CCAPI.dll");
    virtual ~CCAPI();
    bool GetLibraryState() const;
    int GetDllVersion() const;
    int Connect(const std::string& ip) const;
    int Disconnect() const;
    bool IsConnected() const;
    std::vector<ConsoleInfo> GetConsoleList() const;
    std::vector<ProcessInfo> GetProcessList();
    u32 GetAttachedProcess();
	void AttachProcess(u32 ProcessId);
    int AttachGameProcess(bool* found, u32* ProcessId = NULL);
    int ReadMemory(u64 address, u32 size, void* data) const;
    int WriteMemory(u64 address, u32 size, const void* data) const;
    int GetTemperature(int* cell, int* rsx) const;
    int Shutdown(ShutdownMode mode) const;
    int RingBuzzer(BuzzerType type) const;
    int SetConsoleLed(ColorLed led, StatusLed status) const;
    int SetConsoleIds(ConsoleIdType idType, const std::string& id) const;
    int SetConsoleIds(ConsoleIdType idType, const ConsoleId* id) const;
    int SetBootConsoleIds(ConsoleIdType idType, const std::string& id) const;
    int SetBootConsoleIds(ConsoleIdType idType, const ConsoleId* id) const;
    int ResetBootConsoleIds(ConsoleIdType idType) const;
    int VshNotify(NotifyIcon icon, const std::string& msg) const;
    int GetVersion(u32* version) const;
    int GetFirmware(u32* firmware) const;
    int GetConsoleType(ConsoleType* ctype) const;

	template <typename T>
    T ReadMemory(u64 address, int* ret = NULL) const;

    template <typename T>
    int WriteMemory(u64 address, T data) const;
	
    static std::string FirmwareToString(u32 firmware);
    static std::string ConsoleTypeToString(ConsoleType cType);
    static u8* StringToArray(std::string s, u8* id);

protected:
	u32 ProcessId;

	
private:
    HMODULE LibHandle;
    bool LibLoaded;
    bool Init(const std::string& path);
	int (__cdecl* CCAPIConnectConsole)(const char* ip);
	int (__cdecl* CCAPIDisconnectConsole) ();
    int (__cdecl* CCAPIGetConnectionStatus) (int* status);
    int (__cdecl* CCAPISetBootConsoleIds) (ConsoleIdType idType, int on, const ConsoleId* id);
    int (__cdecl* CCAPISetConsoleIds) (ConsoleIdType idType, const ConsoleId* id);
	int (__cdecl* CCAPISetMemory) (u32 pid, u64 address, u32 size, const void* data);
	int (__cdecl* CCAPIGetMemory) (u32 pid, u64 address, u32 size, void* data);
	int (__cdecl* CCAPIGetProcessList) (u32* npid, u32* pids);
    int (__cdecl* CCAPIGetProcessName) (u32 pid, ProcessName* name);
	int (__cdecl* CCAPIGetTemperature) (int* cell, int* rsx);
	int (__cdecl* CCAPIShutdown) (ShutdownMode mode);
	int (__cdecl* CCAPIRingBuzzer) (BuzzerType type);
	int (__cdecl* CCAPISetConsoleLed) (ColorLed color, StatusLed status);
    int (__cdecl* CCAPIGetFirmwareInfo) (u32* firmware, u32* ccapi, ConsoleType* cType);
    int (__cdecl* CCAPIVshNotify) (NotifyIcon icon, const char* msg);
	int (__cdecl* CCAPIGetNumberOfConsoles)();
   void (__cdecl* CCAPIGetConsoleInfo)(int index, ConsoleName* name, ConsoleIp* ip);
	int (__cdecl* CCAPIGetDllVersion) ();

private:
    CCAPI(const CCAPI& ccapi);
    CCAPI& operator=(const CCAPI& ccapi);
};
/**
    Memory functions
    -- to be used with primitive types only + std::string--
**/

template <typename T>
T CCAPI::ReadMemory(u64 address, int* ret) const
{
    T data;
    int r = CCAPIGetMemory(ProcessId,address,sizeof(T),&data);
    if (ret)
    {
        *ret = r;
    }
#ifdef LITTLE_ENDIAN
    u8* data8 = (u8*) &data;
    std::reverse(data8,data8+sizeof(T));
#endif
    return data;
}

template <>
std::string CCAPI::ReadMemory<std::string>(u64 address, int* ret) const;


template <typename T>
int CCAPI::WriteMemory(u64 address, T data) const
{
#ifdef LITTLE_ENDIAN
    u8* data8 = (u8*) &data;
    std::reverse(data8,data8+sizeof(T));
#endif
    return CCAPISetMemory(ProcessId,address,sizeof(T),&data);
}

template <>
int CCAPI::WriteMemory<std::string>(u64 address, std::string data) const;


#endif

