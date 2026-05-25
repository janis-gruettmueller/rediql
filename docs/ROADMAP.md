# RediQL Project Roadmap & Milestones

This document bridges the long-term engineering vision for RediQL with the direct execution phases required to build the engine.

---

## 📍 High-Level Release Milestones

These macro-milestones define the core version strategy for RediQL as it evolves from a basic memory store to an optimized, integrated library.

* **v0.1: In-Memory Foundations:** Establish the runtime type safety system, dynamic relational schema validation, and database catalog management structures in RAM.
* **v0.2: Relational MVP:** Implement relational algebra operations allowing horizontal filtering, vertical column projection, and cross-table Nested Loop Joins (supporting Inner, Left, and Cross Join mechanics).
* **v0.3: Durable Engine:** Protect in-memory data against volatile process crashes by writing an append-only transaction logger and initialization recovery subsystem.
* **v0.4: Production Capable & Bound:** Expose the engine interface smoothly to Python environments and implement indexing strategies to move critical data operations to O(1) complexities.
* **v0.5: Advanced Multi-Tenancy (Optional):** Introduce MySQL-style logical isolation layers to manage multiple independent databases under a single catalog controller.

---

## 📋 Implementation Phases (GitLab Ticket Blueprint)

Use these structural phases to populate your GitLab Issues board.

### Phase 1: Core Type System & Catalogue 🎫 (Ticket #1)

* **Goal:** Establish the dynamic runtime data layout in RAM.
* [ ] **Task 1.1:** Define the `Value` cell type using `std::variant`.
* [ ] **Task 1.2:** Design the `Column` metadata struct and the `Table` class wrapping row vectors.
* [ ] **Task 1.3:** Create the central `DatabaseCatalog` instance. *Note: Design this component keeping future multi-database mapping in mind.*

### Phase 2: DDL & DML Operations 🎫 (Ticket #2)

* **Goal:** Build the system state modifiers with strict runtime type-checking.
* [ ] **Task 2.1:** Implement `create_table(name, schema)` allocation.
* [ ] **Task 2.2:** Build `insert_into(table_name, row_vector)` with type-matching checks.
* [ ] **Task 2.3:** Write vector-mutation routines for `delete_from()` and `update()`.

### Phase 3: The Relational Query Engine 🎫 (Ticket #3)

* **Goal:** Process and combine structured matrices using relational algebra.
* [ ] **Task 3.1:** Implement `select()` for horizontal row filtering and vertical column projection.
* [ ] **Task 3.2:** Build a physical **Nested Loop Join** engine capable of executing logical Inner, Left, and Cross Joins by iterating over outer and inner table vectors.

### Phase 4: Redis-Style Persistence (AOF) 🎫 (Ticket #4)

* **Goal:** Ensure durability across restarts using text streams.
* [ ] **Task 4.1:** Write a sequential append logger targeting `database.aof`.
* [ ] **Task 4.2:** Build a boot-time recovery sequencer to parse and replay the AOF log.

### Phase 5: Python Bindings & Index Optimization 🎫 (Ticket #5)

* **Goal:** Create the user interface and elevate search complexity.
* [ ] **Task 5.1:** Wrap the `DatabaseCatalog` methods via `pybind11`.
* [ ] **Task 5.2:** Integrate a primary key index (`std::unordered_map`) for O(1) lookups.
* [ ] **Task 5.3:** Refactor the join engine to support an **Index Nested Loop Join**, replacing the inner scanning loop with instant hash lookups.

### Phase 6: Stretch Goal - Multi-Database Namespace Layer 🎫 (Ticket #6) [OPTIONAL]

* **Goal:** Add MySQL-style isolation capabilities to support multiple separate databases under one runtime instance.
* [ ] **Task 6.1:** Refactor the master catalog to map strings to unique database instances (`std::unordered_map<std::string, DatabaseInstance>`).
* [ ] **Task 6.2:** Implement the `use_database(name)` state-switching method and update table creation routing to target the active namespace.
* [ ] **Task 6.3:** Update the Python bindings layer to expose namespace selection, allowing syntax like `db.use_database("analytics")`.

---

## 🚀 Future Horizons (Long-Term Backlog)

These components are slated for investigation after the core relational engine MVP is complete:

1. **Hash Joins:** Upgrade compilation to support true O(M+N) hash join algorithms using transient in-memory hash tables.
2. **Network Protocol Layer:** Wrap the binary engine inside a lightweight TCP socket server (`boost::asio`) to communicate over local networks.
3. **SIMD Vectorization:** Restructure data vectors into column-oriented stores to take advantage of parallel CPU execution flags.
