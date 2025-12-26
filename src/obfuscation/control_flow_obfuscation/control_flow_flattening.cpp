/* control_flow_flattening.cpp - Educational Control Flow Obfuscation
 * 
 * DISCLAIMER: This code is for EDUCATIONAL PURPOSES ONLY.
 * Understanding control flow obfuscation helps security researchers
 * analyze protected software and malware.
 * 
 * DO NOT use this code for malicious purposes.
 * 
 * Control flow flattening transforms structured code (if/else, loops)
 * into a flat switch-based dispatcher. This makes reverse engineering
 * more difficult as the program flow is less obvious.
 */

#include <cstdint>

namespace upx_obfuscation {
namespace control_flow_obfuscation {

// Example: Original straightforward function
inline int original_function(int x) {
    int result = 0;
    
    if (x > 10) {
        result = x * 2;
    } else {
        result = x + 5;
    }
    
    if (result > 20) {
        result = result - 10;
    }
    
    return result;
}

// Example: Flattened version using dispatcher pattern
inline int flattened_function(int x) {
    int result = 0;
    int state = 0; // State variable controls flow
    
    // Dispatcher loop - all control flow goes through here
    while (true) {
        switch (state) {
            case 0: // Initial state
                state = 1;
                break;
                
            case 1: // First if condition
                if (x > 10) {
                    state = 2; // Go to then branch
                } else {
                    state = 3; // Go to else branch
                }
                break;
                
            case 2: // Then branch: result = x * 2
                result = x * 2;
                state = 4; // Go to next check
                break;
                
            case 3: // Else branch: result = x + 5
                result = x + 5;
                state = 4; // Go to next check
                break;
                
            case 4: // Second if condition
                if (result > 20) {
                    state = 5; // Go to subtraction
                } else {
                    state = 6; // Go to return
                }
                break;
                
            case 5: // Subtract 10
                result = result - 10;
                state = 6; // Go to return
                break;
                
            case 6: // Return state
                return result;
                
            default:
                return -1; // Error state
        }
    }
}

// Advanced example with opaque predicates
// Opaque predicate: condition that's always true/false but hard to analyze
inline bool opaque_true(int x) {
    // Always true: (x * (x + 1)) % 2 == 0
    // Because x*(x+1) is always even (consecutive integers)
    return ((x * (x + 1)) % 2) == 0;
}

inline bool opaque_false(int x) {
    // Always false: (x^2 + x + 1) % 2 == 0 when x is odd
    // Actually this depends on x, but we can construct truly opaque ones
    // For simplicity, using: (x * x) < 0 (squares are never negative)
    return (x * x) < 0;
}

// Flattened with opaque predicates
inline int flattened_with_opaques(int x) {
    int result = 0;
    int state = 0;
    
    while (opaque_true(state)) { // Always true, but analyzer doesn't know
        switch (state) {
            case 0:
                if (opaque_true(x)) { // Always true
                    state = 1;
                } else {
                    state = 999; // Dead code, never reached
                }
                break;
                
            case 1:
                if (x > 10) {
                    state = 2;
                } else {
                    if (opaque_false(x)) { // Always false
                        state = 888; // Dead code
                    } else {
                        state = 3;
                    }
                }
                break;
                
            case 2:
                result = x * 2;
                state = 4;
                break;
                
            case 3:
                result = x + 5;
                if (opaque_true(result)) { // Always true
                    state = 4;
                }
                break;
                
            case 4:
                if (result > 20) {
                    state = 5;
                } else {
                    state = 6;
                }
                break;
                
            case 5:
                result = result - 10;
                state = 6;
                break;
                
            case 6:
                if (opaque_true(0)) { // Always true
                    return result;
                }
                break;
                
            default:
                // Dead code - makes analysis harder
                result = result ^ 0xDEADBEEF;
                state = 0;
        }
    }
    
    return result; // Unreachable but prevents compiler warnings
}

// State variable encryption
// Makes it harder to follow state transitions
inline int flattened_encrypted_state(int x) {
    int result = 0;
    int state = 0x12345678; // Encrypted initial state
    const uint32_t KEY = 0xABCDEF01;
    
    while (true) {
        // Decrypt state
        uint32_t real_state = state ^ KEY;
        
        switch (real_state) {
            case 0: // Initial
                state = 1 ^ KEY; // Encrypt next state
                break;
                
            case 1:
                if (x > 10) {
                    state = 2 ^ KEY;
                } else {
                    state = 3 ^ KEY;
                }
                break;
                
            case 2:
                result = x * 2;
                state = 4 ^ KEY;
                break;
                
            case 3:
                result = x + 5;
                state = 4 ^ KEY;
                break;
                
            case 4:
                if (result > 20) {
                    state = 5 ^ KEY;
                } else {
                    state = 6 ^ KEY;
                }
                break;
                
            case 5:
                result = result - 10;
                state = 6 ^ KEY;
                break;
                
            case 6:
                return result;
                
            default:
                return -1;
        }
    }
}

// Pseudo-random state transitions
// Uses current state to compute next state non-obviously
inline uint32_t compute_next_state(uint32_t current, uint32_t target) {
    // Simple hash function to obfuscate state transitions
    uint32_t hash = current;
    hash ^= target;
    hash = (hash << 13) | (hash >> 19);
    hash += 0x9e3779b9;
    return hash;
}

/*
 * EDUCATIONAL NOTES:
 * 
 * How Control Flow Flattening Works:
 * 
 * 1. Basic Concept:
 *    - Replace structured control flow (if/else, loops) with dispatcher
 *    - Use state variable to track position
 *    - All branches go through single switch statement
 * 
 * 2. Advantages for Obfuscation:
 *    - Harder to understand program flow
 *    - Defeats simple pattern matching
 *    - Makes decompilation produce messy code
 *    - Confuses static analysis tools
 * 
 * 3. Enhancements:
 *    - Opaque predicates: Add always-true/false conditions
 *    - Dead code: Unreachable states confuse analysis
 *    - State encryption: XOR or encrypt state variable
 *    - Random transitions: Non-obvious state changes
 * 
 * Impact on Analysis:
 * 
 * Decompilers (IDA, Ghidra) produce:
 * - Goto spaghetti code
 * - Lost high-level structure
 * - Difficult to read output
 * 
 * Example decompiled output:
 * ```c
 * state = 0;
 * while (1) {
 *   switch (state) {
 *     case 0: goto label_1;
 *     case 1: goto label_2;
 *     // ... many cases ...
 *   }
 * }
 * ```
 * 
 * Countermeasures (for security researchers):
 * 
 * 1. Pattern Recognition:
 *    - Identify dispatcher loop
 *    - Track state variable
 *    - Build state transition graph
 * 
 * 2. Symbolic Execution:
 *    - Use angr, Manticore, KLEE
 *    - Explore all paths
 *    - Reconstruct control flow
 * 
 * 3. Dynamic Analysis:
 *    - Trace execution
 *    - Record state transitions
 *    - Rebuild original flow
 * 
 * 4. Deobfuscation Tools:
 *    - Tigress deobfuscator
 *    - LLVM-based tools
 *    - Custom scripts
 * 
 * Performance Impact:
 * - Overhead: 10-50% slower execution
 * - Code size: 2-5x larger
 * - Cache: Worse locality
 * 
 * Real-World Usage:
 * - Malware: Some APT samples
 * - Software protection: Themida, VMProtect
 * - DRM: Game copy protection
 * - Obfuscators: OLLVM, Tigress
 * 
 * Limitations:
 * - Can be reversed with effort
 * - Performance penalty
 * - Code bloat
 * - Still vulnerable to dynamic analysis
 * 
 * Advanced Techniques:
 * - MBA (Mixed Boolean-Arithmetic): Obfuscate arithmetic
 * - Virtualization: Implement custom VM
 * - Instruction substitution: Replace with equivalents
 * 
 * Detection:
 * Look for:
 * - Large switch statement with many cases
 * - State variable updated in each case
 * - Unconditional loop around switch
 * - Sequential case values
 * 
 * References:
 * - "Control Flow Flattening" by Chenxi Wang (2000)
 * - "OLLVM" - Obfuscator-LLVM project
 * - "Tigress C Obfuscator" by Christian Collberg
 * - "Deobfuscation: Reverse Engineering Obfuscated Code"
 */

} // namespace control_flow_obfuscation
} // namespace upx_obfuscation
