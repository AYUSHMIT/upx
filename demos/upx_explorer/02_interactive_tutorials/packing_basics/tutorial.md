# Packing Basics Tutorial

## Introduction

Welcome to the interactive UPX packing tutorial! This guide will walk you through the fundamentals of executable packing, from basic commands to advanced options.

## Prerequisites

- Basic command-line knowledge
- A sample executable to pack
- UPX installed on your system

## Getting UPX

### Download
Visit the [official UPX releases](https://github.com/upx/upx/releases) page and download the appropriate version for your operating system.

### Verify Installation
```bash
upx --version
```

Expected output:
```
UPX 4.2.1 - Ultimate Packer for eXecutables
Copyright (C) 1996-2024 Markus Oberhumer, Laszlo Molnar & John Reiser
```

## Lesson 1: Your First Pack

### Step 1: Prepare a Test Executable

For this tutorial, we'll use a simple "Hello World" program:

```c
// hello.c
#include <stdio.h>

int main() {
    printf("Hello, UPX World!\n");
    return 0;
}
```

Compile it:
```bash
# Linux/macOS
gcc hello.c -o hello

# Windows (MinGW)
gcc hello.c -o hello.exe

# Windows (MSVC)
cl hello.c /Fe:hello.exe
```

### Step 2: Check Original Size

```bash
# Linux/macOS
ls -lh hello

# Windows
dir hello.exe
```

Example output:
```
-rwxr-xr-x  1 user  group   16K Dec 26 12:00 hello
```

### Step 3: Pack the Executable

```bash
upx hello
```

Output:
```
                       Ultimate Packer for eXecutables
                          Copyright (C) 1996 - 2024
UPX 4.2.1       Markus Oberhumer, Laszlo Molnar & John Reiser   Dec 26th 2024

        File size         Ratio      Format      Name
   --------------------   ------   -----------   -----------
     16384 ->      6144   37.50%   linux/amd64   hello

Packed 1 file.
```

### Step 4: Verify the Packed File

```bash
# Check size reduction
ls -lh hello

# Test that it still works
./hello
```

Output:
```
Hello, UPX World!
```

**Success!** You've packed your first executable! 🎉

## Lesson 2: Compression Levels

UPX offers different compression levels:

### Default Compression
```bash
upx program.exe
```
- Good balance of speed and compression
- Recommended for most use cases

### Maximum Compression (--best)
```bash
upx --best program.exe
```
- Better compression ratio
- Slower packing time
- ~5-10% smaller than default

### Ultra Compression (--brute)
```bash
upx --brute program.exe
```
- Best possible compression
- Very slow packing (can take minutes)
- ~2-5% smaller than --best
- Good for final release builds

### Fast Compression
```bash
upx --fast program.exe
```
- Faster packing
- Slightly larger output
- Good for testing

### Comparison

| Level    | Speed  | Ratio | Use Case |
|----------|--------|-------|----------|
| --fast   | ⚡⚡⚡  | 60%   | Testing |
| (default)| ⚡⚡    | 40%   | General |
| --best   | ⚡     | 35%   | Release |
| --brute  | 🐌     | 32%   | Final   |

## Lesson 3: Unpacking

UPX packing is fully reversible:

### Decompress a File
```bash
upx -d packed.exe
```

This restores the exact original file:
```
upx -d hello

                       Ultimate Packer for eXecutables
                          Copyright (C) 1996 - 2024
UPX 4.2.1       Markus Oberhumer, Laszlo Molnar & John Reiser   Dec 26th 2024

        File size         Ratio      Format      Name
   --------------------   ------   -----------   -----------
      6144 ->     16384   37.50%   linux/amd64   hello

Unpacked 1 file.
```

### Verify Integrity
```bash
# Original checksum
sha256sum hello.original

# Unpacked checksum  
sha256sum hello

# Should match!
```

## Lesson 4: Testing Without Modifying

### Test Pack (Dry Run)
```bash
upx -t program.exe
```

This tests if a file is packed correctly without modifying it.

### List Information
```bash
upx -l program.exe
```

Shows details about a packed file:
```
        File size         Ratio      Format      Name
   --------------------   ------   -----------   -----------
     16384 ->      6144   37.50%   linux/amd64   hello

Tested 1 file.
```

## Lesson 5: Backup and Safety

### Automatic Backup
```bash
upx --backup program.exe
```

Creates `program.exe~` before packing.

### Keep Original
```bash
upx program.exe -o program_packed.exe
```

Packs to a new file, preserving the original.

## Lesson 6: Advanced Options

### Preserve Metadata

#### Keep Timestamps (Linux/macOS)
```bash
upx --no-time program
```

#### Preserve File Permissions
```bash
# Already default on Linux/macOS
upx program
```

### Memory Protection

#### Allow Write Access (Linux)
```bash
upx --force-execve program
```

### Overlay Data

Some executables have data after the code:

```bash
# Preserve overlay data
upx --overlay=copy program.exe
```

## Lesson 7: Batch Processing

### Pack Multiple Files
```bash
upx *.exe
```

### Recursive Directory
```bash
find . -name "*.exe" -exec upx {} \;
```

### Parallel Packing (Linux/macOS)
```bash
find . -name "*.exe" | xargs -P 4 upx
```

## Lesson 8: Troubleshooting

### Common Issues

#### 1. "NotCompressible" Error
```
program.exe: NotCompressible
```

**Cause**: File is already compressed or too small  
**Solution**: Accept that packing won't help

#### 2. "CantPackException"
```
program.exe: CantPackException: unknown executable format
```

**Cause**: Unsupported file format  
**Solution**: Check if your format is supported

#### 3. Packed File Crashes
```bash
# Try different compression
upx --force program.exe

# Or unpack
upx -d program.exe
```

#### 4. Antivirus False Positive

**Solution**: 
- Whitelist the file
- Use `--best` (more recognizable pattern)
- Sign the packed executable

## Lesson 9: Verification

### Check if File is Packed
```bash
upx -t program.exe
```

### View Packer Info
```bash
upx -l program.exe
```

### Strings Analysis
```bash
strings program.exe | grep UPX
```

Look for:
```
UPX!
$Info: This file is packed with the UPX executable packer
```

## Lesson 10: Best Practices

### ✅ Do's

1. **Test packed executables** before distribution
2. **Keep original files** as backup
3. **Use --best for releases** to get best compression
4. **Document packing** in build scripts
5. **Check antivirus** behavior beforehand

### ❌ Don'ts

1. **Don't pack debug builds** (makes debugging impossible)
2. **Don't pack signed executables** without re-signing
3. **Don't pack DLLs used by plugins** (compatibility issues)
4. **Don't pack system files** or drivers
5. **Don't expect miracles** on already compressed files

## Summary

You've learned:

- ✅ Basic packing with UPX
- ✅ Compression levels (--fast, --best, --brute)
- ✅ Unpacking and testing
- ✅ Advanced options
- ✅ Troubleshooting common issues
- ✅ Best practices

## Next Steps

- [Explore PE/ELF Structure](../executable_structure/pe_format_explorer.html)
- [Learn Compression Algorithms](../compression_deep_dive/lzma_algorithm.html)
- [Watch Unpacking Process](../unpacking_process/decompression_trace.html)

## Interactive Exercises

Try these challenges:

1. Pack a program with all three levels and compare sizes
2. Verify unpacking produces identical file (checksums match)
3. Use `upx -l` to inspect a packed executable
4. Create a build script that automatically packs release builds

---

**Congratulations!** You now know the basics of executable packing with UPX! 🚀
