## This is obviously not a very clean or organized document yet, and is mostly just for myself

[LLVM IR reference]https://releases.llvm.org/16.0.0/docs/LangRef.html

### inbuilt basic types
| Size    | Integer | Unsigned integer | Float | Character |
| ------- | ------- | ------- | ------- | ------- |
| 8 bits | _int8_ | _uint8_ | N/A | _char8_ |
| 16 bits | _int16_ | _uint16_ | N/A | _char16_|
| 32 bits | _int32_ | _uint32_ | _float32_ | _char32_ |
| 64 bits | _int64_ | _uint64_ | _float64_ | N/A |
_bool_ and _void_ also exist with their obvious C++ meanings.

### variable declaration:

(var | const) type