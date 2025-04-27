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

## Sequence Diagram

```plantuml
participant MAIN
participant CTRL
participant UI
participant DRVR

MAIN -> DRVR: initialize()
MAIN -> UI: initialize(IDriver)
MAIN -> CTRL: initialize(IDriver)

loop
  MAIN -> UI++: read_move()
  return move

  MAIN -> CTRL++: execute_move(move)
    CTRL -> CTRL: //calculate//
    CTRL -> DRVR: accelerate()
    ...
  return
end loop

```
