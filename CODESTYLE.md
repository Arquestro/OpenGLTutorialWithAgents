# Code Style

- Use `.clang-format` as the single source of truth for C/C++ formatting.
- Separate includes, namespaces, and functions with a single empty line.
- Separate variable declarations from function declarations with a single empty line.
- Do not keep empty lines between code lines inside scopes (`{ ... }`).
- Indent with 4 spaces everywhere (no tabs).
- Keep class access modifiers (`public:`, `private:`, `protected:`) aligned with the `class` keyword.
- Put opening braces on a new line (no opening brace at end of line).
- Do not use trailing brace comments (for example `} // namespace`).
- Set the column width to 120 characters.
- Use integer literals for return codes (`0`, `1`) instead of `EXIT_SUCCESS`/`EXIT_FAILURE`.
- `constexpr` names must be UPPERCASE with `_`, and must not use a `k` prefix.
- Function names must use UpperCamelCase and may include digits (for example `ThisIsAFunction912`).
- Do not use relative include paths (no `./` or `../` includes); use project-root include paths.
- In implementation files, include the corresponding header first.
- Place project includes above `std`/system includes.
- Keep formatting consistent by running:
  - `find src -type f \( -name '*.cpp' -o -name '*.hpp' \) -print0 | xargs -0 clang-format -i`
  - or `clang-format -i <changed_cpp_or_hpp_files>`
