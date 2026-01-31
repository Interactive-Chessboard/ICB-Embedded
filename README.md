# Project Setup with PlatformIO

This guide will walk you through installing **PlatformIO** and getting your environment ready to clone and work on this project.

---

## Installation

### 1. Install VS Code
- Download and install [Visual Studio Code](https://code.visualstudio.com/)
- Open VS Code once installed.

### 2. Install PlatformIO IDE Extension
- In VS Code, open the **Extensions Marketplace** (`Ctrl+Shift+X` or `Cmd+Shift+X` on macOS).
- Search for **PlatformIO IDE**.
- Click **Install**.

### 3. Verify PlatformIO Installation
- After installation, you’ll see the **alien-head PlatformIO icon** on the left sidebar.
- Click it to open the PlatformIO Home screen.

### 4. Install Git (if not already installed)
- Download and install [Git](https://git-scm.com/).
- Verify the installation by running in a terminal:
  ```bash
  git --version
  ```

### 5. Clone a Project
- Open a terminal in VS Code.
- Navigate to the folder where you want the project stored:
  ```bash
  cd path/to/your/workspace
  ```
- Clone the project:
  ```bash
  git clone git@github.com:Interactive-Chessboard/ICB-Embedded.git
  ```

### 6. Open the Project in VS Code
- In VS Code, go to **File → Open Folder**.
- Select the cloned project folder.
- PlatformIO will automatically detect and configure the environment.

---

## You’re Ready!

Now you can:
- Build the project:
- ```bash
  pio run -e esp32s3-wroom-1-n16r8
  ```
- Upload firmware:
- ```bash
  pio run -e esp32s3-wroom-1-n16r8 -t upload
  ```
- Monitor serial output → `PlatformIO: Serial Monitor`


## Extras
Install Markdown Preview Mermaid Support to view the design graphs in docs/

Read README.md file in test/



## Precompiled downloads
Download the firmware from the following link

// TODO

Run the following command from the firmware directory. Fill in the firware version
```bash
pio run -e esp32s3-wroom-1-n16r8 -t upload \
  --upload-port /dev/ttyUSB0 \
  --upload-file firmware-vx.x.x.bin
```
