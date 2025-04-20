# READ ME!!
##
This is a basic cache simulator I designed for a Computer Architecture course. Supports set-associative cache as well as direct mapped cache (when set associativity = 1). 
Currently this runs over select 'addresses' for testing purposes. I intend to refactor the main file for more user input regarding reading/writing to the memory.


## RUN THE EXECUTABLE `runme.exe`

There is already an .exe file for this project included in the .zip. For whatever reason, if this doesn't work, this can be manually compiled with a C/C++ compiler of your choosing.

Just remember when compiling, include cache.cpp in the call. e.g.,

g++ main.cpp cache.cpp

(if using gcc/g++)

Code is documented robustly.

# Note:
In any case runme.exe is not working, I copied the output of the file to output.txt via
the `./runme > output.txt` command in my cli.