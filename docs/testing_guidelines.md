# Test Levels

There are 3 test levels corresponding to the logical 'size' of the SUT
(System Under Test) and the amount of interaction with other 'systems'.


## Unit Tests (UT)
- SUT is an individual C++ class or module.
- SUT and the test do **not** interact with the file system, network, etc.
- Dependencies on other classes and modules are mocked.
- Test cases focus on individual operations (e.g. calculate X).

## Functional Tests (FT)
- SUT is the component or a logically coherent subset of it.
- SUT and the test may interact with the file system, network, etc.
- Dependencies on other components are mocked where practical.
- Test cases focus on individual behaviours.

## Integration Tests (IT)
- SUT is the component or a collection of components.
- SUT and the test may interact with the file system, network, etc.
- Dependencies are set up realistically.
- Test cases focus on whole functionalities.
