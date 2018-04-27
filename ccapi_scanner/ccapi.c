#include "ccapi.h"

#define LITTLE_ENDIAN

static HMODULE LibHandle = NULL;
static int LibLoaded = 0;
static u32 ProcessId = ~0;
static int (__cdecl* _CCAPIConnectConsole) (const char* ip) = NULL;
static int (__cdecl* _CCAPIDisconnectConsole) () = NULL;
static int (__cdecl* _CCAPIGetConnectionStatus) (int* status) = NULL;
static int (__cdecl* _CCAPISetBootConsoleIds) (ConsoleIdType idType, int on, const ConsoleId* id)= NULL;
static int (__cdecl* _CCAPISetConsoleIds) (ConsoleIdType idType, const ConsoleId* id)= NULL;
static int (__cdecl* _CCAPISetMemory) (u32 pid, u64 address, u32 size, const void* data)= NULL;
static int (__cdecl* _CCAPIGetMemory) (u32 pid, u64 address, u32 size, void* data)= NULL;
static int (__cdecl* _CCAPIGetProcessList) (u32* npid, u32* pids)= NULL;
static int (__cdecl* _CCAPIGetProcessName) (u32 pid, ProcessName* name)= NULL;
static int (__cdecl* _CCAPIGetTemperature) (int* cell, int* rsx)= NULL;
static int (__cdecl* _CCAPIShutdown) (ShutdownMode mode)= NULL;
static int (__cdecl* _CCAPIRingBuzzer) (BuzzerType type)= NULL;
static int (__cdecl* _CCAPISetConsoleLed) (ColorLed color, StatusLed status)= NULL;
static int (__cdecl* _CCAPIGetFirmwareInfo) (u32* firmware, u32* ccapi, ConsoleType* cType)= NULL;
static int (__cdecl* _CCAPIVshNotify) (NotifyIcon icon, const char* msg)= NULL;
static int (__cdecl* _CCAPIGetNumberOfConsoles)()= NULL;
static void (__cdecl* _CCAPIGetConsoleInfo)(int index, ConsoleName* name, ConsoleIp* ip)= NULL;
static int (__cdecl* _CCAPIGetDllVersion) ()= NULL;

inline static void reverse(register u8* b, register u8* e)
{
    for (e--; e-b > 0;b++,e--)
    {
        register u8 t = *b;
        *b = *e;
        *e = t;
    }
}
int CCAPIInit(const char* libpath)
{
    if (LibHandle)
        return CCAPI_OK;

    LibHandle = LoadLibraryA(libpath);

    if (!LibHandle)
        return CCAPI_ERROR;

    _CCAPIConnectConsole = (int (__cdecl*)(const char*)) GetProcAddress(LibHandle,"CCAPIConnectConsole");
    _CCAPIDisconnectConsole = (int (__cdecl*)()) GetProcAddress(LibHandle,"CCAPIDisconnectConsole");
    _CCAPIGetConnectionStatus = (int (__cdecl*) (int*)) GetProcAddress(LibHandle,"CCAPIGetConnectionStatus");
    _CCAPISetBootConsoleIds = (int (__cdecl*)(ConsoleIdType, int, const ConsoleId*)) GetProcAddress(LibHandle,"CCAPISetBootConsoleIds");
    _CCAPISetConsoleIds = (int (__cdecl*)(ConsoleIdType, const ConsoleId*)) GetProcAddress(LibHandle,"CCAPISetConsoleIds");
    _CCAPISetMemory = (int (__cdecl*)(u32, u64, u32, const void*)) GetProcAddress(LibHandle,"CCAPISetMemory");
    _CCAPIGetMemory = (int (__cdecl*)(u32, u64, u32, void*)) GetProcAddress(LibHandle,"CCAPIGetMemory");
    _CCAPIGetProcessList = (int (__cdecl*)(u32*, u32*)) GetProcAddress(LibHandle,"CCAPIGetProcessList");
    _CCAPIGetProcessName = (int (__cdecl*)(u32, ProcessName*)) GetProcAddress(LibHandle,"CCAPIGetProcessName");
    _CCAPIGetTemperature = (int (__cdecl*)(int* cell, int* rsx)) GetProcAddress(LibHandle,"CCAPIGetTemperature");
    _CCAPIShutdown = (int (__cdecl*)(ShutdownMode)) GetProcAddress(LibHandle,"CCAPIShutdown");
    _CCAPIRingBuzzer = (int (__cdecl*)(BuzzerType)) GetProcAddress(LibHandle,"CCAPIRingBuzzer");
    _CCAPISetConsoleLed = (int (__cdecl*)(ColorLed, StatusLed)) GetProcAddress(LibHandle,"CCAPISetConsoleLed");
    _CCAPIGetFirmwareInfo = (int (__cdecl*)(u32*, u32*, ConsoleType*)) GetProcAddress(LibHandle,"CCAPIGetFirmwareInfo");
    _CCAPIVshNotify = (int (__cdecl*)(NotifyIcon, const char*)) GetProcAddress(LibHandle,"CCAPIVshNotify");
    _CCAPIGetNumberOfConsoles = (int (__cdecl*)()) GetProcAddress(LibHandle,"CCAPIGetNumberOfConsoles");
    _CCAPIGetConsoleInfo = (void (__cdecl*)(int, ConsoleName*, ConsoleIp*)) GetProcAddress(LibHandle,"CCAPIGetConsoleInfo");
    _CCAPIGetDllVersion = (int (__cdecl*)()) GetProcAddress(LibHandle,"CCAPIGetDllVersion");


    LibLoaded =  (_CCAPIConnectConsole != NULL)
            && (_CCAPIDisconnectConsole != NULL)
            && (_CCAPIGetConnectionStatus != NULL)
            && (_CCAPISetBootConsoleIds != NULL)
            && (_CCAPISetConsoleIds != NULL)
            && (_CCAPISetMemory != NULL)
            && (_CCAPIGetMemory != NULL)
            && (_CCAPIGetProcessList != NULL)
            && (_CCAPIGetProcessName != NULL)
            && (_CCAPIGetTemperature != NULL)
            && (_CCAPIShutdown != NULL)
            && (_CCAPIRingBuzzer != NULL)
            && (_CCAPISetConsoleLed != NULL)
            && (_CCAPIGetFirmwareInfo != NULL)
            && (_CCAPIVshNotify != NULL)
            && (_CCAPIGetNumberOfConsoles != NULL)
            && (_CCAPIGetConsoleInfo != NULL)
            && (_CCAPIGetDllVersion != NULL);

    return LibLoaded ? CCAPI_OK : CCAPI_ERROR;
}
int CCAPIEnd()
{
    LibLoaded = 0;
    return CCAPI_OK;
}
int CCAPIGetLibraryState()
{
    return LibLoaded;
}
int CCAPIConnect(const char* ip)
{
    return _CCAPIConnectConsole(ip);
}
int CCAPIDisconnect()
{
    return _CCAPIDisconnectConsole();
}
u32 CCAPIGetAttachedProcess()
{
    return ProcessId;
}
int CCAPIAttachProcess(u32 pid)
{
    ProcessId = pid;
    return CCAPI_OK;
}
int CCAPIAttachGameProcess(int* found, u32* processId)
{
    *found = 0;

    u32 ProcessIds[32];
    u32 NProcessIds = sizeof(ProcessIds)/sizeof(ProcessIds[0]);
    int ret = CCAPIGetProcessList(&NProcessIds,ProcessIds);
    if (ret != CCAPI_OK)
    {
        return ret;
    }
    else
    {
        u32 i;
        for (i = 0; i < NProcessIds ;i++)
        {
            ProcessName name;
            int ret = CCAPIGetProcessName(ProcessIds[i],&name);
            if (ret != CCAPI_OK)
            {
                return ret;
            }
            else
            {
                if (!strstr(name.value,"dev_flash"))
                {
                    if (processId)
                        *processId = ProcessIds[i];
                    ProcessId = ProcessIds[i];
                    *found = 1;
                    return ret;
                }
            }
        }
        if (processId)
            *processId = ~0;
        ProcessId = ~0;
        return ret;
    }
}
int CCAPISetBootConsoleIds(ConsoleIdType idType, const ConsoleId* id)
{
    return _CCAPISetBootConsoleIds(idType,1,id);
}
int CCAPISetBootConsoleIdsString(ConsoleIdType idType, const char* id)
{
    ConsoleId cid;
    CCAPIStringToArray(id,cid.value);
    return CCAPISetBootConsoleIds(idType,&cid);
}
int CCAPIResetBootConsoleIds(ConsoleIdType idType)
{
    return _CCAPISetBootConsoleIds(idType,0,NULL);
}
int CCAPISetConsoleIds(ConsoleIdType idType, const ConsoleId* id)
{
    return _CCAPISetConsoleIds(idType,id);
}
int CCAPISetConsoleIdsString(ConsoleIdType idType, const char* id)
{
    ConsoleId cid;
    CCAPIStringToArray(id,cid.value);
    return CCAPISetConsoleIds(idType,&cid);
}
int CCAPIWriteMemory(u64 address, u32 size, const void* data)
{
    return _CCAPISetMemory(ProcessId,address,size,data);
}
int CCAPIWriteMemoryI8(u64 address, u8 data)
{
    return CCAPIWriteMemory(address,sizeof(data),&data);
}
int CCAPIWriteMemoryI32(u64 address, u32 data)
{
#ifdef LITTLE_ENDIAN
    u8* data8 = (u8*) &data;
    reverse(data8,data8+sizeof(data));
#endif
    return CCAPIWriteMemory(address,sizeof(data),&data);
}
int CCAPIWriteMemoryF32(u64 address, f32 data)
{
#ifdef LITTLE_ENDIAN
    u8* data8 = (u8*) &data;
    reverse(data8,data8+sizeof(data));
#endif
    return CCAPIWriteMemory(address,sizeof(data),&data);
}
int CCAPIWriteMemoryI64(u64 address, u64 data)
{
#ifdef LITTLE_ENDIAN
    u8* data8 = (u8*) &data;
    reverse(data8,data8+sizeof(data));
#endif
    return CCAPIWriteMemory(address,sizeof(data),&data);
}
int CCAPIWriteMemoryF64(u64 address, f64 data)
{
#ifdef LITTLE_ENDIAN
    u8* data8 = (u8*) &data;
    reverse(data8,data8+sizeof(data));
#endif
    return CCAPIWriteMemory(address,sizeof(data),&data);
}
int CCAPIReadMemory(u64 address, u32 size, void *data)
{
    return _CCAPIGetMemory(ProcessId,address,size,data);
}
u8 CCAPIReadMemoryI8(u64 address, int* ret)
{
    u8 data;
    int r = CCAPIReadMemory(address,sizeof(data),&data);
    if (ret)
        *ret = r;
    return data;
}
u32 CCAPIReadMemoryI32(u64 address, int* ret)
{
    u32 data;
    int r = CCAPIReadMemory(address,sizeof(data),&data);
    if (ret)
        *ret = r;
#ifdef LITTLE_ENDIAN
    u8* data8 = (u8*) &data;
    reverse(data8,data8+sizeof(data));
#endif
    return data;
}
f32 CCAPIReadMemoryF32(u64 address, int* ret)
{
    f32 data;
    int r = CCAPIReadMemory(address,sizeof(data),&data);
#ifdef LITTLE_ENDIAN
    u8* data8 = (u8*) &data;
    reverse(data8,data8+sizeof(data));
#endif
    if (ret)
        *ret = r;
    return data;
}
u64 CCAPIReadMemoryI64(u64 address, int* ret)
{
    u64 data;
    int r = CCAPIReadMemory(address,sizeof(data),&data);
#ifdef LITTLE_ENDIAN
    u8* data8 = (u8*) &data;
    reverse(data8,data8+sizeof(data));
#endif
    if (ret)
        *ret = r;
    return data;
}
f64 CCAPIReadMemoryF64(u64 address, int* ret)
{
    f64 data;
    int r = CCAPIReadMemory(address,sizeof(data),&data);
#ifdef LITTLE_ENDIAN
    u8* data8 = (u8*) &data;
    reverse(data8,data8+sizeof(data));
#endif
    if (ret)
        *ret = r;
    return data;
}
int CCAPIGetProcessList(u32* npid, u32* pids)
{
    return _CCAPIGetProcessList(npid,pids);
}
int CCAPIGetProcessName(u32 pid, ProcessName* name)
{
    return _CCAPIGetProcessName(pid,name);
}
int CCAPIGetTemperature(int* cell, int* rsx)
{
    return _CCAPIGetTemperature(cell,rsx);
}
int CCAPIShutdown(ShutdownMode mode)
{
    return _CCAPIShutdown(mode);
}
int CCAPIRingBuzzer(BuzzerType type)
{
    return _CCAPIRingBuzzer(type);
}
int CCAPISetConsoleLed(ColorLed color, StatusLed status)
{
    return _CCAPISetConsoleLed(color,status);
}
int CCAPIGetVersion(u32* version)
{
    return _CCAPIGetFirmwareInfo(NULL,version,NULL);
}
int CCAPIGetFirmware(u32* firmware)
{
    return _CCAPIGetFirmwareInfo(firmware,NULL,NULL);
}
int CCAPIGetConsoleType(ConsoleType* ctype)
{
    return _CCAPIGetFirmwareInfo(NULL,NULL,ctype);
}
char* CCAPIFirmwareToString(u32 firmware, char* s, int size)
{
    u32 h = (firmware >> 24);
    u32 l = ((firmware>>12) & 0xFF);
    snprintf(s,size,"%01x.%02x",h,l);

    return s;
}
const char* CCAPIConsoleTypeToString(ConsoleType cType)
{
    const char* s = "UNK";

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
int CCAPIVshNotify(NotifyIcon icon, const char* msg)
{
    return _CCAPIVshNotify(icon,msg);
}
int CCAPIGetNumberOfConsoles()
{
    return _CCAPIGetNumberOfConsoles();
}
void CCAPIGetConsoleInfo(int index, ConsoleName* name, ConsoleIp* ip)
{
    return _CCAPIGetConsoleInfo(index, name, ip);
}
int CCAPIGetDllVersion()
{
    return _CCAPIGetDllVersion();
}
int CCAPIWriteString(u64 address, const char* str)
{
    return CCAPIWriteMemory(address,strlen(str)+1,str);
}
int CCAPIReadString(u64 address, char* str, int size)
{
    return CCAPIReadMemory(address,size,str);
}
u8* CCAPIStringToArray(const char* s, u8* id)
{
    u32 len = strlen(s);
    if (!len)
    {
        return id;
    }

    int j = 0;
    u32 i;
    for (i=0;i<(len+1);i+=2)
    {
        char b[3] = {0,0,0};
        strncpy(b,&s[i],2);
        b[1] = b[1] ? b[1]: '0';
        id[j++] = strtoul(b,NULL,16);
    }
    return id;
}
