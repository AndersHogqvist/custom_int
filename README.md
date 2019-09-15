# custom_int
An integer (signed and unsigned) class based on std::bitset

# Background
During a project at my work I needed a convenient way to store values taking up the smallest amount of bits necessary. I then started to search around for a solution but found none that suited my needs so I decided to develop one my self.

# Usage
The idea was to make the usage as close to a regular integral type as possible.

#### Initialization:
```
Int<13> value1;
UInt<4> value2 = 2;
```
For implementations where a certain type is used alot one can make life easier with typedef:
```
typedef Int<5> Int5;
typedef Int<7> Int7;
typedef UInt<4> Uint4;
typedef UInt<9> Uint9;

Int5 value1;
Int7 value2;
Uint5 value3;
Uint9 value4;
```

#### Arithmetics
I've implemented support for just about every kind arithmetic operations including additon, subtraction, division and multiplication.
```
Int<5> value1, value2;

value1 = 1;
value2 = value1;

value1++;
value2--;

value1 * value2;

value1 * -2;

etc...
```
# Contribution
I welcome any idea on how to improve this and please let me know if you find anything that isn't working!
