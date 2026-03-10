# Code Style

- Use `.clang-format` as the single source of truth for C/C++ formatting.
- Separate includes, namespaces, and functions with a single empty line.
- Separate variable declarations from function declarations with a single empty line.
- Do not keep empty lines between code lines inside scopes (`{ ... }`).
- Indent anonymous namespace members (and all block scopes) with tabs.
- Do not use trailing brace comments (for example `} // namespace`).
- Set the column width to 120 characters.
- Use integer literals for return codes (`0`, `1`) instead of `EXIT_SUCCESS`/`EXIT_FAILURE`.
- `constexpr` names must be UPPERCASE with `_`, and must not use a `k` prefix.
- Keep formatting consistent by running:
  - `clang-format -i src/main.cpp`
  - or `clang-format -i <changed_cpp_files>`
