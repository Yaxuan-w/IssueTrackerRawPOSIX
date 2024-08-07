gcc two-process-read-namedpp.c -o r
gcc two-process-write-namedpp.c -o w

./r &
./w "hello"