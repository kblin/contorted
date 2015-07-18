# Introduction #

There are two different ID request packets, differing in the data payload.

# Components #

## Layout ##

```
 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
+----------------------------------------------------------------+
|       ID request type.        |
+-------------------------------+
```

## Legend ##

| `ID request type`  | Identify the type of the ID request. Possible values seem to be `0x00000009` and `0x00000008`. |
|:-------------------|:-----------------------------------------------------------------------------------------------|

# Example #
Add an example here.