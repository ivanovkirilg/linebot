# LineBot Architecture

## Top-level

```plantuml
object main

class Driver
class Controller
class UI as "UserInterface"

main --> Driver
main --> Controller
main --> UI

Controller --> Driver
UI --> Driver
```
