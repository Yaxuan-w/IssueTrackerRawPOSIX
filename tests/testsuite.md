# Testsuite

This document is a guideline for modifications from RustPOSIX to RawPOSIX test suite.

### `chmod`

1. Modify behaviors: cannot setting 0o777 so set to 0o755 instead
2. Modify behaviors: The extra bits are special permission bits in native linux

### `close` 

1. Fix bug in fdtable array

### `mmap` 

1. Fix error return 
2. Test fix: Native linux may default to treating the combination as one or the other. 
3. Test fix: Native linux will return EINVAL when offset is negative or not in range. 
4. Test fix: Native linux can mmap character device. 
5. Test fix: Native linux require specific flags to open a dir. 
6. Test fix: Native linux will return ENODEV when mapping a dir.

### `chdir` 

1. Fix bug that we should first check if desired path exists in native

### `fchdir` 

1. Test fix: Native linux require specific flags to open a dir. 
2. Fix bug that we should also updated the lind cwd (calling libc::cwd and then extract the working directory) after calling fchdir.

### `open` 

1. Fix the condition when opening large number of files

### `dup` 

1. Fix to handle bad file descriptor condition


### `dup2`

1. Fix bug that might be caused by below. Soln: reserving `STDIN / STDOUT / STDERR` in `lindrustinit` by redirecting fd=0/1/2 to `/dev/null`

```
Sometimes the `ut_lind_fs_dup2` can succeed sometimes cannot, so I suspect the error caused by following:

Daemon Processes: Daemon processes or services that explicitly close standard file descriptors to run in the background may result in open returning 0
```

2. Fix to handle bad file descriptor condition

### `fcntl`

1. Test fix: Use `FD_CLOEXEC` when setting and checking file descriptor flags with `F_SETFD` and `F_GETFD`.
2. Test fix: Change to use `&` to check if flag has been set, and `O_RDONLY` should be checked by using `O_ACCMODE` 

```
Set the file status flags to the value specified by arg.
              File access mode (O_RDONLY, O_WRONLY, O_RDWR) and file
              creation flags (i.e., O_CREAT, O_EXCL, O_NOCTTY, O_TRUNC)
              in arg are ignored.  On Linux, this operation can change
              only the O_APPEND, O_ASYNC, O_DIRECT, O_NOATIME, and
              O_NONBLOCK flags
```

3. Test fix: F_SETFD, F_SETFL with negative value args will not cause fcntl return error.
4. Fix bug that op is F_DUPFD and arg is negative or is greater than the maximum allowable value.
5. Fix bug that op is F_DUPFD and the per-process limit on the number of open file descriptors has been reached.
6. Fix bug that

```
Duplicate the file descriptor fd using the lowest-numbered
              available file descriptor greater than or equal to arg.
              This is different from dup2(2), which uses exactly the
              file descriptor specified
```

7. Test fix: We should do `lseek` first to put file position at beginning, because in native linux when we read from the end of the file it will return 0

```
EOF (End of File): This indicates that the end of the file has been reached. If reading from a regular file, this means there is no more data left to read.
```

### `ioctl`

1. Test fix (exclusive): change ioctl_union to libc::ioctl
2. Test fix (exclusice): Those invalid argument(`FIONBIO`) will success in native linux (ArchLinux)
            [https://stackoverflow.com/a/1151077/22572322]
            ...but these behaved inconsistently between systems, and even within the same system... 
3. Test fix: I suspect that The ioctl call first checks whether the file descriptor type supports the operation associated with the request code. If the file descriptor is of a type that does not handle the requested operation (such as a socket not supporting a terminal command), ENOTTY is returned.

### `link`

1. Test fix (exclusive): because in rawposix we'll transfer relative path into absolute path, native linux will always return `EPERM` instead of `ENOENT` when passing null value.

### `unlink`

1. Test fix (exclusive): because in rawposix we'll transfer relative path into absolute path, native linux will alway
s return `EISDIR` instead of `ENOENT` when passing null value.

### `rmdir`

1. Test fix (exclusive): because in rawposix we'll transfer relative path into absolute path, native linux will alway
s return `ENOTEMPTY` instead of `ENOENT` when passing null value.

2. Test fix: In the condition: calling `rmdir_syscall()`on the child directory should return `Directory does not allow write permission` error because the directory cannot be removed if its parent directory does not allow write permission. Native linux will return `EACCES` instead of `EPERM`.

3. Test fix: changing all directory name into test-specific, because native `mkdir` will return error when directory has been created before. 

4. Test fix: `rmdir` can succeed even if the directory has restrictive permissions because rmdir doesn't require write permissions on the directory itself; it requires write and execute permissions on the parent directory of the target directory.

5. Test fix: bug related to mkdir in rustposix but not in native linux, so changing test to correct version.


