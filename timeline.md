## 6/13/2024

- Fixed type casting bugs in `getsockname` and `getsockopt`
- RawPOSIX could run PostgreSQL
- Traced down to the root cause of sockaddr (details in sockaddr.md)
- Implemented error handling for all libc calls
- Fixed kernel_close callback usage
