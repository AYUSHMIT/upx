/* timing_checks.cpp - Educational Anti-Debug Technique
 * 
 * DISCLAIMER: This code is for EDUCATIONAL PURPOSES ONLY.
 * Understanding anti-debugging techniques helps security researchers
 * analyze malware and develop better defensive tools.
 * 
 * DO NOT use this code for malicious purposes.
 * 
 * Timing-based debugger detection works by measuring execution time.
 * Debuggers introduce overhead that causes code to run slower.
 */

#include <chrono>
#include <thread>

namespace upx_obfuscation {
namespace anti_debug {

// Technique 1: RDTSC (Read Time-Stamp Counter) 
// Uses x86 RDTSC instruction to measure CPU cycles
inline bool detect_debugger_rdtsc() {
    unsigned long long start, end;
    
    #ifdef _MSC_VER
        // Microsoft Visual C++
        start = __rdtsc();
        // Execute some simple operations
        volatile int dummy = 0;
        for (int i = 0; i < 100; i++) {
            dummy += i;
        }
        end = __rdtsc();
    #elif defined(__GNUC__) || defined(__clang__)
        // GCC/Clang
        unsigned int lo, hi;
        asm volatile("rdtsc" : "=a"(lo), "=d"(hi));
        start = ((unsigned long long)hi << 32) | lo;
        
        // Execute some simple operations
        volatile int dummy = 0;
        for (int i = 0; i < 100; i++) {
            dummy += i;
        }
        
        asm volatile("rdtsc" : "=a"(lo), "=d"(hi));
        end = ((unsigned long long)hi << 32) | lo;
    #else
        return false; // Unsupported compiler
    #endif
    
    // If execution took too long, likely debugging
    // Threshold may need tuning based on CPU speed
    unsigned long long elapsed = end - start;
    return (elapsed > 1000); // Arbitrary threshold
}

// Technique 2: High-Resolution Timer
// Uses std::chrono for cross-platform timing checks
inline bool detect_debugger_chrono() {
    auto start = std::chrono::high_resolution_clock::now();
    
    // Simple operation that should be fast
    volatile int dummy = 0;
    for (int i = 0; i < 1000; i++) {
        dummy += i * i;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // If it took more than 100 microseconds, possibly debugging
    return (duration.count() > 100);
}

// Technique 3: Sleep-based Detection
// Debuggers may not accurately sleep for requested time
inline bool detect_debugger_sleep() {
    auto start = std::chrono::high_resolution_clock::now();
    
    // Sleep for exactly 50 milliseconds
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // If sleep time deviates significantly, possible debugging
    // Allow 10ms tolerance for system scheduling
    long long elapsed = duration.count();
    return (elapsed < 40 || elapsed > 70);
}

// Technique 4: GetTickCount Delta (Windows-specific)
#ifdef _WIN32
#include <windows.h>

inline bool detect_debugger_gettickcount() {
    DWORD start = GetTickCount();
    
    // Perform some work
    volatile int sum = 0;
    for (int i = 0; i < 10000; i++) {
        sum += i;
    }
    
    DWORD end = GetTickCount();
    DWORD elapsed = end - start;
    
    // If took longer than expected, likely debugging
    return (elapsed > 10); // milliseconds
}
#endif

// Technique 5: Exception-based Timing
// Throwing and catching exceptions is slow, especially in debugger
inline bool detect_debugger_exception_timing() {
    auto start = std::chrono::high_resolution_clock::now();
    
    try {
        // Throw and catch multiple exceptions
        for (int i = 0; i < 10; i++) {
            try {
                throw i;
            } catch (int) {
                // Caught
            }
        }
    } catch (...) {
        // Should not reach here
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // Exception handling is much slower in debuggers
    return (duration.count() > 500);
}

// Combined detection function
// Returns true if debugger is likely present
inline bool is_debugger_present_timing() {
    int detection_count = 0;
    
    // Run multiple checks (avoid false positives)
    if (detect_debugger_rdtsc()) detection_count++;
    if (detect_debugger_chrono()) detection_count++;
    if (detect_debugger_sleep()) detection_count++;
    
    #ifdef _WIN32
    if (detect_debugger_gettickcount()) detection_count++;
    #endif
    
    if (detect_debugger_exception_timing()) detection_count++;
    
    // If 2 or more checks detect anomalies, likely debugging
    return (detection_count >= 2);
}

/*
 * EDUCATIONAL NOTES:
 * 
 * Limitations of Timing-Based Detection:
 * 1. Can produce false positives on slow systems
 * 2. Can be defeated by modifying RDTSC behavior
 * 3. Anti-anti-debug plugins can normalize timing
 * 4. Virtual machines may trigger false positives
 * 
 * Countermeasures (for security researchers):
 * - Use ScyllaHide or similar plugins
 * - Patch RDTSC instructions
 * - Use hardware-assisted debugging (less overhead)
 * - Normalize timing via hypervisor
 * 
 * Real-World Usage:
 * - Common in malware (Emotet, TrickBot, etc.)
 * - Used in game anti-cheat systems
 * - Found in software protectors (Themida, VMProtect)
 * 
 * References:
 * - "Anti-Debug Tricks" by Peter Ferrie
 * - "The Art of Unpacking" by Mark Vincent Yason
 */

} // namespace anti_debug
} // namespace upx_obfuscation
