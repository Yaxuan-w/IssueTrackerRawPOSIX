I currently initialized sockaddr with default Unix GenSockaddr, which has largest memory space. (which might cause error in getpeername)

IPv4/IPv6 shouldn't have path field, which might cause problem

We should do byte op instead of struct type check

---------------------------------------------

In Linux, there are various sockaddr structures [link from Ubuntu]. After tracing down the postgres source code, I found that postgres uses sockaddr_storage [link from postgres] [link from postgres], which is cast to sockaddr when used [link from postgres]. The difference between sockaddr_storage and sockaddris the size. sockaddr_storage is designed to allocate enough memory to satisfy the memory requirements of all sockaddr types.

I also observed that syscalls like accept will receive sockaddr->family=0 with sock_len=128 from postgres for all types sockaddrs, making it impossible to determine the family based on size alone.

To address these issues, I suggest we could consider allocating an extra buffer beforehand for accept / recvfrom / getsockname / getpeername, while using your design (passing ptrs) for rest. This would involve

1. refining the conversion steps in the dispatcher to directly allocate a buffer with size=128 [current rawposix implementation],
2. implement new function for passing ptrs
3. and then performing the necessary copy&paste operations.


------------------------------------------
I refactored accept_syscall [rawposix link] and found the root cause: we were passing a NULL value to libc::accept, 
causing libc::accept to always return a NULL sockaddr. Then, the copy_out function in the dispatcher didn’t perform any operations.
Due to the fact that the three address structures (UNIX - 110 bytes, IPv4 - 16 bytes, IPv6 - 28 bytes) are contiguously allocated in 
memory starting from the initial position, we use their lengths to determine the different fields. Therefore, when initializing the 
GenSockaddr structure, it essentially reserves an address space containing the corresponding address information. In the copy_out 
function, we only copy the mini number of bytes between the initial length (initaddrlen) and the corresponding length of GenSockaddr.

This means that even if the returned address structure is a UNIX address structure, but the structure was previously initialized as 
an IPv4 address structure, it will still only copy the first 16 bytes of the reserved address space (since the length of the IPv4 
address structure is 16 bytes). This will result in failure when getsockname is used in PostgreSQL to retrieve the sockaddr information.

To avoid this issue, I modified the type used to initialize GenSockaddr to be a UNIX address structure, thereby always reserving the 
maximum byte space (110 bytes). In the copy_out function, the number of bytes to copy is determined by comparing initaddrlen with the 
actual length of the structure (taking the minimum value), ensuring that the appropriate number of bytes is copied even if it is an IPv4 
or IPv6 address structure, without exceeding the reserved space.

This explained why only declaring IPv4 GenSockaddr for all cases, whether IPv4, IPv6, or Unix all worked before. I tested directly passing 
sockaddr into rawposix and resulted in failure.

To resolve this, I modified accept_syscall to initialize a default GenSockaddr struct [rawposix dispatcher link] based on the family of 
the sockaddr received during the dispatcher stage [rawposix dispatcher link] and handled the Unix path conversion within the syscall.
