# 🧭 File Explorer in C++ (Capstone Project)

A **console-based file explorer** built in **C++17** using the `std::filesystem` library.  
It allows users to navigate directories, manage files, search recursively, and handle file permissions — all within a terminal interface.

---

## 🚀 Features

| Category | Functionality |
|-----------|----------------|
| 📂 **File Navigation** | List directories, move in/out, display contents |
| 🧰 **File Operations** | Create, copy, move, rename, delete files |
| 🔍 **Search** | Recursive search for files/folders by name |
| 🔒 **Permissions** | View and change read/write/execute permissions |
| 🐳 **Docker Support** | Easily runs inside an Ubuntu Docker container |

---

## 🧑‍💻 How to Run

### 🔹 1. Build Docker Image

docker build -t file-explorer .

### 🔹 2. Run Container

docker run -it --name fileexplorer-dev -v .:/app file-explorer

### 🔹 3. Inside Container

g++ main.cpp -o explorer
./explorer

