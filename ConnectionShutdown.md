# Introduction #

A connection shutdown is sent for every DPID acquired from the game host.

# Components #

## Layout ##

```
 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
+----------------------------------------------------------------+
| null padding  |      DPID     |          null padding...       | 
+----------------------------------------------------------------+
|..null padding |
+---------------+
```

## Legend ##

| `DPID`  | dplay ID to shut down |
|:--------|:----------------------|

# Example #
Add an example here.


