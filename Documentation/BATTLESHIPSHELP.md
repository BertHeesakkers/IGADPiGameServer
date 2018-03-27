[Click here to go the the server help.](/Documentation/SERVERHELP.md)

# BattleShips Help
Battleships++ is a game based on the classical BattleShips game. For the rules of the classical version, please check [Wikipedia](https://en.wikipedia.org/wiki/Battleship_(game)). For this version of the game, some rules have been changed. This text will give you some insight in the messages that can be sent to a game of BattleShips++ and what kind of data you can expect to get back in return.

Any messages described below will only be sent after receiving the `EMessage_RecvJoinGame` message.

## Request Messages
For more information, please check out [EMessages.h](/Include/BattleShipsLobby/EMessages.h) for BattleShips.

### `EMessage_SendPlaceShip`
```
DATA_1: ClientID([YOUR CLIENT ID])
DATA_2: GameID([YOUR GAME ID])
DATA_3: short([SHIP TO PLACE])
DATA_4: short([X COORDINATE ON BOARD])
DATA_5: short([Y COORDINATE ON BOARD])
DATA_6: short([DIRECTION OF SHIP])
```

### `EMessage_SendStartGame`
TBA

### `EMessage_SendStopGame`
TBA

### `EMessage_SendGetBoardData`
```
DATA_1: ClientID([YOUR CLIENT ID])
DATA_2: GameID([YOUR GAME ID])
```

### `EMessage_SendPlaceShip`
```
DATA_1: ClientID([YOUR CLIENT ID])
DATA_2: GameID([YOUR GAME ID])
```

### `EMessage_SendRemoveShip`
```
DATA_1: ClientID([YOUR CLIENT ID])
DATA_2: GameID([YOUR GAME ID])
```

### `EMessage_SendFinishedShipPlacement`
```
DATA_1: ClientID([YOUR CLIENT ID])
DATA_2: GameID([YOUR GAME ID])
```

### `EMessage_SendShoot`
```
DATA_1: ClientID([YOUR CLIENT ID])
DATA_2: GameID([YOUR GAME ID])
```

### `EMessage_SendMoveShip`
```
DATA_1: ClientID([YOUR CLIENT ID])
DATA_2: GameID([YOUR GAME ID])
```

### `EMessage_SendRotateShip`
```
DATA_1: ClientID([YOUR CLIENT ID])
DATA_2: GameID([YOUR GAME ID])
```

## Response Messages
For more information, please check out [EMessages.h](/Include/BattleShipsLobby/EMessages.h) for BattleShips.

### `EMessage_RecvGetBoardData`
TBA

### `EMessage_RecvGameStarted`
TBA

### `EMessage_RecvGameOver`
TBA

### `EMessage_RecvGameStopped`
TBA

## Error Messages
These messages are for BattleShips only. For server related error messages, please check out the [documentation file for the server](/Documentation/SERVERHELP.md).

* `EMessage_RecvGameAlreadyStarted`
* `EMessage_RecvShipPlacementNotDone`
* `EMessage_RecvShipPlacementAlreadyDone`
* `EMessage_RecvShipPlacementInvalid`
* `EMessage_RecvShipRemovalInvalid`
* `EMessage_RecvPlayedBeforeYourTurn`

For more information, please check out [EMessages.h](/Include/BattleShipsLobby/EMessages.h) for BattleShips.
