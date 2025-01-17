# File Structure

## Directory Structure

The codebase is organized in Components named by an even number of uppercase
letters which convey one or two words corresponding to the responsibility or
functionality implemented.
- Examples: LOGR for 'Logger', UI for 'User Interface'.
- XMPL is used as an 'Example' Component in documentation;
  - For multiple examples, use XMPA, XMPB, etc. (pronounced Example-A etc.).

A Component has the following structure:
- `XMPL/`
  - `inc/` internal header files, not accessible outside the Component
  - `include/XMPL/` externally accessible header files
  - `py/` single-file scripts
    - `example_package/` python package source code
      - `test/` per-package test code
  - `src/` C++ source code
  - `test/` test code
    - `CMakeLists.txt` test targets
  - `CMakeLists.txt` non-test targets
Directories may be omitted when unnecessary.


## File Naming

C++ source and header files have PascalCase names and `.cpp`/`.hpp` extensions.
- As an exception, header files that must reside in the 'external' `include/`
  directory but actually contain implementation details are named `internal.hpp`
  and may have an associated `src/internal.cpp`.
- Test files also have a prefix denoting the type of test:
  - `UT_` unit tests
  - `FT_` functional tests
  - `IT_` integration tests
  - _See [Testing Guidelines](/docs/testing_guidelines) for more info_.

Python packages, modules, and scripts have snake_case names.


# Source Code

## C++

### Namespaces

All declarations and definitions (except `main()` functions) are contained
either in an anonymous namespace, or a namespace matching the Component name.
- Further nested namespaces are allowed, but to be used sparingly.
- All declarations and definitions in an `internal.hpp` are to be contained
  in a `namespace internal` nested within the Component namespace.


### Identifiers

**Classes** are named in PascalCase and are generally contained in a file
of the same name, though small helper classes don't need separate files.

**Structs** are used for defining collections of data, also PascalCase.

**Enum classes** are named in PascalCase, their members in UPPER_CASE.
C-style enums are **not** used.

**Functions** (free-standing or members) are named in camelCase.
- Generally, use a verb or an imperative clause (e.g. `doSomething()`);
- Getters may be named as a noun (e.g. `.size()`);

**Variables** (locals and **members**) are camelCase noun phrases.
- Private member variables are prefixed with 'm_'.

**Constants** are named in UPPER_CASE when in namespace scope or static members.
- Local scope constants are named like variables.


## Python

Follow official Python naming guidelines.
