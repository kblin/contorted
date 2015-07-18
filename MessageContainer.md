# Introduction #

This message encapsulates another dplay message. Encapsulation is controled by the apporpriate flag in the
LobbyInformation packet. Not all packets are encapsulated. E.g. the server only sends ConnectionShutdown packets encapsulated.

# Components #

## Layout ##

```
 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
+----------------------------------------------------------------+
|                    container GUID                              |
+----------------------------------------------------------------+
| null padding  |container  size| null padding  |    unknown     |
+----------------------------------------------------------------+
|container  size| null padding  | encapsulated packet............|
+----------------------------------------------------------------+
```

## Legend ##

| `container size`     | Size of the encapsulated packet. |
|:---------------------|:---------------------------------|
| `unknown`            | Might be the number of encapsulated packets. Needs verification. |
| `encpsulated packet` | The encapsulated packet. Contains partial DplayHeaders, without size, ident and sockaddr\_in struct.|

# Example #
Add an example here.