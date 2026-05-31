# RediQL Design Notes & Engineering Logic

This document chronicles the fundamental architectural decisions, technical trade-offs, and internal design specifications made during the development of the RediQL engine.

Rather than serving as user-facing documentation, these notes act as an engineering journal—explaining *why* the system is built the way it is. They cover the evolution of the zero-dependency runtime memory structures, the trade-offs of the single-threaded model, and the rationale behind selecting Redis-style sequential transaction logs over heavier relational storage engines.

## Architectural Decision Records

An Architectural Decision Record (ADR) documents a significant design decision made during the development of RediQL, including the context, reasoning, and trade-offs considered.

---

### ADR-001: Simple Test Suite over Assert

#### ADR-001 Decision

Replace raw `assert` calls with a lightweight custom test suite implementing `runTest`, `check`, and `printSummary` rather than adopting a third party testing framework like Catch2 or GoogleTest.

#### ADR-001 Reasoning

RediQL is zero dependency by design. While the core engine is the primary concern of that constraint, keeping the test infrastructure consistent with the project philosophy avoids introducing build complexity early. The custom suite covers the core requirements — named test cases, pass/fail reporting, failure collection, execution time, and pytest-style output — without external dependencies. A migration to Catch2 remains straightforward if the test suite outgrows this implementation.

**Gains:** zero additional dependencies, full control over output format, pytest-style output familiar to the Python side of the project, failure collection shows all failures rather than stopping at the first.

**Losses:** no test discovery — tests must be registered manually in `main.cpp`, no built-in fixtures, parameterization, or mocking, maintenance burden if the suite grows significantly in complexity.

---

### ADR-002: Cell Wrapper Struct over Free Functions

#### ADR-002 Decision

Refactor the core cell type system from free functions operating on `CellValue` to a wrapper struct `Cell` that owns a `CellValue` and exposes methods directly. This also eliminates `printCell` as a dedicated function — since `cell.string()` returns the value directly, printing becomes a natural one liner at the call site without a dedicated print function.

```cpp
// before
cellType(cell);   // returns type as string
printCell(cell);  // prints value to cout
captureOutput(cell); // redirects cout to capture value as string for testing

// after
cell.type();      // returns type as string
cell.string();    // returns value as string
std::cout << cell.string(); // printing needs no dedicated function
```

**Eliminated:** `printCell` and `captureOutput` become redundant — `cell.string()` returns the value directly making both obsolete.

#### ADR-002 Reasoning

Free functions are appropriate for generic algorithms but for a core domain type that the entire engine revolves around, methods on a struct is the more idiomatic C++ pattern. Production C++ codebases such as LLVM, RocksDB, and Arrow all wrap their core value types in structs with methods. The `Cell` wrapper also provides a clean boundary for future functionality — validation, serialization, and casting can all live naturally as methods rather than scattered free functions.

**Gains:** clean method syntax at the call site, single canonical representation of a cell throughout the engine, natural home for future cell operations without polluting the global namespace.

**Losses:** larger refactor surface, tests need rewriting to construct `Cell` instead of `CellValue` directly, slightly more indirection as `Cell` owns a `CellValue` internally.

---

### ADR-003: Unified DataType over ColumnType and CellType

#### ADR-003 Decision

Introduce a unified `DataType` enum in `src/core/types.hpp` that replaces the existing `RowType` enum in `table.hpp`. Both `Cell` and `Column` reference `DataType` as the single source of truth for what types the engine supports.

```cpp
// before
enum class ColumnType { Int64, Double, String, Bool }; // in table.hpp
cell.type() -> std::string                             // untyped string

// after
enum class DataType { Int64, Double, String, Bool };   // in types.hpp
cell.type()   -> std::optional<DataType>               // null safe, type safe
column.type   -> DataType                              // same enum
```

#### ADR-003 Reasoning

A database engine has one type system — not one per concept. Having separate `ColumnType` and a string-based cell type creates two sources of truth that can drift apart. A unified `DataType` in a dedicated `types.hpp` matches the architecture of production database engines such as DuckDB (`common/types.hpp`), Apache Arrow (`type.h`), and PostgreSQL (`catalog/pg_type.h`). As RediQL grows, `DataType` will be referenced by cells, columns, query expressions, indexes, and serializers — none of which own the concept, so it belongs in its own file.

`std::optional<DataType>` is used over throwing an exception for null cells because null is a valid, expected state in a database — not a programming error. `std::optional` forces callers to consciously handle the null case at compile time rather than risking a runtime exception. This matches how production engines like DuckDB and Arrow treat nullability — as a first class concern rather than an error condition.

**Gains:** single source of truth for the engine type system, null safe `cell.type()` returning `std::optional<DataType>`, type safe column definitions, natural extension point as new types are added, consistent with production database engine architecture.

**Losses:** additional file in the core module, `table.hpp` and `cell.hpp` both depend on `types.hpp` introducing a shared dependency, string representation of types now requires a separate `toString(DataType)` utility, callers must unwrap `std::optional` before using the type.
