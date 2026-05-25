# AI Agent Guidelines & System Constraints

This file outlines the core architecture principles, constraints, and development guidelines for AI agents contributing to the RediQL repository. Read this entirely before writing, refactoring, or suggesting code.

---

## Core Architecture Constraints

RediQL has strict architectural rules. Do not violate these under any circumstances:

1. **Strictly Single-Threaded:** Do not introduce `std::thread`, `std::mutex`, `std::async`, or any multithreading primitives. The engine must remain purely single-threaded to maximize memory throughput and eliminate lock contention.
2. **In-Memory Row Storage:** Data tables must live entirely in RAM as vectors of variants (`std::vector<std::vector<Value>>`). Do not introduce page-based disk caching or block file structures for the active table layout.
3. **Append-Only Persistence:** State mutations must only persist via plain-text sequential logging to `database.aof`. Do not attempt to write binary formats or modify random access points on the disk.

---

## Engineering Philosophies

Adhere to standard high-performance systems programming guidelines. When generating code, prioritize these patterns:

### 1. KISS (Keep It Simple, Stupid)

* **Rule:** Prefer clear, predictable loops and basic array indexing over complex template metaprogramming or deep inheritance hierarchies.
* **Example:** Do not build abstract query plan factories if a straightforward iterator pattern or simple function call fulfills the query requirements.

### 2. YAGNI (You Aren't Gonna Need It)

* **Rule:** Only implement code that satisfies the current phase of the project blueprint.
* **Example:** Do not write optimization layers for multi-column composite indexing or B-Trees during Phase 1 or 2. Stick strictly to flat table layouts and single-key hash indexes as outlined in the roadmap.

### 3. Cache-Friendly Code

* **Rule:** Prioritize sequential memory access. Relational databases depend heavily on CPU cache locality.
* **Example:** Keep vector elements contiguous. Avoid excessive pointer-chasing through deeply nested heap allocations (`std::shared_ptr` / `std::unique_ptr`) inside core row iteration loops unless absolutely necessary for lifetime management.

---

## Implementation Preferences

* **Language Standard:** Modern C++20 or higher. Use features like `std::variant`, `std::visit`, and concepts where appropriate for type safety.
* **Error Handling:** Use explicit return codes or validation status checks for data manipulation operations rather than throwing costly C++ exceptions in hot loops.
* **Python Bindings:** Ensure all exposed C++ entry points maintain signatures that easily map to `pybind11` modules without requiring complex memory ownership translations.
