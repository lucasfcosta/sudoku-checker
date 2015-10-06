# sudoku-checker
A program that checks if Sudoku solutions are valid.
This has been written for the Operating Systems subject in UFSC - Universidade Federal de Santa Catarina.

The base code for this and the sudoku examples were given to us by [Professor Márcio Castro](http://www.marciocastro.com/).


## The Challenge

* We should evaluate a Sudoku Grid (9x9) and find errors on lines, columns or regions using as many threads as the user wants.
* When a thread finds an error it should print its number alongside the description of where is the error (Example: `thread 0 - Error on column 2`)
* After completing every validation the total number of errors should be printed too


## Compiling

```
gcc -o sudoku sudoku-base.c -pthread -std=c11
```


## Running

```
./sudoku <filename.txt> <number_of_threads>
```


## Authors

* [Lucas Fernandes da Costa](http://lucasfcosta.com)
* Guilherme Nunes