# 🚀 MyVimShell: The Developer's Asynchronous Terminal
> **"Because a frozen UI is a broken tool."** > A high-performance, C++/Qt6 desktop shell that brings Vim-like efficiency to system process management without ever locking up.

## 🌟 IN ACTION 
![Untitled design (1)](https://github.com/user-attachments/assets/e73a7d78-86c6-4cff-a15c-1de6c844e7bc)

## 🌟 Why I Built This (The "Problem")
Standard terminals are powerful, but building one from scratch reveals the "blocking" nightmare. Most simple shells freeze the moment you run a long command (like `ping`). I built **MyVimShell** to solve:
* **The "Hang" Problem:** Utilizing asynchronous `QProcess` handling so you can still move the window while a command runs.
* **The Navigation Friction:** Implementing a dynamic prompt that actually tracks your location in the OS.
* **The Vim Workflow:** Integrating the `-- INSERT --` feel with a status bar that responds to your actions.

---

## 🛠️ The Tech Stack

| Category | Tool | Why? |
| :--- | :--- | :--- |
| **Language** | C++17 | For low-level memory control and raw execution speed. |
| **GUI Framework** | Qt 6 | Industry-standard for cross-platform, high-performance desktop apps. |
| **Process Core** | `QProcess` | To bridge the gap between GUI code and the macOS/Unix kernel. |
| **Build System** | CMake | To manage complex dependencies and the MOC (Meta-Object Compiler). |

---

## ✨ Key Features (v1.0)

* 🎨 **Vim-Aesthetic UI:** A dark-mode terminal with a permanent status bar tracking your mode.
* 🛣️ **Smart Navigation:** A custom-built `cd` logic that updates your working directory and refreshes the UI prompt in real-time.
* 🕵️ **Event Interception:** A specialized `eventFilter` that "spies" on your keystrokes to give you **Command History** (Up/Down arrows).
* 🛡️ **The Kill Switch:** A manual "Stop" button and `Ctrl+C` shortcut that sends a `terminate()` signal to any runaway process.
* 🏗️ **Robust Error Catching:** If you type a command that doesn't exist, the shell catches the `FailedToStart` error and resets the UI instead of freezing.

---

## 🏗️ The Build Process (How it Works)
To turn this C++ code into an app, it goes through a specific pipeline:
1. **MOC (Meta-Object Compiler):** Qt scans your `MainWindow.h` for `Signals` and `Slots` and generates extra C++ code to handle the communication.
2. **Compilation:** Your code is turned into machine instructions.
3. **Linking:** CMake connects your code to the Qt6 libraries.



---

## ⌨️ Supported Commands

| Command | Type | Description |
| :--- | :--- | :--- |
| `cd <path>` | **Built-in** | Navigates the file system (updates prompt). |
| `clear` | **Built-in** | Wipes the terminal screen. |
| `exit` / `:q` | **Built-in** | Safely shuts down the app. |
| `mkdir` / `ls` | **System** | Standard file and directory management. |
| `ping` / `top` | **System** | Real-time process monitoring. |

---

## 🚀 Roadmap: What's coming in v2.0
- [ ] **Tab Auto-Completion:** Automatically finish filenames when you hit Tab.
- [ ] **Syntax Highlighting:** Color-coding commands vs. arguments.
- [ ] **Custom Mode (Normal Mode):** Press `Esc` to navigate the output with `h, j, k, l`.

---

## 💻 Local Setup

1. **Clone & Enter:**
   ```bash
   git clone https://github.com/ritiksharma/MyVimShell.git
   cd MyVimShell
   ```

2. **Configure & Build:**
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

3. **Launch:**
   ```bash
   ./MyVimShell
   ```

---
