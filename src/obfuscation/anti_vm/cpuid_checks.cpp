/* cpuid_checks.cpp - Educational Anti-VM Detection Technique
 * 
 * DISCLAIMER: This code is for EDUCATIONAL PURPOSES ONLY.
 * Understanding anti-VM techniques helps security researchers
 * analyze malware and improve sandbox detection.
 * 
 * DO NOT use this code for malicious purposes.
 * 
 * CPUID-based VM detection uses x86 CPUID instruction to query
 * processor information. Virtualization platforms often expose
 * themselves through CPUID hypervisor leaves.
 */

#include <cstdint>
#include <cstring>

namespace upx_obfuscation {
namespace anti_vm {

// Execute CPUID instruction and return results
struct CPUIDResult {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
};

inline CPUIDResult cpuid(uint32_t function, uint32_t subfunction = 0) {
    CPUIDResult result = {0, 0, 0, 0};
    
    #if defined(_MSC_VER)
        // Microsoft Visual C++
        int info[4];
        __cpuidex(info, function, subfunction);
        result.eax = info[0];
        result.ebx = info[1];
        result.ecx = info[2];
        result.edx = info[3];
    #elif defined(__GNUC__) || defined(__clang__)
        // GCC/Clang
        __asm__ volatile(
            "cpuid"
            : "=a"(result.eax), "=b"(result.ebx), "=c"(result.ecx), "=d"(result.edx)
            : "a"(function), "c"(subfunction)
        );
    #endif
    
    return result;
}

// Technique 1: Check Hypervisor Present Bit
// CPUID leaf 0x1, ECX bit 31 indicates hypervisor presence
inline bool detect_vm_hypervisor_bit() {
    CPUIDResult result = cpuid(0x1);
    
    // Bit 31 of ECX indicates hypervisor present
    return (result.ecx & (1U << 31)) != 0;
}

// Technique 2: Check Hypervisor Vendor String
// CPUID leaf 0x40000000 returns hypervisor vendor ID
inline bool detect_vm_hypervisor_vendor() {
    // First check if hypervisor leaves are available
    CPUIDResult result = cpuid(0x40000000);
    
    // Extract vendor string from EBX, ECX, EDX
    char vendor[13] = {0};
    memcpy(&vendor[0], &result.ebx, 4);
    memcpy(&vendor[4], &result.ecx, 4);
    memcpy(&vendor[8], &result.edx, 4);
    
    // Known hypervisor vendor strings
    const char* known_vendors[] = {
        "VMwareVMware",  // VMware
        "KVMKVMKVM",     // KVM
        "Microsoft Hv",  // Hyper-V
        "XenVMMXenVMM",  // Xen
        "VBoxVBoxVBox",  // VirtualBox
        "prl hyperv",    // Parallels
        "TCGTCGTCGTCG",  // QEMU/TCG
        nullptr
    };
    
    for (int i = 0; known_vendors[i] != nullptr; i++) {
        if (strncmp(vendor, known_vendors[i], 12) == 0) {
            return true;
        }
    }
    
    return false;
}

// Technique 3: Check CPU Brand String for VM indicators
// CPUID leaves 0x80000002-0x80000004 return CPU brand string
inline bool detect_vm_cpu_brand() {
    char brand[49] = {0};
    
    // Get CPU brand string across three CPUID calls
    CPUIDResult r1 = cpuid(0x80000002);
    CPUIDResult r2 = cpuid(0x80000003);
    CPUIDResult r3 = cpuid(0x80000004);
    
    memcpy(&brand[0],  &r1, 16);
    memcpy(&brand[16], &r2, 16);
    memcpy(&brand[32], &r3, 16);
    
    // Convert to lowercase for comparison
    for (int i = 0; brand[i]; i++) {
        if (brand[i] >= 'A' && brand[i] <= 'Z') {
            brand[i] = brand[i] - 'A' + 'a';
        }
    }
    
    // Check for VM-related strings
    const char* vm_indicators[] = {
        "qemu",
        "virtual",
        "vmware",
        "xen",
        nullptr
    };
    
    for (int i = 0; vm_indicators[i] != nullptr; i++) {
        if (strstr(brand, vm_indicators[i]) != nullptr) {
            return true;
        }
    }
    
    return false;
}

// Technique 4: Check CPU Core Count
// VMs often have unusual core counts
inline bool detect_vm_core_count() {
    CPUIDResult result = cpuid(0x1);
    
    // Bits 16-23 of EBX contain logical processor count
    uint32_t logical_cores = (result.ebx >> 16) & 0xFF;
    
    // Common VM configurations: 1, 2, 4 cores
    // Physical machines more varied: 6, 8, 12, 16, etc.
    // This is a weak indicator, use with other checks
    
    return (logical_cores == 1 || logical_cores == 2);
}

// Technique 5: Check for Invalid CPUID Responses
// Some sandboxes/VMs incorrectly implement CPUID
inline bool detect_vm_invalid_cpuid() {
    // Check maximum basic CPUID function
    CPUIDResult result = cpuid(0x0);
    uint32_t max_basic = result.eax;
    
    // Real CPUs support at least leaf 0x1
    if (max_basic < 1) {
        return true; // Likely emulated
    }
    
    // Check maximum extended CPUID function
    result = cpuid(0x80000000);
    uint32_t max_extended = result.eax;
    
    // Extended functions should start at 0x80000000 or higher
    if (max_extended > 0 && max_extended < 0x80000000) {
        return true; // Invalid response, likely VM
    }
    
    return false;
}

// Technique 6: VMware-specific CPUID
// VMware implements special CPUID backdoor
inline bool detect_vmware_backdoor() {
    #if defined(__GNUC__) || defined(__clang__)
        uint32_t magic = 0x564D5868; // 'VMXh'
        uint32_t port = 0x5658;      // 'VX'
        uint32_t eax, ebx, ecx, edx;
        
        __asm__ volatile(
            "inl %%dx, %%eax"
            : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
            : "a"(magic), "c"(0x10), "d"(port)
        );
        
        // If VMware, EBX will contain magic value
        return (ebx == 0x564D5868);
    #else
        // Not implemented for MSVC (requires inline asm)
        return false;
    #endif
}

// Combined VM detection
inline bool is_virtual_machine() {
    int detection_count = 0;
    
    // Run multiple checks to reduce false positives
    if (detect_vm_hypervisor_bit()) detection_count++;
    if (detect_vm_hypervisor_vendor()) detection_count++;
    if (detect_vm_cpu_brand()) detection_count++;
    
    // Weaker indicators
    if (detect_vm_core_count()) detection_count++;
    if (detect_vm_invalid_cpuid()) detection_count++;
    
    // VMware-specific
    if (detect_vmware_backdoor()) detection_count += 2; // Strong indicator
    
    // If 2 or more checks indicate VM, likely virtual
    return (detection_count >= 2);
}

/*
 * EDUCATIONAL NOTES:
 * 
 * How CPUID-Based VM Detection Works:
 * 
 * 1. Hypervisor Present Bit:
 *    - Modern hypervisors set ECX bit 31 in CPUID leaf 1
 *    - Easy to check, hard to hide
 *    - Standard across most VMs
 * 
 * 2. Vendor String:
 *    - CPUID leaf 0x40000000 exposes hypervisor identity
 *    - VMware: "VMwareVMware"
 *    - VirtualBox: "VBoxVBoxVBox"
 *    - Hyper-V: "Microsoft Hv"
 * 
 * 3. CPU Brand String:
 *    - May contain "QEMU", "Virtual", "VMware"
 *    - Easily spotted in processor name
 * 
 * 4. Core Count:
 *    - VMs often have 1-4 cores
 *    - Physical machines more varied
 *    - Weak indicator alone
 * 
 * Countermeasures (for security researchers):
 * 
 * 1. Paravirtualization Patches:
 *    - Modify hypervisor to hide vendor strings
 *    - Clear hypervisor present bit
 *    - Fake CPU brand string
 * 
 * 2. Hardware-Assisted Virtualization:
 *    - Use nested virtualization
 *    - Pass through real CPU information
 * 
 * 3. Hypervisor Modifications:
 *    - Custom KVM/QEMU builds
 *    - Modified VirtualBox
 *    - Patched VMware
 * 
 * 4. Dynamic Binary Instrumentation:
 *    - Intercept CPUID instructions
 *    - Return fake values
 *    - Use tools like Pin, DynamoRIO
 * 
 * Real-World Usage:
 * - Malware: APT28, Carbanak, Dridex
 * - Ransomware: Some variants refuse to run in VMs
 * - Game anti-cheat: Detect debugging VMs
 * - DRM: License checking in VMs
 * 
 * Limitations:
 * - Can produce false positives
 * - Not all VMs are detectable
 * - Hardware virtualization harder to detect
 * - Can be bypassed with patches
 * 
 * Detection Tools (for defenders):
 * - Pafish: VM detection tool
 * - Al-Khaser: Anti-analysis checks
 * - YARA rules for CPUID patterns
 * 
 * References:
 * - Intel SDM Vol 2: CPUID instruction reference
 * - "Detecting Virtualization" by SANS
 * - "Red Pill" by Joanna Rutkowska (2004)
 * - VMware CPUID backdoor documentation
 */

} // namespace anti_vm
} // namespace upx_obfuscation
