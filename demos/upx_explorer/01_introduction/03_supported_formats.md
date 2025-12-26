# Supported Executable Formats

## Overview

UPX supports a wide variety of executable formats across different operating systems and architectures. Understanding which formats are supported helps you use UPX effectively.

## Platform Support Matrix

| Platform | Format | Architectures | Status |
|----------|--------|---------------|--------|
| **Windows** | PE32 | x86 | ✅ Full |
| | PE32+ | x64 | ✅ Full |
| | PE32+ | ARM64 | 🚧 Limited |
| **Linux** | ELF | x86, x64 | ✅ Full |
| | ELF | ARM, ARM64 | ✅ Full |
| | ELF | MIPS, PowerPC | ✅ Full |
| **macOS** | Mach-O | x86, x64 | ✅ Full |
| | Mach-O | ARM64 (M1/M2) | ✅ Full |
| **DOS** | COM | 8086 | ✅ Full |
| | EXE | 8086, 286, 386 | ✅ Full |
| **Other** | Various | See below | Varies |

## Windows Formats

### PE32 (Portable Executable 32-bit)

#### File Types
- `.exe` - Executables
- `.dll` - Dynamic Link Libraries
- `.sys` - System drivers (limited)
- `.scr` - Screen savers
- `.cpl` - Control Panel applets

#### Architecture Support
- **x86 (32-bit)**: Full support
- **x86 with SSE**: Supported
- **x86-16**: Not supported

#### Features
- ✅ Import table compression
- ✅ Export table compression
- ✅ Resource compression
- ✅ TLS (Thread Local Storage) support
- ✅ Exception handlers
- ✅ Relocations

#### Example
```bash
# Pack Windows x86 executable
upx myapp.exe

# Pack with maximum compression
upx --best myapp.exe

# Pack DLL
upx library.dll
```

### PE32+ (Portable Executable 64-bit)

#### File Types
Same as PE32 but for 64-bit architecture

#### Architecture Support
- **x64 (AMD64)**: Full support
- **ARM64**: Limited (work in progress)

#### Features
All PE32 features plus:
- ✅ 64-bit addressing
- ✅ Large address aware
- ✅ SEH (Structured Exception Handling)

#### Example
```bash
# Pack Windows x64 executable
upx myapp64.exe

# Check if file is 64-bit
file myapp64.exe
# Output: PE32+ executable (console) x86-64
```

### Windows Subsections

#### .NET Executables
```
Status: ❌ Not Supported

Reason:
- Managed code requires .NET runtime
- IL (Intermediate Language) not compressible
- Breaks metadata tables

Alternative:
- Use .NET-specific tools (ILPack, ConfuserEx)
```

#### Signed Executables
```
Warning: ⚠️ Signature Breaks

Packing invalidates digital signatures
Must re-sign after packing:

1. Unpack with: upx -d signed.exe
2. Modify or pack
3. Re-sign with signtool
```

## Linux Formats

### ELF (Executable and Linkable Format)

#### File Types
- Executables
- Shared libraries (`.so`)
- Kernel modules (limited)

#### Architecture Support

**x86 (i386)**
```bash
upx myapp_32
```

**x86-64 (amd64)**
```bash
upx myapp_64
```

**ARM**
```bash
# ARMv5, ARMv6, ARMv7
upx myapp_arm
upx myapp_armhf
```

**ARM64 (AArch64)**
```bash
upx myapp_aarch64
```

**MIPS**
```bash
# Big-endian and little-endian
upx myapp_mips
upx myapp_mipsel
```

**PowerPC**
```bash
upx myapp_ppc
upx myapp_ppc64
```

#### Features
- ✅ PT_LOAD segment compression
- ✅ Dynamic linking support
- ✅ Position Independent Executables (PIE)
- ✅ GOT/PLT preservation
- ✅ Symbol table compression
- ⚠️ Kernel modules (experimental)

#### Example
```bash
# Pack Linux executable
upx ./myprogram

# Verify it works
./myprogram

# Check format
file myprogram
# Output: ELF 64-bit LSB executable, x86-64, packed with UPX
```

## macOS Formats

### Mach-O (Mach Object)

#### Architectures
- **x86-64**: Intel Macs
- **ARM64**: M1/M2 Apple Silicon

#### File Types
- Executables
- Dynamic libraries (`.dylib`)
- Bundles (`.bundle`)

#### Universal Binaries
```bash
# "Fat" binaries with multiple architectures
# UPX can pack each architecture separately

lipo myapp -thin x86_64 -output myapp_intel
lipo myapp -thin arm64 -output myapp_m1

upx myapp_intel
upx myapp_m1

lipo -create myapp_intel myapp_m1 -output myapp_universal
```

#### Example
```bash
# Pack macOS executable
upx ./MyApp

# Check architecture
file MyApp
# Output: Mach-O 64-bit executable arm64
```

## DOS Formats

### COM Files

Simple flat binary format

```bash
# Pack DOS COM file
upx hello.com

# Very efficient for tiny programs
```

### EXE Files

DOS MZ (Mark Zbikowski) format

```bash
# Pack DOS EXE
upx game.exe

# Supports overlays
upx --overlay=copy game.exe
```

## Other Formats

### DJGPP (DOS 32-bit Extender)
```bash
upx myprog.exe  # DJGPP executable
```

### Watcom DOS Extender
```bash
upx watcom.exe
```

### OS/2
- **Linear Executable (LX)**: Supported
- **New Executable (NE)**: Limited support

### Atari TOS
```bash
upx game.tos
```

### PlayStation 1
```bash
upx game.ps1
```

## Format Detection

UPX automatically detects file format:

```bash
$ upx program.exe
# UPX detects: PE32 executable

$ upx ./linux_app
# UPX detects: ELF 64-bit executable
```

## Unsupported Formats

### ❌ Not Supported

- **.NET assemblies** (Managed code)
- **Java JAR files** (Use ProGuard instead)
- **Python .pyc** (Use PyInstaller)
- **Windows Store apps** (APPX)
- **Android APK** (Use APK compression)
- **iOS IPA** (Use Xcode optimization)

### 🚧 Limited Support

- **Windows drivers** (`.sys`)
- **Kernel modules**
- **UEFI applications**
- **Bootloaders**

## Checking Format Support

### Identify File Format
```bash
file myprogram
```

Output examples:
```
PE32 executable (console) Intel 80386
ELF 64-bit LSB executable, x86-64
Mach-O 64-bit arm64 executable
COM executable for DOS
```

### Try Packing
```bash
upx -t myprogram
```

If supported, shows:
```
        File size         Ratio      Format      Name
   --------------------   ------   -----------   -----------
     100000 ->     40000   40.00%   linux/amd64   myprogram
```

If unsupported:
```
upx: myprogram: UnknownExecutableFormatException
```

## Platform-Specific Notes

### Windows
- Most common use case
- Excellent support
- Watch for antivirus false positives
- Re-sign after packing

### Linux
- Works with most distributions
- PIE executables supported
- May need `--force-execve` for some cases
- Preserves execution permissions

### macOS
- Gatekeeper may flag packed apps
- Code signing required for distribution
- Universal binaries need special handling
- M1/M2 native support

### Embedded Systems
- Perfect for size-constrained devices
- ARM support excellent
- Test on target hardware
- Consider startup time

## Best Practices

### Before Packing
```bash
# 1. Identify format
file myprogram

# 2. Check if packable
upx -t myprogram

# 3. Backup original
cp myprogram myprogram.bak

# 4. Pack
upx myprogram

# 5. Test
./myprogram
```

### Cross-Platform Projects
```bash
# Pack each platform's binary
upx windows/myapp.exe
upx linux/myapp
upx macos/MyApp

# Verify all work
wine windows/myapp.exe
./linux/myapp
./macos/MyApp
```

## Summary

UPX supports:
- ✅ **Windows**: PE32, PE32+ (x86, x64)
- ✅ **Linux**: ELF (x86, x64, ARM, MIPS, PowerPC)
- ✅ **macOS**: Mach-O (Intel, Apple Silicon)
- ✅ **DOS**: COM, EXE
- ✅ **Others**: Various legacy formats

**Not supported**: .NET, Java, managed code, most mobile formats

Always test packed executables on target platform before distribution!

---

**Next**: [Packing Basics Tutorial](../02_interactive_tutorials/packing_basics/tutorial.md)
