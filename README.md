# RediQL (In-Memory Relational Database Engine)

RediQL is a high-performance, single-threaded, in-memory relational database management system built from scratch in Modern C++. Inspired by the simplicity and performance of Redis, RediQL stores data entirely in volatile RAM using dynamic vectors and variants, utilizing an Append-Only File (AOF) for simple persistence and recovery.

The engine exposes a programmatic relational API (Create, Insert, Update, Delete, Select, Joins) which is bound natively to Python via `pybind11` for a modern, seamless development experience.

---

## 🛠️ Prerequisites

### 1. Toolchain Installation

* **CMake:** Required to generate native build files and compile the C++ engine.
  * *Installation:* On macOS run `brew install cmake` or on Linux run `sudo apt update && sudo apt install cmake`
* **uv:** Fast Python package installer and resolver used to manage the testing and benchmarking scripts.
  * *Installation:* On macOS/Linux run `brew install uv` or `curl -LsSf https://astral.sh/uv/install.sh | sh`
* **C++ Compiler:** A modern compiler supporting the C++20 standard (`Clang >= 13` on macOS/Xcode, or `GCC >= 11` on Linux).

### 2. Verifying Your Setup

Open your terminal and run the following commands to confirm your environment is ready:

```bash
cmake --version && uv --version
```

---

## 🚀 Getting Started & Local Setup

Follow these steps to initialize the environment and compile the project locally:

### 1. Clone the Repository

```bash
git clone [https://gitlab.com/janis-gruettmueller1/rediql.git](https://gitlab.com/janis-gruettmueller1/rediql.git)
cd rediql
```

### 2. Initialize the Python Project with `uv`

Initialize the workspace as a Python library project and add `pybind11` as a dependency. `uv` will automatically create and manage the virtual environment behind the scenes:

```bash
# Initialize the project configuration (run once)
uv init --lib

# Add pybind11 to your project dependencies
uv add pybind11
```

### 3. Compile the C++ Engine & Python Bindings

Create a build directory and use `uv run` to execute CMake. This ensures CMake automatically detects the correct Python paths and dependencies managed by `uv`:

```bash
# Create and enter the build directory
mkdir build && cd build

# Generate build files using the uv environment context
uv run cmake ..

# Compile the library
uv run cmake --build .
```

This compiles the C++ engine and generates the shared library component (`rediql.so` or `rediql.pyd`) directly into your project space.

---

## 📂 Project Structure

```text
rediql/
├── .gitignore
├── README.md               # High-level overview, prerequisites, and setup
├── LICENSE                 # MIT License
├── AGENTS.md               # AI Agent instructions & engineering philosophy
├── CMakeLists.txt          # Build system configuration
├── docs/                   # Reference Documentation
│   ├── ARCHITECTURE.md     # System design overview and data layouts
│   ├── ROADMAP.md          # High-level release milestones & implementation phases
│   ├── DESIGN_NOTES.md     # Architectural decisions, trade-offs, and design specs
│   └── TROUBLESHOOTING.md  # Common issues, build errors, and debug tips
├── src/                    # Core C++ source files
│   ├── core/               # Core Type System & Catalogue
│   │   ├── value.hpp       # Dynamic Cell (std::variant)
│   │   ├── table.hpp       # Table structure & schemas
│   │   └── database.hpp    # Master Catalogue
│   ├── engine/             # Execution Layer (SELECT, JOINS)
│   ├── index/              # Optimization Layer (Hash Indexes)
│   ├── storage/            # Persistence Layer (AOF Logging)
│   └── main.cpp            # C++ sandbox executable
├── bindings/
│   └── python_module.cpp   # pybind11 wrappers
└── tests/                  # Python verification & benchmark scripts
```
