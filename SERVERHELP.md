# RakNet Messages
The server is built on RakNet (c) and uses the best practices described in their documentation. This means that, to ensure correct endianness for both client and server, data is sent through `RakNet::BitStream` structures and does not use custom message structures.

Connecting to the server is explained in the [online documentation of RakNet (c)](http://www.jenkinssoftware.com/raknet/manual/index.html).

## EMessage
For the latest update and the actual class for it, see https://github.com/BertHeesakkers/IGADPiGameServer/blob/master/Include/Network/EMessages.h

### EMessage_SendServerHelp
Will return the server_help.txt file in a `EMessage_RecvServerHelp` message.

### EMessage_SendGameHelp
Will return the help file for the specified game in a `EMessage_RecvGameHelp` message.
```
DATA_1: EGame(<GAME>)
```
For EGame, see https://github.com/BertHeesakkers/IGADPiGameServer/blob/master/Include/Server/EGame.h

### EMessage_SendLogin
Will make you login as a user on the server.
```
DATA_1: RakString(<LOGIN>)
DATA_2: RakString(<PASSWORD>)
```

### EMessage_SendDisconnect
Will make you disconnect from the server.

### EMessage_SendJoinGame
Will make you join the specified game. Success will be indicated using a `EMessage_RecvGameJoined` message and error via the `EMessage_RecvServerError` message.
```
DATA_1: EGame(<GAME>)
```
For EGame, see https://github.com/BertHeesakkers/IGADPiGameServer/blob/master/Include/Server/EGame.h

### EMessage_SendWhoseTurnIsIt
Will return a `EMessage_RecvWhoseTurnIsIt` message with the player whose turn it is.

### EMessage_RecvServerHelp
Will return the server help file.
```
DATA_1: RakString(<HELPTEXT>)
```

### EMessage_RecvGameHelp
Will return the help file for the specified game.
```
DATA_1: RakString(<HELPTEXT>)
```

### EMessage_RecvLoginSuccess
Will indicate that logging in was a success and will also provide the client with their client id.
```
DATA_1: uint32_t(<CLIENT ID>)
```

### EMessage_RecvDisconnected
Will indicate that a player has disconnected from the current game.

### EMessage_RecvWaitingForPlayers
TBA

### EMessage_RecvGameJoined
Sent to all players in the current game to inform of a joined player.
```
DATA_1: uint32_t(<CLIENT ID>)
DATA_2: EGame(<GAME>)
```

### EMessage_RecvWhoseTurnIsIt
Will be received when asked using `EMessage_SendWhoseTurnIsIt`.
```
DATA_1: uint32_t(<CLIENT ID>)
```

### EMessage_RecvOpponentFinished
TBA

### EMessage_RecvAcknowledgement
TBA

### EMessage_RecvServerError
Will inform the client of errors.
```
DATA_1: EServerError(<ERROR>)
```

### EMessage_RecvGameNotActive
TBA