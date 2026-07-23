# 🗜️ File Compression & Decompression

<p align="center">
  <b>A Lossless File Compression System built with C++ and Qt using Huffman Coding</b>
</p>

<p align="center">
  Compress files • Reduce storage • Restore files without data loss
</p>

---

## 📌 About the Project

**File Compression & Decompression** is a desktop application developed using **C++ and Qt** that performs lossless file compression using the **Huffman Coding Algorithm**.

The application allows users to select a file, compress it into a custom `.huff` format, and later decompress it to reconstruct the original file.

It also provides compression statistics such as original size, compressed size, decompressed size, and percentage of space saved.

---

## ✨ Features

- 📂 Browse and select files using Qt File Dialog
- 🗜️ Compress files using **Huffman Coding**
- 📦 Generate compressed `.huff` files
- 🔄 Decompress `.huff` files
- 🔐 Lossless reconstruction of the original file
- 📊 Display original file size
- 📉 Display compressed file size
- 📤 Display decompressed file size
- 💾 Calculate percentage of space saved
- ⚠️ Detect when compression increases file size
- 📶 Compression/decompression progress indicator
- 🖥️ Clean and responsive Qt GUI
- 📁 Supports text, PDF, and other binary file formats

---

## 🛠️ Tech Stack

<p>
  <img src="https://img.shields.io/badge/C++-17-blue?logo=cplusplus">
  <img src="https://img.shields.io/badge/Qt-Framework-41CD52?logo=qt&logoColor=white">
  <img src="https://img.shields.io/badge/CMake-Build_System-064F8C?logo=cmake">
  <img src="https://img.shields.io/badge/Algorithm-Huffman_Coding-orange">
</p>

**Language:** C++  
**GUI Framework:** Qt  
**Build System:** CMake  
**Algorithm:** Huffman Coding  
**IDE:** Qt Creator

---

## ⚙️ How It Works

The application follows this workflow:

```text
                  ┌─────────────────┐
                  │  Original File  │
                  └────────┬────────┘
                           │
                           ▼
                  ┌─────────────────┐
                  │ Huffman Encoder │
                  └────────┬────────┘
                           │
                           ▼
                  ┌─────────────────┐
                  │   .huff File    │
                  └────────┬────────┘
                           │
                           ▼
                  ┌─────────────────┐
                  │ Huffman Decoder │
                  └────────┬────────┘
                           │
                           ▼
                  ┌─────────────────┐
                  │  Original File  │
                  └─────────────────┘
```

### Compression

1. The application reads the input file.
2. Frequency of each byte is calculated.
3. A Huffman Tree is constructed.
4. Huffman codes are generated.
5. Original data is encoded using those codes.
6. The compressed data is stored in a `.huff` file.

### Decompression

1. The `.huff` file is selected.
2. Huffman information is reconstructed.
3. Encoded bits are decoded.
4. The original file data is restored.

---

# 📸 Application Screenshots

## 🗜️ Compression Process

The user selects a file and clicks **Compress File**.

The application creates a `.huff` file and displays the compression statistics.

<p align="center">
  <img src="screenshots/Screenshot%202026-07-24%20014816.png" width="850">
</p>

---

## 🔄 Decompression Process

The compressed `.huff` file can then be selected and decompressed.

The application reconstructs the original file and displays the decompressed file size.

<p align="center">
  <img src="screenshots/Screenshot%202026-07-24%20014844.png" width="850">
</p>

---

## 📊 Compression Statistics

The application provides useful statistics after processing a file:

| Statistic | Description |
|---|---|
| **Original Size** | Size of the file before compression |
| **Compressed Size** | Size of the generated `.huff` file |
| **Decompressed Size** | Size of the reconstructed file |
| **Space Saved** | Percentage reduction achieved |
| **Size Increased** | Displayed when compression creates a larger file |

The percentage of space saved is calculated as:

```text
Space Saved (%) =
((Original Size - Compressed Size) / Original Size) × 100
```

> **Note:** Already-compressed formats such as PDFs, JPEG images, ZIP archives, etc. may achieve little compression or may slightly increase in size because additional Huffman metadata must be stored.

---

## 🧠 Huffman Coding

**Huffman Coding** is a lossless data compression algorithm.

It works by assigning:

- Shorter binary codes to frequently occurring bytes
- Longer binary codes to less frequently occurring bytes

For example:

```text
Character Frequency

A → 50
B → 25
C → 15
D → 10
```

Frequently occurring `A` can receive a shorter code than less frequently occurring `D`.

This reduces the number of bits required to represent data when the input contains useful frequency patterns.

---

## 📁 Project Structure

```text
FileCompressor/
│
├── FileCompressor/
│   ├── CMakeLists.txt
│   ├── main.cpp
│   ├── mainwindow.cpp
│   ├── mainwindow.h
│   ├── mainwindow.ui
│   ├── huffman.cpp
│   └── huffman.h
│
├── screenshots/
│   ├── Screenshot 2026-07-24 014816.png
│   └── Screenshot 2026-07-24 014844.png
│
├── .gitignore
└── README.md
```

---

## 🚀 Getting Started

### Prerequisites

Make sure you have:

- C++ compiler
- Qt Framework
- Qt Creator
- CMake

### Clone the Repository

```bash
git clone YOUR_GITHUB_REPOSITORY_URL
```

Open the cloned project folder.

### Build with Qt Creator

1. Open **Qt Creator**
2. Open `CMakeLists.txt`
3. Select a compatible Qt Kit
4. Configure the project
5. Build the project
6. Click **Run**

---

## 💡 Why Huffman Compression?

Huffman Coding is widely used for understanding important concepts such as:

- Greedy Algorithms
- Binary Trees
- Priority Queues
- Frequency Mapping
- Bit Manipulation
- File Handling
- Encoding and Decoding
- Lossless Data Compression

This project combines these concepts with a practical **C++ desktop application**.

---

## 🔮 Future Improvements

Possible extensions include:

- Real-time byte-level progress tracking
- Drag-and-drop file selection
- Folder compression
- Multiple-file compression
- Compression history
- Compression ratio visualization
- Compare Huffman with other compression algorithms
- Custom compressed-file metadata/header improvements

---

## 👩‍💻 Author

**Sakshi Phand**

Computer Engineering  
C++ • Data Structures & Algorithms • Qt

---

<p align="center">
  <b>Built using C++ • Qt • Huffman Coding</b>
</p>

<p align="center">
  ⭐ If you find this project useful, consider starring the repository!
</p>
