# Introduction #

The lobby broadcast is sent through the network to find available dplay games.
Even though I called this packet _broadcast_, the same message is sent to a specific IP address when your game allows specifying the IP address of the server.

Usally, this is an UDP packet, but if the sender doesn't get any reply after a timeout, it will send the same packet via TCP, too.

# Components #

## Layout ##

```
 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
+----------------------------------------------------------------+
|                      dplay game GUID                           |
+----------------------------------------------------------------+
|    Unknown data (ignored)     |
+-------------------------------+
```

## Legend ##

| `dplay game GUID`  | GUID of the game to connect to, can be all 0 to get all games. |
|:-------------------|:---------------------------------------------------------------|
| `ignored`          | Looks like padding, but it's set to different things in different games. However, preliminary testing doesn't show any differences in the LobbyInformation reply, no matter what these bits are set to. |

# Example #

Add example here

