# Troubleshooting & Build Notes

A quick-reference guide for resolving build failures, environment mismatches, and runtime bugs while developing RediQL.

---

## 1. CMake Cannot Find `pybind11`

- **Symptom:** CMake outputs an error similar to:

  ```text
  Could not find a package configuration file provided by "pybind11".
  ```

- **Cause:** CMake cannot locate the `pybind11` installation because it is looking in the system-wide Python directories instead of your project-specific virtual environment.

- **Fix:** Ensure your `uv` virtual environment is explicitly active *before* creating your build directory and running CMake. CMake relies on the active environment's execution path to discover package locations.

  ```bash
  # Activate the environment
  source .venv/bin/activate

  # Clean up the failed build attempt and re-run CMake
  rm -rf build
  mkdir build && cd build
  cmake ..
  ```

---

## 2. Segfaults During Row Iteration

- **Symptom:** The program crashes with a `Segmentation fault` (core dumped) when running complex `SELECT` or `JOIN` queries.

- **Cause:** This is typically caused by referencing memory that has already been deallocated (use-after-free) or attempting an invalid type-cast inside a dynamic cell.

- **Checklist to Resolve:**

  - **Check Value Scopes:** Ensure that any pointers or references returned by the query engine aren't pointing to elements inside a temporary `Table` or `Row` object that went out of scope when a processing function returned.

  - **Pass-by-Value vs. Pass-by-Reference:** When creating temporary intermediate tables during a `JOIN` or `SELECT` pipeline, copy the underlying data vectors into the new result table rather than passing around raw pointers to volatile memory spaces.

  - **Type Validation:** Verify that a `std::variant` cell actually holds the type you are requesting before accessing it via `std::get`. Use `std::holds_alternative` or a type-safe `std::visit` structure to safely inspect dynamic fields:

    ```cpp
    if (std::holds_alternative<int>(cell_value)) {
        int id = std::get<int>(cell_value);
        // Process integer safely
    }
    ```

---

## 3. Python Module Import Errors

- **Symptom:** Running your test scripts outputs:

  ```text
  ModuleNotFoundError: No module named 'rediql'
  ```

- **Cause:** Python cannot find the compiled binary shared library object because it is sitting inside your `build/` folder rather than your Python search path.

- **Fix:** You can either append the build directory to your script's path dynamically during testing, or symlink the compiled `.so` / `.pyd` file to your `tests/` root directory.

  ```python
  # Temporary fix inside your Python test script:
  import sys
  sys.path.append("../build")
  import rediql
  ```
  