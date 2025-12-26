# Compression Algorithms Explained

## Overview

UPX uses advanced compression algorithms to reduce executable size. Understanding how these algorithms work helps you choose the right one for your needs.

## Compression Algorithm Comparison

| Algorithm | Ratio | Speed | Memory | Used By |
|-----------|-------|-------|--------|---------|
| **LZMA**  | Best  | Slow  | High   | 7-Zip, UPX --lzma |
| **UCL**   | Good  | Fast  | Medium | UPX default (source) |
| **NRV**   | Best  | Medium| Low    | UPX official releases |
| **Zlib**  | Fair  | Fast  | Low    | PNG, gzip, HTTP |

## Algorithm Details

### 1. LZMA (Lempel-Ziv-Markov chain Algorithm)

#### How It Works

**Step 1: Dictionary Encoding**
- Maintains sliding window of recently seen data
- Finds repeated sequences in the input
- Replaces repeats with (position, length) references

**Step 2: Range Encoding**
- Uses probability models for compression
- Markov chain predicts next bytes
- Arithmetic coding achieves near-optimal compression

**Step 3: Filters**
- x86 filter for executable code (converts jumps/calls)
- BCJ (Branch/Call/Jump) transformation
- Improves compression of code sections

#### Example

```
Original data:
"hello world, hello universe"

Dictionary encoding finds:
"hello " appears twice
Replace second occurrence: "hello world, [ref:0,6] universe"

Range encoding:
Frequent characters use fewer bits
'h', 'e', 'l', 'o' get optimized

Result:
"hello world, hello universe" (27 bytes)
→ Compressed (≈12 bytes)
```

#### Characteristics
- **Compression**: 70-80% for executables
- **Decompression**: Very fast
- **Compression speed**: Slow (minutes for large files)
- **Memory**: 32-128 MB dictionary

#### Best For
- Release builds (one-time compression)
- Maximum size reduction
- Files > 1 MB
- Distribution binaries

### 2. UCL (Ultimate Compression Library)

#### How It Works

**Based on LZ77:**
- Sliding window compression
- Finds matching sequences
- Encodes as (distance, length) pairs

**Fast decompression:**
- Simple algorithm
- No complex arithmetic coding
- Optimized for speed

#### Example

```
Input:
"abcdefghabcdefgh"

UCL finds:
Positions 0-7: "abcdefgh"
Positions 8-15: matches 0-7

Output:
"abcdefgh" + (distance:8, length:8)

Decompression:
Read "abcdefgh", store it
See (8, 8), copy 8 bytes from 8 bytes back
```

#### Characteristics
- **Compression**: 60-70% for executables
- **Decompression**: Very fast
- **Compression speed**: Fast
- **Memory**: Low (< 1 MB)

#### Best For
- Development builds (quick iteration)
- Small files (< 1 MB)
- When speed matters
- Testing

### 3. NRV (Not Released Variants)

#### Proprietary Algorithm
- Developed by UPX authors
- Better than UCL
- Not in open source version

#### Characteristics
- **Compression**: 65-75% (between UCL and LZMA)
- **Decompression**: Very fast
- **Compression speed**: Medium
- **Memory**: Low-medium

#### Variants
- **NRV2B**: Balanced
- **NRV2D**: Better compression
- **NRV2E**: Even better compression

#### Best For
- Official UPX releases
- Production use
- Commercial distributions

### 4. Zlib (DEFLATE)

#### How It Works

**LZ77 + Huffman Coding:**
- LZ77 for dictionary compression
- Huffman coding for entropy reduction
- Two-pass compression

#### Example

```
Input:
"aaabbbcccddd"

LZ77:
"aaa" → (0, 3, 'a')
"bbb" → (0, 3, 'b')
etc.

Huffman:
'a' appears 3 times → short code: 0
'd' appears 3 times → short code: 1
Build tree for optimal encoding
```

#### Characteristics
- **Compression**: 50-60% for executables
- **Decompression**: Fast
- **Compression speed**: Fast
- **Memory**: Low

#### Best For
- Web content
- HTTP compression
- Quick compression
- Low memory environments

## Decompression Process

### LZMA Unpacking Steps

```
1. Read compressed stream
2. Initialize range decoder
3. For each byte position:
   a. Check if literal or match
   b. If literal: decode byte directly
   c. If match: copy from dictionary
4. Apply reverse BCJ filter
5. Output original data
```

### Performance Comparison

#### Compression Time (10 MB executable)

| Algorithm | Time | CPU Usage |
|-----------|------|-----------|
| LZMA      | 45s  | 100%      |
| NRV       | 8s   | 100%      |
| UCL       | 3s   | 100%      |
| Zlib      | 2s   | 100%      |

#### Decompression Time (same file)

| Algorithm | Time | CPU Usage |
|-----------|------|-----------|
| LZMA      | 80ms | 100%      |
| NRV       | 50ms | 100%      |
| UCL       | 45ms | 100%      |
| Zlib      | 60ms | 100%      |

#### Resulting Sizes

```
Original:  10,485,760 bytes (10.0 MB)

LZMA:       3,145,728 bytes (3.0 MB) - 70% reduction
NRV:        3,670,016 bytes (3.5 MB) - 65% reduction
UCL:        4,194,304 bytes (4.0 MB) - 60% reduction
Zlib:       5,242,880 bytes (5.0 MB) - 50% reduction
```

## Choosing an Algorithm

### Decision Tree

```
Need maximum compression?
├─ Yes → LZMA (--lzma)
└─ No
   ├─ Need fast compression?
   │  └─ Yes → UCL (default) or Zlib
   └─ No
      └─ Use NRV (official builds)
```

### Use Case Recommendations

#### 1. **Final Release**
```bash
upx --lzma --best program.exe
```
- Maximum compression
- Slow packing acceptable
- Users get smallest download

#### 2. **Development Build**
```bash
upx --fast program.exe
```
- Quick iteration
- Fast compression
- Slightly larger OK

#### 3. **Balanced**
```bash
upx program.exe  # Default
```
- Good compromise
- Reasonable speed
- Good compression

#### 4. **Ultra Compression**
```bash
upx --lzma --brute program.exe
```
- Best possible
- Very slow
- For critical size constraints

## Technical Details

### Dictionary Size Impact

Larger dictionary = better compression (finds more matches)

```
Dictionary:  Compression:  Time:
   64 KB        55%         Fast
  256 KB        60%         Medium
    1 MB        65%         Slow
    4 MB        68%         Very slow
```

### Match Length

Longer matches = better compression

```
Min match:   Compression:
  3 bytes       50%
  4 bytes       55%
  5 bytes       60%
```

### Entropy

High entropy = harder to compress

```
Data type:          Entropy:  Compressible:
Text                4.5       Very
Executables         6.0       Good
Compressed          7.8       Poor
Random/Encrypted    8.0       No
```

## Visualization

### LZ77 Sliding Window

```
Input: "abcabcabc"

Window:    [    ]abc abc abc
Match!          └─┬─┘
           Copy from here

Output: "abc" + (3, 3) + (3, 3)
```

### Huffman Tree

```
Frequencies: a=5, b=3, c=2, d=1

Tree:
       (11)
      /    \
    (5)    (6)
    a     /  \
        (3)  (3)
        b   /  \
          (2)  (1)
          c    d

Codes:
a = 0
b = 10
c = 110
d = 111
```

## Resources

### Interactive Visualizations
- [LZMA Algorithm Demo](../compression_deep_dive/lzma_algorithm.html)
- [Dictionary Builder](../compression_deep_dive/dictionary_builder.html)
- [Entropy Analysis](../compression_deep_dive/entropy_analysis.html)

### Further Reading
- [7-Zip LZMA SDK](https://www.7-zip.org/sdk.html)
- [UCL Documentation](http://www.oberhumer.com/opensource/ucl/)
- [DEFLATE Specification (RFC 1951)](https://www.ietf.org/rfc/rfc1951.txt)

## Summary

**Best Overall**: LZMA
- Excellent compression
- Fast decompression
- Industry standard

**Fastest**: UCL
- Quick compression
- Good for development
- Lower ratio acceptable

**Balanced**: NRV (if available)
- Good compression
- Reasonable speed
- Official UPX default

Choose based on your priorities: size vs. speed vs. time.

---

**Next**: [Supported Formats](03_supported_formats.md) | [Interactive Demo](../02_interactive_tutorials/compression_deep_dive/lzma_algorithm.html)
