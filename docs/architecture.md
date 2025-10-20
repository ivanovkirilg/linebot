# Architecture

## Layers

```plantuml
@startuml
rectangle infra {
  [COMM]
  [LOGR]
  [SYNC]
}

rectangle domain {
  [DOMN]
}

rectangle functional {
  [DRVR]
}

rectangle executive {
  [CTRL]
}
executive --> functional

rectangle planning {
  [MAIN] #LightPink
}
planning --> executive

rectangle presentation {
  [UI]
}
presentation ..> planning

' Existing undesired dependencies
MAIN -up[#red]-> UI
@enduml
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
