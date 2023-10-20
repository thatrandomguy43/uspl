
### This is obviously not a very clean or organized document yet, and is mostly just to organize my own ideas.
(who on this earth would want to use this language anyway)


Built-in basic types

| Size    | Integer | Float   | Character |
| ------- | ------- | ------- | -------   |
| 8 bits  | int8    | N/A     | char      |
| 16 bits | int16   | N/A     | N/A       |
| 32 bits | int32   | float32 | N/A       |
| 64 bits | int64   | float64 | N/A       |

bool and void also exist with their obvious C++ meanings.
UPDATE: scrapped the wide chars, they add extra text encoding problems and make dealing with strings in the compiler more difficult
i'm just gonna add utf8 shit in the std::string clone i'll make (if this project isn't dead by then ofc)



blocks are started with do and ended with end like lua

the keywords and, or and not are used for boolean logic instead of operators

i think i'm gonna have to do oops all parentheses for binary operators so 3 * 4 + 2 must be (3 * 4) + 2
not doing that would mean i have to deal with operator presidence and introduce behaviour that is hard to program and easy to misunderstand 

another new syntax change for the sake of programming ease! function parameter names are decalred after the function type like this

function var int32(const int32, const char$$) main :argc, argv:

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



i think i finally decided what i wnat for arrays
they are basically a type of pointer, and there isn't any "value type" for arrays
they store pointer and size
i decided having const refs to non const arrays/ non const refs to const arrays is not really needed
it would be very rarely wanted

i dont think i can have references on top of this
they're just syntax sugar anyway, and i'm not gonna do pointer arithmetic, so that mistake is not a thing you can  make

wait i could merge ptr and array
this seems like a reasonable thing?
i just have regular ptrs be 1 length arrays, and have syntax shorthand of $p = p[ 0 ]
i'm still gonna leave references out until later, they are not really neccesary and complicate things for me
multi layer ptrs and arrays can exist at once like this!

i saw something else and thought of a good way to represent var/const with multilayer pointers
you put the $ in fromnt instead so 
var$ const char = const char*
const$ var$ const int32 = const int** const 
var$ const$ const$ var float64 = double * const * const *

this makes sense when read out, like the last one is "a variable pointer to a constant pointer to a constant pointer to a float64



i have been dealing with this for a whil now, but have not written it down yet
how do i do type conversion? which conversions between fundamental types should be allowed?
how does the user define their own conversions?
which also leads into operator overloading, or at least something like it
but i really do not think i can accomodate it, its too damn hard to deal with on top of everything else