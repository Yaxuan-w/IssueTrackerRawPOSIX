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
