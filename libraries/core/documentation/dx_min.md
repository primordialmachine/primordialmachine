# `dx_min`

## Signatures

```
dx_result dx_min_<suffix>(<type>* RETURN, <type> x, <type> y)
```

where `<suffix>` and `<type>` can assume the values of

```
<suffix> <type>
f32      dx_f32
f64      dx_f64
sz       dx_size
n8       dx_n8
n16      dx_n16
n32      dx_n32
n64      dx_n64
i8       dx_i8
i16      dx_i16
i32      dx_i32
i64      dx_i64
```

## Description

Computes the lesser of two values `x` and `y`.

## Error Handling
-`DX_ERROR_INVALID_ARGUMENT` `RETURN` is a null pointer
-`DX_ERROR_INVALID_ARGUMENT` `x` and/or `y` is `±∞` or `NaN`
