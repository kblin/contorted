# Introduction #

The lobby information packet is sent as a reply to the LobbyBroadcast packet. It contains information about the connection setup, the number of maximal allowed and current players and the game name.

Most of the packet seems to map to the DPN\_APPLICATION\_DESC struct in the dplay code.

# Components #

## Layout ##

```
 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
+----------------------------------------------------------------+
| Message size  | dplay flags   |        instance GUID           |
+----------------------------------------------------------------+
|   instance GUID cont.         |         game GUID              |
+----------------------------------------------------------------+
|      game GUID cont.          |  max players  | curr players   |
+----------------------------------------------------------------+
| unknown magic 8 bytes         | DPID          | unknown 20...  |
+----------------------------------------------------------------+
| ... bytes                                                      |
+----------------------------------------------------------------+
| Custom game name data, size is Message Size - 80               |
+----------------------------------------------------------------+
```

## Legend ##

| `message size`  | The complete size of the message. |
|:----------------|:----------------------------------|
| `dplay flags`   | dplay message flags, see below for a list of identified flags. |
| `instance GUID` | The GUID of the game instance.    |
| `game GUID`     | The GUID of the game.             |
| `max players`   | Maximum players allowed in the game. |
| `curr players`  | Number of players currently in the game. |
| `magic 8 bytes` | Usage not identified yet. Might be game specific. |
| `DPID`          | A dplay ID, usage not identified yet. |
| `20 bytes`      | Usage not identified, might be padding. |

## dplay flags ##

| `0x00000001` | Use client/server. If this flag is not set, this is a peer-to-peer session. |
|:-------------|:----------------------------------------------------------------------------|
| `0x00000002` | Unknown                                                                     |
| `0x00000004` | Migrate host. Only valid for peer-to-peer sessions.                         |
| `0x00000008` | Unknown                                                                     |
| `0x00000010` | Unknown                                                                     |
| `0x00000020` | Unknown                                                                     |
| `0x00000040` | NODPSRV. Don't use dpsrv.exe to multiplex connections for the game.         |
| `0x00000080` | Requrire password. Pasword is transmitted in plain text.                    |
| `0x00000100` | NOENUMS. Don't do enumeration queries.                                      |
| `0x00000200` | Fast sign. Signing algorithm not explaind in the SDK, haven't seen any app using this so far. |
| `0x00000400` | Full sign. SHA-1 signing. Haven't seen any app using this so far.           |
| `0x00000800` | Unknown                                                                     |
| `0x00001000` | Unknown                                                                     |
| `0x00002000` | Use a MessageContainer to encapsulate the actual packet.                    |
| `0x00004000` | Unknown                                                                     |
| `0x00008000` | Unknown                                                                     |


# Example #

Add example here
