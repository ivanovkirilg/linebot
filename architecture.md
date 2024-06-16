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

MAIN --> service
MAIN --> control
MAIN --> basic
service --> control
service --> basic
control --> basic
```
