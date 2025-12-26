/* stub_generator.cpp - Educational Polymorphic Stub Generation
 * 
 * DISCLAIMER: This code is for EDUCATIONAL PURPOSES ONLY.
 * Understanding polymorphism helps security researchers analyze
 * malware and develop better detection systems.
 * 
 * DO NOT use this code for malicious purposes.
 * 
 * Polymorphic code generation creates functionally equivalent but
 * syntactically different code for each executable. This makes
 * signature-based detection more difficult.
 */

#include <random>
#include <vector>
#include <string>
#include <cstdint>

namespace upx_obfuscation {
namespace polymorphism {

// Random number generator
class PolyRNG {
    std::mt19937 gen;
public:
    PolyRNG() : gen(std::random_device{}()) {}
    
    uint32_t next() {
        return gen();
    }
    
    uint32_t range(uint32_t min, uint32_t max) {
        std::uniform_int_distribution<uint32_t> dist(min, max);
        return dist(gen);
    }
};

// x86 instruction encoder (simplified)
class X86Encoder {
public:
    std::vector<uint8_t> code;
    
    // MOV reg32, imm32 - Move immediate to register
    void mov_reg_imm32(uint8_t reg, uint32_t imm) {
        code.push_back(0xB8 + reg); // B8+r
        code.push_back(imm & 0xFF);
        code.push_back((imm >> 8) & 0xFF);
        code.push_back((imm >> 16) & 0xFF);
        code.push_back((imm >> 24) & 0xFF);
    }
    
    // NOP - No operation
    void nop() {
        code.push_back(0x90);
    }
    
    // Multi-byte NOP (more variety)
    void nop_multi(int size) {
        switch(size) {
            case 2: // 66 90 - xchg ax, ax
                code.push_back(0x66);
                code.push_back(0x90);
                break;
            case 3: // 0F 1F 00 - nop [eax]
                code.push_back(0x0F);
                code.push_back(0x1F);
                code.push_back(0x00);
                break;
            case 4: // 0F 1F 40 00 - nop [eax+0]
                code.push_back(0x0F);
                code.push_back(0x1F);
                code.push_back(0x40);
                code.push_back(0x00);
                break;
            default:
                nop();
        }
    }
    
    // PUSH reg32
    void push_reg(uint8_t reg) {
        code.push_back(0x50 + reg);
    }
    
    // POP reg32
    void pop_reg(uint8_t reg) {
        code.push_back(0x58 + reg);
    }
    
    // XOR reg, reg (clears register)
    void xor_reg_reg(uint8_t reg) {
        code.push_back(0x31); // XOR r/m32, r32
        code.push_back(0xC0 + (reg << 3) + reg); // ModR/M
    }
    
    // RET - Return
    void ret() {
        code.push_back(0xC3);
    }
};

// Polymorphic stub generator
class StubGenerator {
    PolyRNG rng;
    
public:
    // Generate random junk instruction
    void generate_junk(X86Encoder& encoder) {
        int junk_type = rng.range(0, 5);
        
        switch(junk_type) {
            case 0: // Single NOP
                encoder.nop();
                break;
                
            case 1: // Multi-byte NOP
                encoder.nop_multi(rng.range(2, 4));
                break;
                
            case 2: // PUSH + POP same register (no effect)
                {
                    uint8_t reg = rng.range(0, 7); // EAX-EDI
                    encoder.push_reg(reg);
                    encoder.pop_reg(reg);
                }
                break;
                
            case 3: // MOV reg, 0 followed by test
                {
                    uint8_t reg = rng.range(0, 7);
                    encoder.xor_reg_reg(reg); // Clear register
                }
                break;
                
            case 4: // Stack manipulation (preserves stack)
                {
                    uint8_t reg1 = rng.range(0, 7);
                    uint8_t reg2 = rng.range(0, 7);
                    if (reg1 != reg2) {
                        encoder.push_reg(reg1);
                        encoder.push_reg(reg2);
                        encoder.pop_reg(reg2);
                        encoder.pop_reg(reg1);
                    }
                }
                break;
                
            case 5: // Multiple NOPs
                for (int i = 0; i < rng.range(1, 4); i++) {
                    encoder.nop();
                }
                break;
        }
    }
    
    // Generate instruction substitution
    // Example: MOV EAX, 0 can be replaced with XOR EAX, EAX
    void generate_equivalent_zero(X86Encoder& encoder, uint8_t reg) {
        int method = rng.range(0, 3);
        
        switch(method) {
            case 0: // XOR reg, reg
                encoder.xor_reg_reg(reg);
                break;
                
            case 1: // MOV reg, 0
                encoder.mov_reg_imm32(reg, 0);
                break;
                
            case 2: // SUB reg, reg (not shown, similar to XOR)
                encoder.xor_reg_reg(reg); // Simplified
                break;
        }
    }
    
    // Generate polymorphic unpacking stub
    std::vector<uint8_t> generate_stub() {
        X86Encoder encoder;
        
        // Add random junk at the beginning
        int junk_count = rng.range(3, 10);
        for (int i = 0; i < junk_count; i++) {
            generate_junk(encoder);
        }
        
        // Real stub code would go here
        // For demo, just use some example instructions
        
        // Initialize registers with random junk
        for (uint8_t reg = 0; reg < 4; reg++) {
            if (rng.range(0, 1) == 0) {
                generate_junk(encoder);
            }
            generate_equivalent_zero(encoder, reg);
        }
        
        // More junk
        junk_count = rng.range(2, 8);
        for (int i = 0; i < junk_count; i++) {
            generate_junk(encoder);
        }
        
        // Stub would contain decompression code here
        // (not implemented in this educational example)
        
        // Final junk
        generate_junk(encoder);
        
        return encoder.code;
    }
    
    // Generate unique key for each stub
    uint32_t generate_key() {
        return rng.next();
    }
    
    // Calculate stub entropy (randomness measure)
    double calculate_entropy(const std::vector<uint8_t>& data) {
        int freq[256] = {0};
        
        // Count byte frequencies
        for (uint8_t byte : data) {
            freq[byte]++;
        }
        
        // Calculate Shannon entropy
        double entropy = 0.0;
        int size = data.size();
        
        for (int i = 0; i < 256; i++) {
            if (freq[i] > 0) {
                double p = (double)freq[i] / size;
                entropy -= p * log2(p);
            }
        }
        
        return entropy;
    }
};

/*
 * EDUCATIONAL NOTES:
 * 
 * How Polymorphism Works:
 * 1. Generate functionally equivalent but different code
 * 2. Use instruction substitution (MOV vs XOR for zeroing)
 * 3. Insert junk instructions that don't affect behavior
 * 4. Randomize register allocation
 * 5. Vary instruction order when possible
 * 
 * Detection Methods (for security researchers):
 * - Emulation-based detection (execute and compare behavior)
 * - Statistical analysis (entropy, instruction distribution)
 * - Heuristic analysis (look for common patterns)
 * - Code normalization (remove junk, normalize instructions)
 * 
 * Real-World Examples:
 * - Viruses (Win32.Expiro, Win32.Virut)
 * - Ransomware (Some variants of WannaCry)
 * - APT malware (Careto/Mask)
 * 
 * Advanced Techniques Not Shown:
 * - Register reallocation
 * - Code block reordering
 * - Opaque predicate insertion
 * - Metamorphic engines (code rewrites itself)
 * 
 * References:
 * - "Polymorphic Viruses" by Mark Ludwig
 * - "Advanced Polymorphism in Malware" by Symantec
 */

} // namespace polymorphism
} // namespace upx_obfuscation
