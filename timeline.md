## 6/16/2024

- Keep debugging the Nginx
  Try to get the strace results and compare to see the error location
  
## 6/14/2024

- RawPOSIX: Nginx could connect and set the port to listening status, but cannot get the file
  - When debugging: trying to locate the error with running in verbose mode but failed 

## 6/13/2024

- Fixed type casting bugs in `getsockname` and `getsockopt`
- RawPOSIX could run PostgreSQL
- Traced down to the root cause of sockaddr (details in sockaddr.md)
- Implemented error handling for all libc calls
- Fixed kernel_close callback usage
