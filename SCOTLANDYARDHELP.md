# Scotland Yard Messages
Scotland Yard is a game based on the original Scotlan Yard board game. For an impression of the classical version, please check: https://en.wikipedia.org/wiki/Scotland_Yard_(board_game). The following will give you some insight in the messages that can be sent to a game of Scotland Yard and what kind of data you can expect to get back in return.

## Client to server

### EMessage_SendGetMap
This will return the map to you using `EMessage_RecvGetMap`.
```
DATA_1: ClientID(<YOUR CLIENT ID>)
```

### EMessage_SendGetSpyTravelLog
This will return the spy's travel log using `EMessage_RecvGetSpyTravelLog`.
```
DATA_1: ClientID(<YOUR CLIENT ID>)
```

### EMessage_SendGetPlayerLocations
This will return the locations of all players using  `EMessage_RecvGetPlayerLocations`.
```
DATA_1: ClientID(<YOUR CLIENT ID>)
```

### EMessage_SendTravel
```
DATA_1: ClientID(<YOUR CLIENT ID>)
DATA_2: GameID(<YOUR GAME ID>)
DATA_3: uint32_t(<DESTINATION>)
```

### EMessage_SendGetLocation
```
DATA_1: ClientID(<YOUR CLIENT ID>)
```

### EMessage_SendGetTravelLog
```
DATA_1: ClientID(<YOUR CLIENT ID>)
```

### EMessage_SendAmISpy
Will return if the player is a spy or not via `EMessage_RecvAmISpy`.
```
DATA_1: ClientID(<YOUR CLIENT ID>)
```

### EMessage_SendGetRemainingTokens
```
DATA_1: ClientID(<YOUR CLIENT ID>)
```

## Server to client

### EMessage_RecvGetMap
```
DATA_1: RakString(<MAPFILE>)
```

### EMessage_RecvTravelResult
TBA

### EMessage_RecvGetSpyTravelLog
TBA

### EMessage_RecvGetPlayerLocations
TBA

### EMessage_RecvGetLocation
```
DATA_1: uint32_t(<LOCATION>)
```

### EMessage_RecvAmISpy
Will return if the player is a spy or not.
```
DATA_1: bool(<VALUE>)
```

### EMessage_RecvGetTravelLog
### EMessage_RecvGetRemainingTokens