# LineBot Architecture

## Layer Diagram

```plantuml

rectangle Low {
    component DOMN
    component DRVR
}
rectangle Mid {
    component CTRL
    component UI
}
component MAIN

MAIN --> Mid
MAIN --> Low
Mid --> Low
```
