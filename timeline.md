## 10/6/2024

### Loopback v.s. Pipe on Native

Nick noticed that the native loopback for 4 looks like its around 25% of native pipe but the raw data suggests it should be slower. I output mean and std for raw data and calculated by hand to verify correctness. The mean of native loopback is ~3.5x slower than native pipe, which aligned to the graph. I checked the results from server3, native loopback is ~3x slower than the native pipe, which aligns to sphere results.

I used tcpdump when running loopback test on native and captured lots of network activities. We handled loopback as domain socket in RustPOSIX which never goes to kernel. My guess is that loopback on native will still go to various layers of the networking stack to communicate via TCP, which will cause extra overhead.

### LAMP Stack on Lind and Native
Updated app.py: https://github.com/Lind-Project/lind_project/blob/lamp-all-4/tests/lamp_stack/profiling/flask_app/app.py

#### Error-1: Cannot find table
**Cause:** The target table could not be found in the database.
**Solution:** Added database initialization logic that automatically inserts data when the table is empty, ensuring that the table is properly initialized before any queries are executed.

**Error-2:** Access uninitialized ID
**Cause:** An uninitialized ID was being accessed, leading to errors.
**Solution:** Refined app.py to ensure that data initialization only happens when necessary, preventing access to uninitialized IDs.

**Error-3:** Cage ID is outside a valid range (only met in lind)
**Cause:** A new database connection was being created and closed for each request, which resulted in a large number of processes being started in a short time, ultimately causing resource limits to be exceeded.
**Solution:** Changed to use a connection pool to replace the frequent creation and closing of connections, limiting resource usage and reducing system load.

**Benchmark Command Line**
```sh
wrk -t1 -c1 -d30 --timeout 30 http://localhost:80/queries?queries=10
wrk -t1 -c1 -d30 --timeout 30 http://localhost:80/db
wrk -t1 -c1 -d30 --timeout 30 http://localhost:80/plaintext
```

## 9/23/2024

quick updates:
**For testing on sphere**
it shows 
```sh
@(1:28) $ lind /write.nexe 65536 > outputs/lind_65536_write.txt

executing: [sel_ldr -a -- "runnable-ld.so" --library-path "/lib/glibc" /write.nexe 65536]
[3652,3107958592:02:53:08.365782] BYPASSING ALL ACL CHECKS
thread '<unnamed>' panicked at std/src/io/stdio.rs:1117:9:
failed printing to stdout: No space left on device (os error 28)
```
i need to find how to increase memory, i currently used the default memory settings.

**Automation**
I finished almost all related scripts in branch ipc-fix (link: https://github.com/Lind-Project/lind_project/tree/ipc-fix)
Details:
- `/lind_project/ipc_test.sh`: suppose to be the general script that run all the tests
- `tests/ipc_performance/sfi_overhead`: suppose to be the SFI overhead tests.
  - Run corresponding tests and extract from output, then generate required file. getpid() and close() parts are tested.
  - Switching scripts for RawPOSIX -> RustPOSIX fast -> RustPOSIX default
- `tests/ipc_performance/buf_allocate`: suppose to be the uds/pipe on lind/native tests.

**TODO**
- `write()` encountered problems on server2 when testing. After redirecting output to a file, `tail -n 1` should be able to extract the average time results, but failed when testing tonight. The server was down before I analyzed the problem. On sphere I met memory limitation. Tmr will start from this.
- Need to test switching scripts
- Need to test overall process
Then should be good to generate Docker image


### `write()` with `perf` on both Native linux and Lind (RustPOSIX)
- Compile rustposix with debug=fast
- make perf graph: lind/native buf_size=4

#### Configuration

Compile:

```sh
x86_64-nacl-gcc write.c -o write.nexe -lrt -std=gnu99 -O3
/usr/local/gcc-4.4.3/bin/gcc write.c -o write -lrt -std=gnu99 -O3
```

Run:

```sh
./write 1 > nat_1.txt
lind /write.nexe 1 > lind_1.txt
```

#### Results

```sh
==> lind_1.txt <==
Buffer size 1 bytes: 1000000 write() calls, average time 114 ns

==> lind_16.txt <==
Buffer size 16 bytes: 1000000 write() calls, average time 128 ns

==> lind_256.txt <==
Buffer size 256 bytes: 1000000 write() calls, average time 324 ns

==> lind_4096.txt <==
Buffer size 4096 bytes: 1000000 write() calls, average time 1962 ns

==> lind_65536.txt <==
Buffer size 65536 bytes: 1000000 write() calls, average time 39855 ns

==> nat_1.txt <==
Buffer size 1 bytes: 1000000 write() calls, average time 360 ns

==> nat_16.txt <==
Buffer size 16 bytes: 1000000 write() calls, average time 367 ns

==> nat_256.txt <==
Buffer size 256 bytes: 1000000 write() calls, average time 448 ns

==> nat_4096.txt <==
Buffer size 4096 bytes: 1000000 write() calls, average time 1515 ns

==> nat_65536.txt <==
Buffer size 65536 bytes: 1000000 write() calls, average time 37755 ns
```

## 9/13/2024

- The issue occurred because the Ubuntu container had multiple versions of GCC installed (gcc and gcc-7). When compiling GCC-4 in the Sphere, gcc-7 was detected and used for compilation, but on the server, gcc (version 9) was automatically used for testing. This led to failures in modifying the Makefile to pass the necessary flags, resulting in gcc compilation failure. 

- I also found several typos in gcc compilation scripts. Accidentally those typos fix problem from another way so the compilation successes when I tested on local server. 

- To run benchmark on sphere, I encountered errors due to lacking source files for unsafe code. @Yashaswi Makula might need to keep files in /home/lind/lind_project/tests/native-rustposix/ when generating sphere image.

- I have fixed the issue and will submit PR. I have tested gcc compilation script in Sphere from scratch, and it should work without any problems now.

## 9/12/2024

- Wrote test case and ran UDS tests
    - pong - alternating send/recv 2kb in a way that read and write are not overlapping
    - graphed results and pushed to remote

## 9/11/2024

- Opened 2 Issues in RawPOSIX repo to describe and track tasks for current RawPOSIX

## 9/10/2024

- Debugging gcc-4 on ubuntu-lind
- Wrote scripts to get things running on sphere with only one botton
- Compiled gcc-4 on lind-ubuntu
- Fixed minor bugs in IPC test to run on both lind-ubuntu and lind-archlinux, generated overview script, and submitted PR

## 9/9/2024

- Indirect getpid() syscall for native
  - Wrote test case and regenerate graph with new native data and old lind data
- write() 11 runs and generated graph with error bars 
- Re-generated lock graphs with raw time data
- Implemented send() test case

## 9/7/2024

- Re-run close() tests:
I got similar results this time, and nothing weird happened.
  - Test case: https://github.com/Yaxuan-w/IssueTrackerRawPOSIX/blob/main/tests/basicTests0806/sfi-tests/close.c
  - Configure: gcc-4/x86_64-nacl-gcc close.c -o close.nexe -std=gnu99 -lrt -O3
  - Test results: https://github.com/Lind-Project/lind-ipc-data/tree/main/micro-benchmarks/sfi/data-0907

- Re-run write() tests:
similar results this time
  - Test case: https://github.com/Yaxuan-w/IssueTrackerRawPOSIX/blob/main/tests/basicTests0806/sfi-tests/write.c
  - Configure:
    - Native client in branch io-novmcheck
    - Compiled by gcc-4/x86_64-nacl-gcc write.c -o write.nexe -O3 -std=gnu99 -lrt
  - Test results: https://github.com/Lind-Project/lind-ipc-data/tree/main/micro-benchmarks/sfi/data-0907

- Pipe tests with same format as the uds test:
I made a new test thats basically the exact same format as the uds test. When generating graph, I set native-uds as 1, and I noticed weird results for native with buffer size 2^16. I re-ran the tests but got the similar results…
  - Test case: https://github.com/Lind-Project/lind_project/blob/pipe-use-udstest/tests/ipc_performance/total_runtime_2/scripts/pipe_write_read.c
  - Configure: All repos use develop branch
  - Test results and graphs: https://github.com/Lind-Project/lind-ipc-data/tree/main/micro-benchmarks/pipe/pipe-uds-nat-lind/data-0907

- PR for some minor fixes:
I added a README file for gcc-4.4.3 inside its folder. Changed file permissions for scripts inside pipe/uds test folder.
  - PR link: https://github.com/Lind-Project/lind_project/pull/387

## 9/3/2024

- Generated cs graphs and slides templates
- UDS


## 9/2/2024

- Finished
  - 

## 8/27/2024

- Finished
  - Redid write tests
  - Redid context switching tests
  - Test SPHERE
  - Read master thesis

## 8/26/2024

- Finished 
  - Redid write() tests -- redirect native output to actual file
  - Implemented new one to use perf stat in all config of pipe tests and extract context time
  - Graphs

## 8/23/2024

- Finished:
  - run 16_x on both server2 and 3, put results into spreadsheet
  - combine 3 graphs (syscall) into just one
  - graph: write syscall w/ varied buffer size 2^0, 2^4, 2^8, 2^12, 2^16

## 8/22/2024

- Finished graphing write results

## 8/21/2024

- Finished
  - UDS:
    - C file: Print before we call parent, print after child
    - Combine different python file into one
    - Use gcc-4 for native (standardized)
  - getpid()
  - close() on invalid fd
  - comparing write() to stdout

## 8/20/2024

- Talked with Rick and Nick about next steps and data analysis.

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
