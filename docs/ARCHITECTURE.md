# RediQL System Architecture

This document specifies the technical design, memory layouts, and data processing pipelines of the RediQL engine.

---

## 1. Engine Execution Paradigm

RediQL implements a single-threaded execution loop. This design choice eliminates the overhead of mutex contention, context switching, and complex thread synchronization, maximizing sequential execution throughput inside CPU caches.

---

## 2. Memory & Type System Data Layout

Data inside RediQL tables is structured as a row-oriented matrix of type-safe unions using modern C++20 structures.

* **Dynamic Cell:** Managed via `std::variant<int, double, std::string>`.
* **Row Tuple:** Managed via `std::vector<Value>`.
* **Table Store:** Managed via `std::vector<std::vector<Value>>` alongside a schema definition metadata vector.

```text
[Table Schema: Name (String), Age (Int)]
               │
               ▼
┌───────────────────────────────┐
│ Row 0: [ "Alice" ] [   28  ]  │ ──► std::vector<Value>
├───────────────────────────────┤
│ Row 1: [ "Bob"   ] [   34  ]  │
└───────────────────────────────┘
               ▲
               │
       std::vector<Row>

```

---

## 3. Storage & Persistence Layer

State mutations (inserts, updates, deletes) are streamed sequentially to an Append-Only File (`database.aof`).

* **Writes:** Hot path operations log transactions in plain text to disk before modifying the in-memory vectors.
* **Recovery:** On initialization, a bootstrap sequence reads the text log sequentially, replaying entries to rebuild the volatile memory state.
