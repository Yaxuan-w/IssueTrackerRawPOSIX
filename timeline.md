## 8/19/2024

- Finished RawPOSIX named pipe comparison graphs and pushed to remote. Including:
  - The combination of graphs of all cases (normal pipe / single named pipe / multi named pipe)
  - The comparison of rawposix single named pipe / multi named pipe / normal pipe
  Seems named pipe has similiar performance with normal pipe in RawPOSIX
- Fixed the new line issue in PR
- Wrote test case for SFI penalty test. 
  - In default optimization level, lind has around 7% SFI penalty which aligns with nacl's paper
  - In O1 optimization level, lind has similiar total runtime with native linux 
  - In O3 optimization level, lind is faster than native linux on both server2 and server3. I found in paper “google-native client-2” page 8
    > This suggests out-of-order execution can help hide the overhead of SFI, although other factors may also contribute, including much smaller caches on the Atom part and the fact that GCC’s 64-bit code generation may be biased towards the Core 2 microarchitecture
  - I checked the CPU type for server3 (`Intel(R) Xeon(R) Gold 5416S`) and server2 (`12th Gen Intel(R) Core(TM) i9-12900K`). Both of them belongs to out-of-order CPU. I’m guessing that O3's optimization strategy combined with the out-of-order CPU type causes lind to run faster?

## 8/18/2024

- Finished named pipe - rawposix tests and pushed to repo
- Generated graphs and pushed to repo too

## 8/16/2024

- Rawposix single process named pipe tests stalled. Looking at the reason
- Met with Rick and talked about next steps
- Submitted PR to replace original pipe scripts

## 8/15/2024

- Finished rawposix normal pipe testing and generated graphs
- Reimplemented scripts for rawposix named pipe and started testing

## 8/14/2024

- Fixed the older version script's bug. The bug is caused by passing `x` instead of real number to execute. 
- Older verison scripts works normally in rawposix / lind / native / unsafe. New scripts will encounter error when generating result files, because we want to extract the platform from user input. When we input command line, it will become impossible to distinguish if the platform is rawposix or normal lind, since they are using same command line. 
- Changed to use older version of runtime scripts. Running tests on server2 with different count sizes to see the time variances. 
- Finished rawposix named pipe tests short version (1GB) for both two processes and single process. 

## 8/13/2024

- Modified test files for rawposix / lind / unsafe. Modified scripts for rawposix/lind/unsafe. Then found that we should use gcc4 for native and unsafe. 
- Finished: 
  - setup script
  - change “user” to “unsafe”
  - change compilation file to use gcc-4 for both native and unsafe
  - make unsafe using -O3
  - merge 3 tests file into 1 (merge stderr&stdout)
  - merge 3 runtime scripts into 1
  - move pipe-cages.c to total_runtime/scripts/unsafe-pipe.c
- Finished -c=5 tests for rawposix / lind / unsafe / native and generated results. Talked with Nick and found that there's some huge error bars, and we decided to increase times of runs.
- Finished:
  - make clean file
  - create separate scripts for compilation
- Start running -c=50 tests for lind / unsafe / native
- New scripts didn't get response from rawposix

## 8/12/2024

- Get results
- Talked with Nick about optimizing tests. We decided to record start time from write-end and end time from read-end then calculating difference to get the runtime results in order to save time. I finished changing related python files and runtime scripts, pushed to remote.

## 8/11/2024

- Finished setups for native and rustposix r/w tests and started running.

## 8/10/2024

- Get results

## 8/9/2024

- Finished setups of rawposix r/w tests and started running. Wait tmr to see the results

## 8/8/2024

- Finished runtime scripts and modifications to tests (RawPOSIX with regular pipe vs rawposix w named pipe vs rawposix 2 process with named pipe)
- Fixed close error returns issue in fdtables (NEED TO SUBMIT PR) - errno should be EBADF instead of EBADFD

## 8/7/2024

- Submitted PR for fdtables fix which including fixes and tests
- Listed ToDos and talked with Nick
- Learned and implemented tests for test#1 (named pipes), generated scripts, and ran locally
- Gave David and Vlad instructions on how to fix test suite

## 8/6/2024

- Discussed with Rick, Justin, and Nick about following test setups

## 8/5/2024

- Porting the RustPOSIX test suite into RawPOSIX, fixing bugs in both tests and RawPOSIX, and documenting fixes

## 7/9/2024

- Scripted setup / clear stage for rawposix
- Redid experiment
- rawposix and native will stay hung in pgsql initialization but initialization will finish normally - don't know reason

## 7/8/2024

- Tried with new server

## 7/1/2024

- Generated nginx-only test diagram
- Need to redo experiment

## 6/29/2024

- Try with newer version of fdtable

test with socketselect.c and met some weird outputs:

```
@(1:14) $ lind /socketselect.nexe

executing: [sel_ldr -a -- "runnable-ld.so" --library-path "/lib/glibc" /socketselect.nexe]
[218,2017537152:18:46:50.266644] BYPASSING ALL ACL CHECKS
Waiting on select()...
Hello message sent
  Listening socket is readable
  New incoming connection - 9
Waiting on select()...
  Descriptor 9 is readable
  17 bytes received
Hello from client
  Connection closed
Waiting on select()...
  select() timed out.  End program.
thread '<unnamed>' panicked at src/example_grates/dashmapvecglobal.rs:439:49:
called `Option::unwrap()` on a `None` value
stack backtrace:
```

- The program will met the same error after the execution finished 
- Suspect error in fdtable
- FIXED the fdtable issuse
  - Cause: The function close_virtualfd might not be removing the entry from FDTABLE because the cloned myfdrow is not being updated back into FDTABLE. When we clone myfdrow, we are working on a separate copy of the data. Any changes made to myfdrow will not affect the original data in FDTABLE. To ensure that the changes are reflected in FDTABLE, you should update FDTABLE after modifying myfdrow.
  - Fix: Add `FDTABLE.insert(cageid, myfdrow.clone());` after setting data to `None`

- LAMP Stack now works smoothly!!!!
- Generated full LAMP Stack diagram

## 6/27/2024

- Met error when running benchmarks 
- the error might caused by log file..? / caused by incorrect fdtable behavior..?

## 6/26/2024

- Working on adding getpeername test to lind testsuite
- Benchmarking rawposix with rustposix's experiment

## 6/25/2024 - LAMP Stack could run with RawPOSIX!!!

- Suspect incorrect error handling: tested by change Err in FDTable to panic
  - Analysis the printing msg and found actual used real fd should be really large... (bc kernel should pick the least unused one)
  - Start testing with smaller test file (socketselect.c)
  - Found rawposix DO open more fds, root cause: close_virtual should close real fd in some case but rawposix didn't. Try to find the reason
  - The FDTable interface doesn't remove the reference count entry when performing a kernel close, resulting in the real file descriptor never being closed if it is used more than once.

- Met `socket.py` with `BADADDR` error
  - rawposix didn't handle error condition
  - Not related to socket, related to `getpeername`

```
Traceback (most recent call last):
  File "/usr/local/lib/python2.7/site-packages/gunicorn/workers/sync.py", line 134, in handle
    req = six.next(parser)
  File "/usr/local/lib/python2.7/site-packages/gunicorn/http/parser.py", line 41, in __next__
    self.mesg = self.mesg_class(self.cfg, self.unreader, self.req_count)
  File "/usr/local/lib/python2.7/site-packages/gunicorn/http/message.py", line 187, in __init__
    super(Request, self).__init__(cfg, unreader)
  File "/usr/local/lib/python2.7/site-packages/gunicorn/http/message.py", line 54, in __init__
    unused = self.parse(self.unreader)
  File "/usr/local/lib/python2.7/site-packages/gunicorn/http/message.py", line 236, in parse
    self.headers = self.parse_headers(data[:idx])
  File "/usr/local/lib/python2.7/site-packages/gunicorn/http/message.py", line 74, in parse_headers
    remote_addr = self.unreader.sock.getpeername()
  File "/usr/local/lib/python2.7/Lib/socket.py", line 224, in meth
    return getattr(self._sock,name)(*args)
error: [Errno 14] Bad address
```

  - type conversion is correct after modification, but still same error. 
  - might caused by socket syscall not success..?
  - no, socket syscall success but getpeername failed in general test case (NEED TO ADD INTO TESTSUITE)
  - still consider type conversion
  - FIXED!!! error is caused by incorrect argument when calling libc::getpeername lol

## 6/24/2024

- Might be int overflow..?
- Print out total open fd in rawposix: around 80 (should be good?
- Try to print total open fd in rustposix:

## 6/21/2024

- Found some wierd performance about fd in rawposix, need more debugging.

## 6/20/2024

- Talked with Nick and confirmed that lamp stack won't execute epoll/poll when dealing with large fds. He suspected that rawposix opened more fds than rustposix, and `close` might have incorrect behaviors.

## 6/19/2024

- Made python run in rawposix and met the large fd issue again

## 6/18/2024

- Try running LAMP Stack with newest libc
  - `bind` / `connect` failed due to ownership issues
  - Solved by moving out variables

- Real FD too large
  - LAMP Stack will auto change to use `epoll` / `poll` when FD >= 1024. The process limitaiton is

```
@(1:248) $ ulimit -n
1048576
```
    
But in RawPOSIX, LAMP Stack didn't known the real fd, so it won't change to use `epoll` or `poll` when using `select`, and will cause `libc::FD_SET` fail.
 
## 6/16/2024 - 6/17/2024

- Keep debugging the Nginx
  Try to get the strace results and compare to see the error location
  - last read returns 0 but should return 19
  - track to fd=255 ... very beginning is gethostname..? - NO
  - problem in select: `select(8, 0x4d110527e0, 0x4d11052860, 0x00000000, 0x00000000) = 1`
  - keep tracking down, problem is in `bind` - **sometimes working sometimes not... NOT known the root cause**
  - get the error log: `fcntl(F_SETOWN) failed while spawning "worker process" (3: No such process)`
  	- F_SETOWN shouldn't call kernel (process / signals is handled inside of Lind)
	- In `F_SETOWN` and `F_GETOWN` conditions should always return 0
  - `accept` met non-UTF-8 error. Unix path might not be UTF8 chars, so change implementation to directly memory operations. Refined `connect` and `bind` as well. 

## 6/14/2024

- RawPOSIX: Nginx could connect and set the port to listening status, but cannot get the file
  - When debugging: trying to locate the error with running in verbose mode but failed 

## 6/13/2024

- Fixed type casting bugs in `getsockname` and `getsockopt`
- RawPOSIX could run PostgreSQL
- Traced down to the root cause of sockaddr (details in sockaddr.md)
- Implemented error handling for all libc calls
- Fixed kernel_close callback usage
