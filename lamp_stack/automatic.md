Could start testing if you see the line: 

```
[2024-07-15 11:09:22 +0000] [30] [INFO] Booting worker with pid: 
```

Open another terminal and run this command to test:

```
wrk -t1 -c1 -d30 --timeout 30 http://localhost:80
```

Here're mapping relationship for testing with different html size:

```
128MB 128MB
512MB 512
2MB 2
8MB 8
32MB 32
128MB 128
```

### Native

Container name: `native-lamp`

```
cd tests/lamp_stack/profiling
./native_run.sh FILESIZE # remember to replace FILESIZE with desired number
```


### RawPOSIX

Container name: `raw-lamp`

```
./raw_run.sh FILESIZE
```

After first run

```
lind /bin/bash run_lamp.sh
```

### RustPOSIX

Container name: `normal-lamp`

```
./lind_run.sh FILESIZE
lind /bin/bash run_lamp.sh
```

After first run

```
./clear.sh
lind /bin/bash run_lamp.sh
```

