# 🚀 Project Setup with PlatformIO

This guide will walk you through installing **PlatformIO** and getting your environment ready to clone and work on this project.  

---

## 📦 Installation  

### 1. Install VS Code  
- Download and install [Visual Studio Code](https://code.visualstudio.com/).  
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
  git clone https://github.com/your-username/your-project.git
  ```  

### 6. Open the Project in VS Code  
- In VS Code, go to **File → Open Folder**.  
- Select the cloned project folder.  
- PlatformIO will automatically detect and configure the environment.  

---

## ✅ You’re Ready!  

Now you can:  
- Build the project → `PlatformIO: Build`  
- Upload firmware → `PlatformIO: Upload`  
- Monitor serial output → `PlatformIO: Serial Monitor`  