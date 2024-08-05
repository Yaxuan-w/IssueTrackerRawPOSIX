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
