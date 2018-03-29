[Click here to go the the server help.](/Documentation/SERVERHELP.md)

# Scotland Yard Help
Scotland Yard is a game based on the original Scotland Yard board game. For an impression of the classical version, please check [Wikipedia](https://en.wikipedia.org/wiki/Scotland_Yard_(board_game)). The following will give you some insight in the messages that can be sent to a game of Scotland Yard and what kind of data you can expect to get back in return.

For the latest update on the messages, please check out the [EMessages.h](/Include/ScotlandYardLobby/EMessages.h) file for Scotland Yard.

## Requests Messages

### `EMessage_SendGetMap`
This will return the map to you using `EMessage_RecvGetMap`.
```
DATA_1: ClientID(<YOUR CLIENT ID>)
DATA_2: GameID(<GAME ID>)
```

### `EMessage_SendGetSpyTravelLog`
This will return the spy's travel log using `EMessage_RecvGetSpyTravelLog`.
```
DATA_1: ClientID(<YOUR CLIENT ID>)
DATA_2: GameID(<GAME ID>)
```

### `EMessage_SendGetPlayerLocations`
This will return the locations of all players using  `EMessage_RecvGetPlayerLocations`.
```
DATA_1: ClientID(<YOUR CLIENT ID>)
DATA_2: GameID(<GAME ID>)
```

### `EMessage_SendTravel`
```
DATA_1: ClientID(<YOUR CLIENT ID>)
DATA_2: GameID(<YOUR GAME ID>)
DATA_3: uint32_t(<DESTINATION>)
```

### `EMessage_SendGetLocation`
```
DATA_1: ClientID(<YOUR CLIENT ID>)
DATA_2: GameID(<GAME ID>)
```

### `EMessage_SendGetTravelLog`
```
DATA_1: ClientID(<YOUR CLIENT ID>)
DATA_2: GameID(<GAME ID>)
```

### `EMessage_SendAmISpy`
Will return if the player is a spy or not via `EMessage_RecvAmISpy`.
```
DATA_1: ClientID(<YOUR CLIENT ID>)
DATA_2: GameID(<GAME ID>)
```

### `EMessage_SendWhoIsSpy`
Will return the client id of the spy via `EMessage_RecvWhoIsSpy`.
```
DATA_1: ClientID(<YOUR CLIENT ID>)
DATA_2: GameID(<GAME ID>)
```

### `EMessage_SendGetRemainingTokens`
```
DATA_1: ClientID(<YOUR CLIENT ID>)
DATA_2: GameID(<GAME ID>)
```

## Response Messages

### `EMessage_RecvGetMap`
```
DATA_1: RakString(<MAPFILE>)
```

### `EMessage_RecvTravelResult`
TBA

### `EMessage_RecvGetSpyTravelLog`
TBA

### `EMessage_RecvGetPlayerLocations`
TBA

### `EMessage_RecvGetLocation`
```
DATA_1: uint32_t(<LOCATION>)
```

### `EMessage_RecvAmISpy`
Will return if the player is a spy or not.
```
DATA_1: bool(<VALUE>)
```

### `EMessage_RecvWhoIsSpy`
Will return if the player is a spy or not.
```
DATA_1: unint32_t(<CLIENT ID OF SPY>)
```

### `EMessage_RecvGetTravelLog`
TBA

### `EMessage_RecvGetRemainingTokens`
TBA

## Travel Options
There are multiple ways of transportation in this game. Below is a list of all the options.

* `ETravelOption_Taxi`
* `ETravelOption_Bus`
* `ETravelOption_Underground`
* `ETravelOption_Ferry` or `ETravelOption_Black`

For more info, please check out [ETravelOption.h](/Include/ScotlandYard/ETravelOption.h).

## Travel Results
When receiving the `EMessage_RecvTravelResult` message, an `ETravelResult` sent with. Below is a list of possible results.

* `ETravelResult_NoTokensLeft`
* `ETravelResult_NoValidTravelOption`
* `ETravelResult_DestinationNotFree`
* `ETravelResult_CaughtSpy`
* `ETravelResult_Success`

For more info, please check out [ETravelResult.h](/Include/ScotlandYard/ETravelResult.h).
