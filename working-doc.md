## This is obviously not a very clean or organized document yet, and is mostly just for myself

[LLVM IR reference]https://releases.llvm.org/16.0.0/docs/LangRef.html

### inbuilt basic types
| Size    | Integer | Unsigned integer | Float | Character |
| ------- | ------- | ------- | ------- | ------- | ------- |
| 8 bits  | int8    | uint8    | N/A    | char8    |
| 16 bits | int16     | uint16    | N/A    | char16    |
| 32 bits    | int32    | uint32    | float32    | char32    |
| 64 bits    | int64    | uint64    | float64    | N/A   |
bool and void also exist with their obvious C++ meanings.

### variable declaration:

(var | const) type