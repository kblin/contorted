# Introduction #

The begin transmission packet seems to mark the begin of sending game data back an forth, either as
plain UDP packet or in DataSend / DataAck packets.

# Components #

## Layout ##

```
 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
+----------------------------------------------------------------+
| null padding  |     DP ID 1   |   DP ID 2     | null padding.. |
+----------------------------------------------------------------+
|..null padding |
+---------------+
```

## Legend ##

| `DP ID 1`  | A dplay ID, same as the ID acquired from the second IdRequest. |
|:-----------|:---------------------------------------------------------------|
| `DP ID 2`  | A dplay ID, usage unknown.                                     |

# Example #
Add an example here.