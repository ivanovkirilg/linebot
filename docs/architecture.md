# LineBot Architecture

## Layer Diagram

```plantuml

rectangle basic {
    component DOMN
    component LOGR
}

rectangle control {
    component DRVR
}

rectangle service {
    component CTRL
    component UI
}

component MAIN

MAIN --> UI: visualize\n read moves
MAIN --> CTRL: execute moves
MAIN --> DRVR: initialize
MAIN --> basic
CTRL --> DRVR: accelerate
UI --> DRVR: get position\n (to visualize)
service --> basic
control --> basic
```
