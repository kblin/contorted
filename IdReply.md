# Introduction #

The ID reply packet hands out a dplay ID in response to an IdRequest packet.

# Components #

## Layout ##

```
 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
+----------------------------------------------------------------+
|      DPID     |     null padding...                            |
+----------------------------------------------------------------+
|               ...null padding...                               |
+----------------------------------------------------------------+
|   ...null padding             |
+-------------------------------+
```

## Legend ##

| `DPID`          | A dplay ID for the requesting client.|
|:----------------|:-------------------------------------|
| `null padding`  | 36 bytes of null padding. Efficient. |

# Example #
Add an example here.
