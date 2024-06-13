# IssueTrackerRawPOSIX

## Convert Path in Unix Domain

Modify related syscalls (`sendto` / `connect` / `bind`)

Refine `accept` syscalls and find the root cause of the previous problems. 

## Epoll & Poll

There'll be different cases in rawposix: In Memory Pipes / Kernel glibc / etc.. 

Current FDTable interface only support the non-real FD or the combination of non-real / real I chose to handle the kernel option manually. 

...Wait to add more details...

## Error handling 

I added the error handling when translating the virtual fd to kernel fd.

Need to handle other cases 
