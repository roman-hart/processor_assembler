<h1 align="center">Simple processor simulator</h1>
<p align="center"><i>Just to see how the main assembler commands for numbers work.</i></p>
<p align="center"><i>Developed as a laboratory project for <a href="http://www.univ.kiev.ua/en">Taras Shevchenko National University of Kyiv</a></i></p>
<br>

To start the program compile and run `main.cpp`, processor instructions are stored in `instructions.txt`

All commands have to be supplemented with 2 arguments: first - a name of the register, where the result will be stored, second - integer or register to take data from.

Command       | Function
------------- | -------------
mov      | Moves information to some register
inv     | Inverts beats
add     | Adds integer to some register  
add     | Adds integer to some register 
com     | Сomplementation code for an integer 

Output for `mov R1, 3`:

```
>>>> mov R1, 3                                                 
R1:  000111001111101100110010010111                            
R2:  101000011100100110100001110110                            
PS:  +                                                         
PC:  1                                                          
TC:  1                                                         
_______________________________________________________________
>>>> mov R1, 3                                                  
R1:  000000000000000000000000000011                              
R2:  101000011100100110100001110110                              
PS:  +                                                            
PC:  1                                                           
TC:  2                                                           
_______________________________________________________________
```

Here you can see all registers that being used by the program: R1 and R2 to store information, PS - staus register (here: sign of the last opperation / first element in processed register), PC and TC - command and ticker couters.

<br>
<br>

<h2 align="center">IEEE 754 - Floating point processing on stack processor</h2>

Compile and run `main2.cpp`, processor instructions are stored in `instructions2.txt`

There is a stack of registers (8 by default). No more named registers like in the first program.

Command       | Function
------------- | -------------
finit      | Registers initialization (insert zeros)
fild 1.1     | Insert 1.1 (ieee754 format) to the first register, shift values in other registers down by one
fadd     | Adds values in the first two registers, save result in the first, clear the second and shift values in others up
fdiv     | Divide value in the second register by the value in the first, clear the second, shift others up 
fdub     | Shift values down then insert the value in second register in the first (dublicates)

<br>
<h3 align="center">Functions to encode and decode numbers for IEEE754 standard using bitset library</h3>

From double to bitset (IEEE 754):

```cpp
std::bitset<BITS_LENTH> Processor::to_ieee754(double x) {
    if (x == 0) {
        return std::bitset<BITS_LENTH> (0);
    }
    std::bitset<MANTISSA_LENTH> m;
    string sign = x > 0 ? "0" : "1";
    x = abs(x);
    int slide = pow(2, EXPONENT_LENTH-1) - 1;
    int expo = -1;
    while (x >= 1) {x /= 2; expo++;}
    while (x < 0.5) {x *= 2; expo--;}
    std::bitset<EXPONENT_LENTH> exponent(slide + expo);
    
    for (int i=MANTISSA_LENTH; i>0; i--) {
        if (x > 0) {
            x *= 2;
            if (x >= 1) {  
                m[i] = 1;
                x -= 1;
            } else m[i] = 0;
        } else break;
    }
    return std::bitset<BITS_LENTH> (sign + exponent.to_string() + m.to_string());
}
```

From bitset (IEEE 754) to double:
```cpp
double Processor::from_ieee754(std::bitset<BITS_LENTH> &x) {
    std::bitset<MANTISSA_LENTH> m;
    int sign = x[BITS_LENTH-1] == 0 ? 1 : -1;
    int e = 0;
    int expo_bit_rank = 1;
    for (int i=MANTISSA_LENTH; i<BITS_LENTH-1; i++) {
        if (x[i] == 1) {
            e += expo_bit_rank;
        }
        expo_bit_rank *= 2;
    }
    double mant_bit_rank = 1;
    double result = 1;
    for (int i=MANTISSA_LENTH; i>=0; i--) {
        if (x[i] == 1) {
            result += mant_bit_rank;
        }
        mant_bit_rank /= 2;
    }
    if (result >= 2) result -= 1;
    if (result == 0) return pow(2, e-127)*sign;
    return result*pow(2, e-127)*sign;
}
```
