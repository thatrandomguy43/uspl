
### This is obviously not a very clean or organized document yet, and is mostly just to organize my own ideas.
(who on this earth would want to use this language anyway)


I am also going to use the same metagrammar as used in the [Lua documentation](https://www.lua.org/manual/5.4/manual.html#9) to describe syntax, because I think it's helpful and pretty too. I will use backticks (´) to enclose literal text, because I don't plan to use them in the language.

[LLVM IR reference](https://releases.llvm.org/16.0.0/docs/LangRef.html)

### Variables and types:

The type system is similar to C++, but with different syntax.


#### Built-in basic types

| Size    | Integer | Unsigned integer | Float   | Character |
| ------- | ------- | -------          | ------- | -------   |
| 8 bits  | int8    | uint8            | N/A     | char      |
| 16 bits | int16   | uint16           | N/A     | N/A       |
| 32 bits | int32   | uint32           | float32 | N/A       |
| 64 bits | int64   | uint64           | float64 | N/A       |

bool and void also exist with their obvious C++ meanings.
UPDATE: scrapped the wide chars, they add extra text encoding problems and make dealing with strings in the compiler more difficult
i'm just gonna add utf8 shit in the std::string clone i'll make (if this project isn't dead by then ofc)


#### var and const

The two keywords var and const are used to create a variable. If var is used, the value of the variable can be changed later. If const is used, it can't. This is unlike most other languages, where either mutable or constant are treated as the default, with the other being a type modifier. JavaScript is the one language I can think of that defines variables in the same way.

Defining a variable then looks like this:

(`var` | `const`) type name `=` expression

Unlike in C++, I should make sure assignmets do not return a value, so you can't make the if (x=5) mistake. 
NTS: This part will need updating once pointers and references are added.

#### Functions

Functions operate in a pretty standard C++-like manner. They are defined with the following syntax:

type ([type name {`,` type name}]) name `=` funcbody

The first type is the return type, with the subsequent variables being parameters. 

NTS: A function body must have its final statement be a return, but is allowed to return early as well.



ok i am tired of doing this formally, ill do it later

blocks are started with do and ended with end like lua

the keywords and, or and not are used for boolean logic instead of operators

i think i'm gonna have to do oops all parentheses for binary operators so 3 * 4 + 2 must be (3 * 4) + 2
not doing that would mean i have to deal with operator presidence and introduce behaviour that is hard to program and easy to misunderstand 


if statements look like:

if exp do
{statement}
end

while loops look like:

while exp do
{statement}
end


for loops are not decided yet, what do i actually want? c++ for loops are just while loops with a fancy hat, and foreach/iterator loops are hard to understand



i want to avoid type inference, because it makes my job harder and creates WTFs like c++ overload resolution

i should then not have to worry about literal typing, because you can't deduce a type from them

classes should be there, though with only interface inheritance
c++ const methods are cool

i should probably add rust style tagged unions/enums, they solve a lot of problems at once
i will probably have to just panic if accessed wrong, at least for now


exceptions are cringe (and a super pain in the ass from a compiler perspective i think)





for self

literal := LiteralInt | LiteralFloat | LiteralChar | LiteralString | LiteralBool
expr := simplexpr | complexpr
simplexpr := literal | Name
complexpr := (simplexpr | `(` complexpr `)`) binop (simplexpr | `(` complexpr `)`) | unop (simplexpr | `(` complexpr `)`) | functioncall 
binop := `and` | `or` | `+` | `-` | `*` | `/` | `%` | `==`| `~=`| `<=`| `>=` | `<` | `>` | `&`| `|` | `^` | `<<` | `>>`
unop ::=  `-` | `not` | `~` | `$` | `@`
functioncall := Name `(`[ expr {`,` expr}]`)`
reference := Name | reference `.` Name | pointerderef
stat := assignment | vardef | functiondef | externaldecl | functioncall | blockstat | returnstat | breakstat
block := `do` {stat} `end`
blockstat := ifstat | whilestat | forstat | block
ifstat := `if` expr block {`else if` expr block} [`else` block]
whilestat := `while` expr block
assignment := reference `=` expr
vardef := (`var` | `const`) TypeName{`$`}[`£`] Name `=` expr
functiondef := TypeName `(`[TypeName Name {`,` TypeName Name}]`)` Name block
file := {vardef | functiondef | externdecl}
this crap is a lot already, a lot of stuff refering to shit thats not gonna be supported for a while