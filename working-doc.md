
### This is obviously not a very clean or organized document yet, and is mostly just to organize my own ideas.
(who on this earth would want to use this language anyway)


I am also going to use the same metagrammar as used in the [Lua documentation](https://www.lua.org/manual/5.4/manual.html#9) to describe syntax, because I think it's helpful and pretty too. I will use backticks (´) to enclose literal text, because I don't plan to use them in the language.

[LLVM IR reference](https://releases.llvm.org/16.0.0/docs/LangRef.html)

### Variables and types:

The type system is similar to C++, but with different syntax.

*INCOMPLETE*
#### Built-in basic types

| Size    | Integer | Unsigned integer | Float | Character |
| ------- | ------- | ------- | ------- | ------- |
| 8 bits | int8 | uint8 | N/A | char8 |
| 16 bits | int16 | uint16 | N/A | char16 |
| 32 bits | int32 | uint32 | float32 | char32 |
| 64 bits | int64 | uint64 | float64 | N/A |

bool and void also exist with their obvious C++ meanings.

*INCOMPLETE*
#### var and const

The two keywords var and const are used to create a variable. If var is used, the value of the variable can be changed later. If const is used, it can't. This is unlike most other languages, where either mutable or constant are treated as the default, with the other being a type modifier. JavaScript is the one language I can think of that defines variables in the same way.

Defining a variable then looks like this:

(´var´ | ´const´) type name ´=´ expression

Unlike in C++, I should make sure assignmets do not return a value, so you can't make the if (x=5) mistake. 
NTS: This part will need updating once pointers and references are added.

#### Functions

Functions operate in a pretty standard C++-like manner. They are defined with the following syntax:

type ([type name {´,´ type name}]) name funcbody

The first type is the return type, with the subsequent variables being parameters. 

NTS: A function body must have its final statement be a return, but is allowed to return early as well.



ok i am tired of doing this formally, ill do it later

blocks are started with do and ended with end like lua

the keywords and, or and not are used for logic instead of operators

if statements look like:

if exp do
{statement}
end

while loops look like:

while exp do
{statement}
end


for loops are not decided yet, what do i actually want? c++ for loops are just while loops with a fancy hat, and foreach/iterator loops are hard to understand


literals 

bool has
true
false

int64 has
any number without a decimal point
will find some way to do other types

float64 has
any number with a decimal point
will find some way to do other types

char8 has
single quoted char literals
c style string literals

types the when combine

classes should be there, though with only interface inheritance
c++ const methods are cool

i should probably add rust style tagged unions/enums, they solve a lot of problems at once
i will probably have to just panic if accessed wrong, at least for now
