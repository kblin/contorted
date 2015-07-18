# Introduction #

All dplay packets share a common header. (Apart from the raw UDP data packets, of course. They don't have a header.)

A dplay header is 28 bytes in size.

Note that all numbers in dplay, apart from the exceptions mentioned below are in little endian encoding, not in network byte order aka. big endian.

# Components #

## Layout ##

```
 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
+----------------------------------------------------------------+
| size  | dpid  |   AF  |  port |      IP       |   0-padding    |
+-----------------------------------------------+----------------+
| 0-padding     |  dplay string | type  |version|                
+-----------------------------------------------+
```

## Legend ##

| `size`  | Packet size of the dplay packet |
|:--------|:--------------------------------|
| `dpid`  | An identifier bit pattern, always `0xb0fa` |
| `AF`    | Address family, from the sockaddr\_in struct |
| `port`  | Port the answer is expected on, in network byte order |
| `IP`    | IPv4 address the answer is expected on, 0.0.0.0 if the same as the sender's address |
| `0-padding` | 0-padding from the sockaddr\_in struct. `AF` through `0-padding` seem to be the sockaddr\_in struct from the port handling the answer, memcpyed into the header |
| `dplay string` | the string "play" in ASCII, not null-terminated |
| `type`  | Message type                    |
| `version` | Dplay version. `0x000b` for dplay 7, `0x000e` for dplay 9 |

# Example #

Add example here.

