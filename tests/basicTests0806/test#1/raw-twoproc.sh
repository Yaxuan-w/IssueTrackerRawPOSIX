x86_64-nacl-gcc two-process-write-namedpp.c -o /home/lind/lind_project/src/safeposix-rust/tmp/w.nexe
x86_64-nacl-gcc two-process-read-namedpp.c -o /home/lind/lind_project/src/safeposix-rust/tmp/r.nexe

lind /r.nexe &
lind /w.nexe "hello"