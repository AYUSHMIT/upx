/* api_hashing.cpp - Educational Import Obfuscation Technique
 * 
 * DISCLAIMER: This code is for EDUCATIONAL PURPOSES ONLY.
 * Understanding API hashing helps security researchers analyze
 * malware that hides its imported functions.
 * 
 * DO NOT use this code for malicious purposes.
 * 
 * API hashing replaces clear-text import names with hash values.
 * At runtime, the loader walks the export table and compares hashes
 * to find the desired function addresses.
 */

#include <cstdint>
#include <string>
#include <cstring>

namespace upx_obfuscation {
namespace memory_protection {

// Simple hash algorithms for API names

// DJB2 Hash - Simple and fast
inline uint32_t hash_djb2(const char* str) {
    uint32_t hash = 5381;
    int c;
    
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    
    return hash;
}

// ROT13 Hash - Rotation-based
inline uint32_t hash_rot13(const char* str) {
    uint32_t hash = 0;
    
    while (*str) {
        hash = (hash << 13) | (hash >> (32 - 13));
        hash += *str++;
    }
    
    return hash;
}

// Custom XOR Hash - Used by some malware families
inline uint32_t hash_xor(const char* str) {
    uint32_t hash = 0x12345678; // Custom seed
    
    while (*str) {
        hash ^= *str++;
        hash = (hash << 7) | (hash >> (32 - 7));
    }
    
    return hash;
}

// FNV-1a Hash - Good distribution
inline uint32_t hash_fnv1a(const char* str) {
    uint32_t hash = 0x811c9dc5; // FNV offset basis
    
    while (*str) {
        hash ^= (uint8_t)*str++;
        hash *= 0x01000193; // FNV prime
    }
    
    return hash;
}

// Common Windows API hashes (for demonstration)
struct APIHash {
    const char* name;
    uint32_t hash_djb2;
    uint32_t hash_fnv1a;
};

// Example API hashes (pre-computed)
const APIHash common_apis[] = {
    {"LoadLibraryA", 0xec0e4e8e, 0x0726774c},
    {"GetProcAddress", 0x7c0dfcaa, 0x7802f749},
    {"VirtualAlloc", 0x91afca54, 0xe553a458},
    {"VirtualProtect", 0x7946c61b, 0xe857500d},
    {"CreateProcessA", 0x863fcc79, 0x72feb316},
    {"WriteProcessMemory", 0x0b3f5a1, 0x5d83e73d},
    {nullptr, 0, 0}
};

// API Resolution by Hash (simplified example)
// In real implementation, this would walk PEB and parse PE exports
void* resolve_api_by_hash(uint32_t hash, int algorithm = 0) {
    // This is a simplified educational example
    // Real implementation would:
    // 1. Access Process Environment Block (PEB)
    // 2. Walk loaded module list
    // 3. Parse each module's export directory
    // 4. Hash each export name and compare
    // 5. Return matching function address
    
    // For educational purposes, we just demonstrate the concept
    for (int i = 0; common_apis[i].name != nullptr; i++) {
        uint32_t target_hash = (algorithm == 0) ? 
            common_apis[i].hash_djb2 : common_apis[i].hash_fnv1a;
            
        if (target_hash == hash) {
            // In real code, return actual function pointer
            // For demo, just return placeholder
            return (void*)(uintptr_t)hash;
        }
    }
    
    return nullptr;
}

// Generate hash table for all API names in a list
class APIHashTable {
    struct Entry {
        uint32_t hash;
        void* address;
    };
    
    static constexpr size_t MAX_ENTRIES = 256;
    Entry entries[MAX_ENTRIES];
    size_t count;
    
public:
    APIHashTable() : count(0) {}
    
    void add(const char* api_name) {
        if (count >= MAX_ENTRIES) return;
        
        entries[count].hash = hash_djb2(api_name);
        entries[count].address = nullptr; // Resolved at runtime
        count++;
    }
    
    void* lookup(uint32_t hash) {
        for (size_t i = 0; i < count; i++) {
            if (entries[i].hash == hash) {
                return entries[i].address;
            }
        }
        return nullptr;
    }
    
    size_t size() const { return count; }
};

/*
 * EDUCATIONAL NOTES:
 * 
 * Why Use API Hashing?
 * 1. Hide imported functions from static analysis
 * 2. Evade signature-based detection
 * 3. Make reverse engineering harder
 * 4. Reduce Import Address Table (IAT) size
 * 
 * How It Works:
 * 1. At compile time: Replace API names with hash values
 * 2. At runtime: 
 *    - Walk loaded DLL export tables
 *    - Hash each export name
 *    - Compare with target hash
 *    - Store matching function address
 * 
 * Detection Methods (for analysts):
 * - Look for PEB access and export table walking
 * - Search for hash constants in code
 * - Monitor unusual GetProcAddress-like behavior
 * - Use emulation to capture resolved APIs
 * - YARA rules for common hash algorithms
 * 
 * Real-World Usage:
 * - Malware families: Zeus, Carberp, Dridex, Emotet
 * - APT groups: APT28, APT29, Lazarus
 * - Shellcode: Metasploit payloads
 * - Packers: Some versions of Themida, VMProtect
 * 
 * Countermeasures:
 * - API monitoring tools (API Monitor, Frida)
 * - Dynamic analysis in sandbox
 * - Emulation-based unpacking
 * - Hash database lookup (recognize common hashes)
 * 
 * Advanced Variations:
 * - Unicode API hashing (for wide-char functions)
 * - Case-insensitive hashing
 * - DLL name + API name combined hashing
 * - Salted hashes (per-sample unique)
 * 
 * Example Hash Values (DJB2):
 * LoadLibraryA     = 0xec0e4e8e
 * GetProcAddress   = 0x7c0dfcaa
 * VirtualAlloc     = 0x91afca54
 * 
 * References:
 * - "Hiding Import Address Table (IAT)" by Rohitab
 * - "Malware API Call Obfuscation" by FireEye
 * - "API Hashing in Malware" by Mandiant
 */

} // namespace memory_protection
} // namespace upx_obfuscation
