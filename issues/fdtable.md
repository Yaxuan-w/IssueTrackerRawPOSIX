# FDTable 

## SegFault of DashMapArr Algo

Previously, DashMapArray algorithm met this issue when running:

```
[1:0 0:118] 05:04:17 Mon Jun 17 [lind@ef2ec2b4c0b9: +1] ~/lind_project 
@(1:118) $ lind /fork.nexe

executing: [sel_ldr -a -- "runnable-ld.so" --library-path "/lib/glibc" /fork.nexe]
[88,215026816:17:04:20.943303] BYPASSING ALL ACL CHECKS
[#1:L20] pid 1 forking

** Signal 11 from untrusted code: pc=dae0d507db1
/home/lind/lind_project/src/scripts/lind: line 86:    88 Segmentation fault      (core dumped) "${ldr_cmd[@]}" "${ldr_args[@]}"
```

When I changed to using DashMapVec, the program stably worked. 

Debugging with Dennis about this issue on 6/17, we found there're `jne` instructions in `copy_fdtable_for_cage` (before entering the function), but there'll not be any `jne` instructions for algos in DashMapVec. 

### Debugging

We analysised the value passed into the function and found that it shows `cannot access memory` in the fields of variables (`cageid` and `destcageid` in this cage). 

#### 1. Limited Stack Size

Dennis and I are firstly suspected root cause is: the limited size of stack. We tested with MAX_FD=10, and fork test worked. But when MAX_FD=1024 and I changed the stack limits to `unlimit`, DashMapArr still met the same problem. 

The way I changed the stack limits:
```
ulimit -s unlimit
```

## FIXED!!

This bug has been fixed by Nick. 
