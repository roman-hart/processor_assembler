<h1 align="center">Simple processor simulator</h1>
<p align="center"><i>Just to see how the main assembler commands work.</i></p>
<p align="center"><i>Developed as a laboratory project for <a href="http://www.univ.kiev.ua/en">Taras Shevchenko National University of Kyiv</a></i></p>
<br>


All commands have to be supplemented with 2 arguments: first - a name of the register, where the result will be stored, second - integer or register to take data from.

Command       | Method name  
------------- | -------------
mov      | Moves information to some register
inv     | Inverts beats
add     | Adds integer to some register  
com     | Сomplementation code for an integer


This command

```
com R2, 3
```

will work the same as

```cpp
mov R1, 3
inv R1, R1
add R1, 1
```
