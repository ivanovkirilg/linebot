# File Structure

## Directory Structure

The codebase is organized in Components named by an even number of uppercase
letters which convey one or two words corresponding to the responsibility or
functionality implemented.
- Examples: LOGR for 'Logger', UI for 'User Interface.
- XMPL is used as an 'Example' Component in documentation;
  - For multiple exmaples, use XMPA, XMPB, etc. (pronounced Example-A etc.).

A Component has the following structure:
- `XMPL/`
  - `inc/` internal header files, not accessible outside the Component
  - `include/XMPL/` externally accessible header files
  - `src/` source code
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


## Namespaces

All declarations and definitions (except `main()` functions) are contained
either in an anonymous namespace, or a namespace matching the Component name.
- Further nested namespaces are allowed, but to be used sparingly.
- All declarations and definitions in an `internal.hpp` are to be contained
  in a `namespace internal` nested within the Component namespace.
