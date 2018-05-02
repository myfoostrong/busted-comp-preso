#include "ccapi.h"
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

CCAPI::CCAPI(const std::string& libpath)
    : ProcessId(~0), LibHandle(NULL), LibLoaded(false)
{
    LibLoaded = Init(libpath);
}
bool CCAPI::Init(const std::string& libpath)
{
	//HMODULE 
	cout << GetModuleFileNameA(NULL,NULL,NULL) << std::endl;
	cout << GetFileAttributesA(libpath.c_str()) << std::endl;
    LibHandle = LoadLibrary(libpath.c_str());
	cout << GetLastError() << std::endl;

    if (!LibHandle)
        return false;

    CCAPIConnectConsole = (int (__cdecl*)(const char*)) GetProcAddress(LibHandle,"CCAPIConnectConsole");
    CCAPIDisconnectConsole = (int (__cdecl*)()) GetProcAddress(LibHandle,"CCAPIDisconnectConsole");
    CCAPIGetConnectionStatus = (int (__cdecl*) (int*)) GetProcAddress(LibHandle,"CCAPIGetConnectionStatus");
    CCAPISetBootConsoleIds = (int (__cdecl*)(ConsoleIdType, int, const ConsoleId*)) GetProcAddress(LibHandle,"CCAPISetBootConsoleIds");
    CCAPISetConsoleIds = (int (__cdecl*)(ConsoleIdType, const ConsoleId*)) GetProcAddress(LibHandle,"CCAPISetConsoleIds");
    CCAPISetMemory = (int (__cdecl*)(u32, u64, u32, const void*)) GetProcAddress(LibHandle,"CCAPISetMemory");
    CCAPIGetMemory = (int (__cdecl*)(u32, u64, u32, void*)) GetProcAddress(LibHandle,"CCAPIGetMemory");
    CCAPIGetProcessList = (int (__cdecl*)(u32*, u32*)) GetProcAddress(LibHandle,"CCAPIGetProcessList");
    CCAPIGetProcessName = (int (__cdecl*)(u32, ProcessName*)) GetProcAddress(LibHandle,"CCAPIGetProcessName");
    CCAPIGetTemperature = (int (__cdecl*)(int* cell, int* rsx)) GetProcAddress(LibHandle,"CCAPIGetTemperature");
    CCAPIShutdown = (int (__cdecl*)(ShutdownMode)) GetProcAddress(LibHandle,"CCAPIShutdown");
    CCAPIRingBuzzer = (int (__cdecl*)(BuzzerType)) GetProcAddress(LibHandle,"CCAPIRingBuzzer");
    CCAPISetConsoleLed = (int (__cdecl*)(ColorLed, StatusLed)) GetProcAddress(LibHandle,"CCAPISetConsoleLed");
    CCAPIGetFirmwareInfo = (int (__cdecl*)(u32*, u32*, ConsoleType*)) GetProcAddress(LibHandle,"CCAPIGetFirmwareInfo");
    CCAPIVshNotify = (int (__cdecl*)(NotifyIcon, const char*)) GetProcAddress(LibHandle,"CCAPIVshNotify");
    CCAPIGetNumberOfConsoles = (int (__cdecl*)()) GetProcAddress(LibHandle,"CCAPIGetNumberOfConsoles");
    CCAPIGetConsoleInfo = (void (__cdecl*)(int, ConsoleName*, ConsoleIp*)) GetProcAddress(LibHandle,"CCAPIGetConsoleInfo");
    CCAPIGetDllVersion = (int (__cdecl*)()) GetProcAddress(LibHandle,"CCAPIGetDllVersion");


    bool loaded =  (CCAPIConnectConsole != NULL)
            && (CCAPIDisconnectConsole != NULL)
            && (CCAPIGetConnectionStatus != NULL)
            && (CCAPISetBootConsoleIds != NULL)
            && (CCAPISetConsoleIds != NULL)
            && (CCAPISetMemory != NULL)
            && (CCAPIGetMemory != NULL)
            && (CCAPIGetProcessList != NULL)
            && (CCAPIGetProcessName != NULL)
            && (CCAPIGetTemperature != NULL)
            && (CCAPIShutdown != NULL)
            && (CCAPIRingBuzzer != NULL)
            && (CCAPISetConsoleLed != NULL)
            && (CCAPIGetFirmwareInfo != NULL)
            && (CCAPIVshNotify != NULL)
            && (CCAPIGetNumberOfConsoles != NULL)
            && (CCAPIGetConsoleInfo != NULL)
            && (CCAPIGetDllVersion != NULL);

    return loaded;
}
CCAPI::~CCAPI()
{
    LibLoaded = false;
}
bool CCAPI::GetLibraryState() const
{
    return LibLoaded;
}
bool CCAPI::IsConnected() const
{
    int state;
    int res = CCAPIGetConnectionStatus(&state);
    return (res == CCAPI_OK) && state;
}
int CCAPI::Connect(const std::string &ip) const
{
    return CCAPIConnectConsole(ip.c_str());
}
int CCAPI::Disconnect() const
{
    return CCAPIDisconnectConsole();
}
int CCAPI::GetDllVersion() const
{
    return CCAPIGetDllVersion();
}
std::vector<CCAPI::ConsoleInfo> CCAPI::GetConsoleList() const
{
    std::vector<ConsoleInfo> list;
    list.reserve(CCAPIGetNumberOfConsoles());

    for (int i = 0; i<CCAPIGetNumberOfConsoles(); i++)
    {
        ConsoleName name;
        ConsoleIp ip;
        CCAPIGetConsoleInfo(i,&name,&ip);
        ConsoleInfo c = {name.value,ip.value};
        list.push_back(c);
    }
    return list;
}
std::vector<CCAPI::ProcessInfo> CCAPI::GetProcessList()
{
    std::vector<CCAPI::ProcessInfo> list;

    u32 ProcessIds[32];
    u32 NProcessIds = sizeof(ProcessIds)/sizeof(ProcessIds[0]);
    int ret = CCAPIGetProcessList(&NProcessIds,ProcessIds);
    if (ret != CCAPI_OK)
    {
        return list;
    }
    else
    {
        for (u32 i = 0; i < NProcessIds ;i++)
        {
            ProcessName name;
            int ret = CCAPIGetProcessName(ProcessIds[i],&name);
            if (ret != CCAPI_OK)
            {
                return list;
            }
            else
            {
                ProcessInfo info = {ProcessIds[i],name.value};
                list.push_back(info);
            }
        }
        return list;
    }
}
u32 CCAPI::GetAttachedProcess()
{
    return ProcessId;
}
void CCAPI::AttachProcess(u32 ProcessId)
{
    this->ProcessId = ProcessId;
}
int CCAPI::AttachGameProcess(bool* found, u32* ProcessId)
{
    *found = false;

    u32 ProcessIds[32];
    u32 NProcessIds = sizeof(ProcessIds)/sizeof(ProcessIds[0]);
    int ret = CCAPIGetProcessList(&NProcessIds,ProcessIds);
    if (ret != CCAPI_OK)
    {
        return ret;
    }
    else
    {
        for (u32 i = 0; i < NProcessIds ;i++)
        {
            ProcessName name;
            int ret = CCAPIGetProcessName(ProcessIds[i],&name);
            if (ret != CCAPI_OK)
            {
                return ret;
            }
            else
            {
                if (string(name.value).find("dev_flash") == string::npos)
                {
                    if (ProcessId)
                        *ProcessId = ProcessIds[i];
                    this->ProcessId = ProcessIds[i];
                    *found = true;
                    return ret;
                }
            }
        }
        if (ProcessId)
            *ProcessId = ~0;
        this->ProcessId = ~0;
        return ret;
    }
}
int CCAPI::ReadMemory(u64 address, u32 size, void *data) const
{
    return CCAPIGetMemory(ProcessId,address,size,data);
}
int CCAPI::WriteMemory(u64 address, u32 size, const void* data) const
{
    return CCAPISetMemory(ProcessId,address,size,data);
}
int CCAPI::GetTemperature(int* cell, int* rsx) const
{
    return CCAPIGetTemperature(cell,rsx);
}
int CCAPI::Shutdown(CCAPI::ShutdownMode mode) const
{
    return CCAPIShutdown(mode);
}
int CCAPI::RingBuzzer(CCAPI::BuzzerType type) const
{
    return CCAPIRingBuzzer(type);
}
int CCAPI::SetConsoleLed(CCAPI::ColorLed color, CCAPI::StatusLed status) const
{
    return CCAPISetConsoleLed(color,status);
}
int CCAPI::SetConsoleIds(CCAPI::ConsoleIdType idType, const CCAPI::ConsoleId* id) const
{
    return CCAPISetConsoleIds(idType,id);
}
int CCAPI::SetConsoleIds(ConsoleIdType idType, const std::string& id) const
{
    if (id.length() != 32)
    {
        return CCAPI_ERROR;
    }
    ConsoleId cid;
    StringToArray(id, cid.value);
    return SetConsoleIds(idType,&cid);
}
int CCAPI::SetBootConsoleIds(CCAPI::ConsoleIdType idType, const CCAPI::ConsoleId* id) const
{
    return CCAPISetBootConsoleIds(idType,1,id);
}
int CCAPI::SetBootConsoleIds(ConsoleIdType idType, const std::string& id) const
{
    if (id.length() != 32)
    {
        return CCAPI_ERROR;
    }
    ConsoleId cid;
    StringToArray(id, cid.value);
    return SetBootConsoleIds(idType,&cid);
}
int CCAPI::ResetBootConsoleIds(CCAPI::ConsoleIdType idType) const
{
    return CCAPISetBootConsoleIds(idType,0,NULL);
}
int CCAPI::VshNotify(CCAPI::NotifyIcon icon, const std::string& msg) const
{
    return CCAPIVshNotify(icon,msg.c_str());
}
int CCAPI::GetVersion(u32* version) const
{
    return CCAPIGetFirmwareInfo(NULL,version,NULL);
}
int CCAPI::GetFirmware(u32* firmware) const
{
    return CCAPIGetFirmwareInfo(firmware,NULL,NULL);
}
int CCAPI::GetConsoleType(ConsoleType* ctype) const
{
    return CCAPIGetFirmwareInfo(NULL,NULL,ctype);
}
std::string CCAPI::FirmwareToString(u32 firmware)
{
    std::stringstream s;

    s << std::hex << (firmware >> 24);
    s << ".";
    s << std::hex << ((firmware>>12) & 0xFF);

    return s.str();
}
std::string CCAPI::ConsoleTypeToString(ConsoleType cType)
{
    std::string s("UNK");

    switch(cType)
    {
        case CEX:
            s = "CEX";
        break;

        case DEX:
            s = "DEX";
        break;

        case TOOL:
           s = "TOOL";
        break;

        default:
        break;
    }

    return s;
}
u8* CCAPI::StringToArray(std::string s, u8* id)
{
    if (!s.length())
    {
        return id;
    }
    if (s.length() % 2)
    {
        s += '0';
    }

    int j = 0;
    for (u32 i=0;i<s.length();i+=2)
    {
        std::string b = s.substr(i,2);
        id[j++] = strtoul(b.c_str(),NULL,16);
    }
    return id;
}
template <>
std::string CCAPI::ReadMemory<std::string>(u64 address, int* ret) const
{
    std::string s;

    while(1)
    {
        s8 chunk[0x100];
        int r = ReadMemory(address,sizeof(chunk),chunk);
        if (r != CCAPI_OK)
        {
            if (ret)
                *ret = r;
            break;
        }
        else
        {
            for (u32 i = 0;i < sizeof(chunk);i++)
            {
                if (!chunk[i])
                {
                    s.append(chunk, i);
                    goto end;
                }
            }

            address += sizeof(chunk);
            s.append(chunk, sizeof(chunk));
        }
    }

    end:

    return s;
}

template <>
int CCAPI::WriteMemory<std::string>(u64 address, std::string data) const
{
    return WriteMemory(address,data.size()+1,data.c_str());
}
