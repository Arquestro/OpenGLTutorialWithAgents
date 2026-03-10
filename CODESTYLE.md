# Code Style

- Use `.clang-format` as the single source of truth for C/C++ formatting.
- Do not keep empty lines between code lines inside scopes (`{ ... }`).
- Indent anonymous namespace members (and all block scopes) with tabs.
- Keep formatting consistent by running:
  - `clang-format -i src/main.cpp`
  - or `clang-format -i <changed_cpp_files>`
