# Server Help
The server is built on RakNet (c) and uses the best practices described in their documentation. This means that, to ensure correct endianness for both client and server, data is sent through `RakNet::BitStream` structures and does not use custom message structures.

Connecting to the server is explained in the [online documentation of RakNet (c)](http://www.jenkinssoftware.com/raknet/manual/index.html).

Any messages described below will only be sent after receiving the `ID_CONNECTION_REQUEST_ACCEPTED` message.

## Request Messages
For more info, please check out [EMessages.h](/Include/Network/EMessages.h).

### `EMessage_SendServerHelp`
Will return the server_help.txt file in a `EMessage_RecvServerHelp` message.

### `EMessage_SendGameHelp`
Will return the help file for the specified game in a `EMessage_RecvGameHelp` message.
```
DATA_1: EGame(<GAME>)
```
For more info on EGame, please check out [EGame.h](/Include/Server/EGame.h).

### `EMessage_SendLogin`
Will make you login as a user on the server.
```
DATA_1: RakString(<LOGIN>)
DATA_2: RakString(<PASSWORD>)
```

### `EMessage_SendDisconnect`
Will make you disconnect from the server.

### `EMessage_SendJoinGame`
Will make you join the specified game. Success will be indicated using a `EMessage_RecvGameJoined` message and error via the `EMessage_RecvServerError` message.
```
DATA_1: EGame(<GAME>)
```
For more info on EGame, please check out [EGame.h](/Include/Server/EGame.h).

### `EMessage_SendWhoseTurnIsIt`
Will return a `EMessage_RecvWhoseTurnIsIt` message with the player whose turn it is.
```
DATA_1: uint32_t(<CLIENT ID>)
DATA_2: GameID(<GAME ID>)
```

### `EMessage_SendLobbyData`
Will request the lobby data for the specified game which will be received using `EMessage_RecvLobbyData`. If no lobby is available, an `EServerError_GameLobbyUnavailable` error will be sent back.
```
DATA_1: EGame(<GAME>)
```
For more info on EGame, please check out [EGame.h](/Include/Server/EGame.h).

## Response Messages
For more info, please check out [EMessages.h](/Include/Network/EMessages.h).

### `EMessage_RecvServerHelp`
Will return the server help file.
```
DATA_1: RakString(<HELPTEXT>)
```

### `EMessage_RecvGameHelp`
Will return the help file for the specified game.
```
DATA_1: RakString(<HELPTEXT>)
```

### `EMessage_RecvLoginSuccess`
Will indicate that logging in was a success and will also provide the client with their client id.
```
DATA_1: uint32_t(<CLIENT ID>)
```

### `EMessage_RecvDisconnected`
Will indicate that a player has disconnected from the current game.

### `EMessage_RecvWaitingForPlayers`
Will be sent after sending `EMessage_SendJoinGame` to the server to indicate the client has been added to the queue for a game. The message will continue to be sent to all the clients to indicate new players joining and this will continue until `EMessage_RecvGameJoined` has been received.
```
DATA_1: uint32_t(<PLAYERS REQUIRED TO START GAME>)
DATA_2: uint32_t(<CURRENT AMOUNT OF PLAYERS IN QUEUE>)

DATA_3: RakString(<NAME OF PLAYER 1>)
DATA_4: uint32_t(<CLIENT ID OF PLAYER 1>)
DATA_5: RakString(<NAME OF PLAYER 2>)
DATA_6: uint32_t(<CLIENT ID OF PLAYER 2>)
DATA_7: ... (Continuing until the amount in DATA_2)
```
`DATA_3` and onwards will be all the names and client id's of all the players.

### `EMessage_RecvGameJoined`
A `EMessage_RecvWaitingForPlayers` messages will be received directly before receiving this message. The `EMessage_RecvWaitingForPlayers` will be to inform about other players in the game.

Will be sent to all players when a game has enough players to be played at it has therefore started. Will also return the id of the game.
```
DATA_1: uint32_t(<CLIENT ID>)
DATA_2: uint32_t(<GAME ID>)
```

### `EMessage_RecvWhoseTurnIsIt`
Will be received when asked using `EMessage_SendWhoseTurnIsIt`.
```
DATA_1: uint32_t(<CLIENT ID>)
```

### `EMessage_RecvOpponentFinished`
TBA

### `EMessage_RecvAcknowledgement`
TBA

### `EMessage_RecvServerError`
Will inform the client of errors.
```
DATA_1: EServerError(<ERROR>)
```

### `EMessage_RecvGameNotActive`
TBA

### `EMessage_RecvLobbyData`
```
DATA_1: EGame(<GAME>)
DATA_2: uint32_t(<PLAYERS REQUIRED TO START GAME>)
DATA_3: uint32_t(<CURRENT AMOUNT OF PLAYERS IN QUEUE>)

DATA_4: RakString(<NAME OF PLAYER 1>)
DATA_5: uint32_t(<CLIENT ID OF PLAYER 1>)
DATA_6: RakString(<NAME OF PLAYER 2>)
DATA_7: uint32_t(<CLIENT ID OF PLAYER 2>)
DATA_8: ... (Continuing until the amount in DATA_3)
```
`DATA_4` and onwards will be all the names and client id's of all the players.

## Server Errors
When receiving the `EMessage_RecvServerError` message, an `EServerError` is sent with. Below is a list of possible errors.

For more info, please check out [EServerErrors.h](/Include/Network/EServerErrors.h)
### `EServerError_NoError`
### `EServerError_GeneralError`
### `EServerError_InvalidPassword`
The wrong password has been supplied with the `EMessage_SendLogin` message.

### `EServerError_NoPassword`
No password has been supplied with the `EMessage_SendLogin` message.

### `EServerError_NotLoggedIn`
### `EServerError_UserDataUnavailable`
An invalid username (student id) has been supplied with the `EMessage_SendLogin` message.

### `EServerError_GameLobbyUnavailable`
### `EServerError_InvalidGameCommand`
### `EServerError_InvalidGameID`

## Game Messages
For game related messages, see their dedicated documentation files:
- [Battleships](/Documentation/BATTLESHIPSHELP.md)
- [Scotland Yard](/Documentation/SCOTLANDYARDHELP.md)
- [Sokoban](/Documentation/SOKOBANHELP.md)
- [Tron](/Documentation/TRONHELP.md)
