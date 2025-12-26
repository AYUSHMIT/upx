# 🎯 UPX Explorer - Interactive Educational Demo

![UPX Logo](../../README)

## 🌟 Overview

Welcome to **UPX Explorer**, an interactive educational demonstration that visualizes how UPX (Ultimate Packer for eXecutables) works internally. This project makes binary packing accessible through beautiful visualizations, step-by-step tutorials, and hands-on exploration tools.

## 🎓 What You'll Learn

- **How UPX Compression Works**: Understand LZMA, UCL, and NRV algorithms
- **Executable Structure**: Explore PE, ELF, and Mach-O formats
- **Unpacking Process**: See how packed executables restore themselves at runtime
- **Binary Analysis**: Learn forensics techniques for analyzing packed files
- **Performance Trade-offs**: Compare compression ratios vs. unpacking speed

## 📂 Project Structure

```
demos/upx_explorer/
├── index.html                          # 🏠 Main interactive explorer
├── 01_introduction/                    # 📖 Educational content
├── 02_interactive_tutorials/           # 🎮 Step-by-step guides
├── 03_visualization_gallery/           # 🎨 Binary visualizations
├── 04_case_studies/                    # 💼 Real-world examples
├── 05_performance_analysis/            # 📊 Benchmarks & metrics
├── 06_advanced_topics/                 # 🔬 Deep dives
└── 07_interactive_playground/          # 🧪 Sandbox environment
```

## 🚀 Quick Start

### Option 1: Open Locally (Recommended)

```bash
# Navigate to the demos directory
cd demos/upx_explorer/

# Open index.html in your browser
# On Linux:
xdg-open index.html

# On macOS:
open index.html

# On Windows:
start index.html
```

### Option 2: Run with Local Web Server

```bash
# Using Python 3
python3 -m http.server 8000

# Using Node.js
npx http-server

# Then visit: http://localhost:8000
```

## 🎨 Features

### Interactive Visualizations

- **🔍 Hex Viewer**: Interactive binary editor with color-coded sections
- **📊 Compression Visualizer**: Watch data compress in real-time
- **🧩 PE/ELF Explorer**: Navigate executable structures interactively
- **🎯 Unpacking Simulator**: Step through the unpacking process
- **📈 Entropy Analysis**: Visualize data randomness with heatmaps

### Educational Tutorials

- **Packing Basics**: Learn the fundamentals of executable compression
- **Executable Formats**: Deep dive into PE, ELF, and Mach-O
- **Compression Algorithms**: Understand LZMA, UCL, and NRV
- **Unpacking Process**: See how stubs decompress and restore code
- **Forensics**: Detect and analyze UPX-packed files

### Case Studies

- **Malware Analysis**: Why attackers use UPX and how to detect it
- **Size Optimization**: Compress binaries for embedded systems
- **Game Modding**: Unpack and modify game executables
- **Digital Forensics**: Extract artifacts from packed files

## 🎯 Learning Paths

### 🌱 Beginner Path
1. Start with `01_introduction/00_what_is_upx.md`
2. Try `02_interactive_tutorials/packing_basics/tutorial.md`
3. Explore `02_interactive_tutorials/executable_structure/pe_format_explorer.html`
4. Watch compression in action: `02_interactive_tutorials/compression_deep_dive/lzma_algorithm.html`

### 🌿 Intermediate Path
1. Deep dive: `02_interactive_tutorials/unpacking_process/decompression_trace.html`
2. Compare algorithms: `03_visualization_gallery/binary_comparison/hex_diff_viewer.html`
3. Study cases: `04_case_studies/malware_analysis/upx_in_malware.md`
4. Analyze performance: `05_performance_analysis/compression_benchmarks/algorithm_comparison.html`

### 🌳 Advanced Path
1. Custom loaders: `06_advanced_topics/custom_loaders/linux_loader.md`
2. Multi-platform: `06_advanced_topics/multi_platform/cross_platform_packing.md`
3. Forensics toolkit: `07_interactive_playground/forensics_toolkit/signature_scanner.html`
4. Build your own: `06_advanced_topics/custom_loaders/custom_stub_dev.md`

## 🛠️ Technologies Used

- **Pure HTML5/CSS3/JavaScript**: No framework dependencies
- **Web APIs**: File API, Canvas API, Web Workers
- **Visualization**: D3.js for charts (loaded from CDN)
- **Code Highlighting**: Prism.js for syntax highlighting
- **Responsive Design**: Works on desktop, tablet, and mobile

## 📚 Educational Content

### Introduction Section
- What is UPX and why pack executables
- Compression algorithms explained (LZMA, UCL, NRV, Zlib)
- Supported executable formats (PE, ELF, Mach-O)
- Use cases and benefits

### Interactive Tutorials
- **Packing Basics**: Step-by-step compression guide
- **Executable Structure**: Explore headers, sections, imports
- **Compression Deep Dive**: Algorithm internals
- **Unpacking Process**: Stub disassembly and memory reconstruction
- **Anti-Debugging**: Detection techniques (educational only)

### Visualization Gallery
- Binary comparison tools (hex diff, entropy heatmaps)
- Execution flow graphs (CFG, call graphs)
- Memory layout visualizations
- Interactive disassembler

### Case Studies
- Malware analysis with UPX
- Size optimization for embedded systems
- Game modding techniques
- Digital forensics

### Performance Analysis
- Compression algorithm benchmarks
- Unpacking overhead measurements
- Optimization techniques

### Advanced Topics
- Custom loader development
- Multi-platform packing strategies
- Overlay data preservation
- Digital signature handling

## 🎮 Interactive Playground

Test your knowledge with hands-on tools:

- **Online Packer**: Pack files in your browser (WASM-based)
- **Algorithm Tester**: Compare compression algorithms
- **Forensics Toolkit**: Detect UPX in binary files
- **Parameter Tuning**: Optimize compression settings

## ⚠️ Educational Purpose

This demo is designed for **educational and research purposes only**. It helps students, researchers, and security professionals understand:

- How executable compression works
- Binary file formats and structures
- Reverse engineering techniques
- Malware analysis methods

## 🤝 Contributing

This is an educational project. Contributions welcome:

- Add new visualizations
- Improve tutorials
- Create additional case studies
- Fix bugs or typos
- Translate content

## 📖 Further Reading

- [UPX Official Documentation](https://github.com/upx/upx)
- [PE Format Specification](https://docs.microsoft.com/en-us/windows/win32/debug/pe-format)
- [ELF Format Specification](https://refspecs.linuxfoundation.org/elf/elf.pdf)
- [LZMA Algorithm](https://www.7-zip.org/7z.html)

## 📄 License

This educational demo follows the same license as UPX (GPL v2+). See the main UPX repository for details.

## 🙏 Acknowledgments

- **UPX Team**: For creating the amazing UPX packer
- **Community Contributors**: For feedback and improvements
- **Security Researchers**: For sharing knowledge about binary analysis

---

**Happy Exploring! 🚀**

*Learn by doing. Understand by visualizing.*
