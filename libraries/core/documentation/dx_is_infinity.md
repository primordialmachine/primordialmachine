# `dx_sin`

## Signatures

```
dx_result dx_is_infinity_<suffix>(<type>* RETURN, <type> x)
```

where `<suffix>` and `<type>` can assume the values of

```
<suffix> <type>
f32      dx_f32
f64      dx_f64
```

## Description

Get a value is `±∞`.

## Error Handling
-`DX_ERROR_INVALID_ARGUMENT` `RETURN` is a null pointer

