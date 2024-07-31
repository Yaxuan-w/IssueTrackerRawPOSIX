## PGBENCH

```
number_of_transactions=10
number_of_transactions=100
number_of_transactions=1000
number_of_transactions=10000
number_of_transactions=100000
```

### RawPOSIX:

Container name: `raw-pg`

```sh
./run_raw.sh number_of_transactions
```

### RustPOSIX:

Container name: `normal-pg`

```sh
./run_lind.sh number_of_transactions
```

### Native:

Container name: `native-pg`

```sh
./run_native.sh number_of_transactions
```

## NGINX

```
128KB: static_html_file_size_in_mbs=17

512KB: static_html_file_size_in_mbs=19

2MB: static_html_file_size_in_mbs=21

8MB: static_html_file_size_in_mbs=23

32MB: static_html_file_size_in_mbs=25

128MB: static_html_file_size_in_mbs=27
```

### RawPOSIX:

Container name: `raw-nginx`

You can running test continuously

First terminal:

```sh
/home/lind/lind_project/tests/lamp_stack/profiling/lind_setup_nginx_only.sh -s static_html_file_size_in_mbs

lind /bin/bash run_lamp_nginx_only.sh
```

Second terminal:

```sh
wrk -t1 -c1 -d30 --timeout 30 http://localhost:80/static.html
```

### RustPOSIX:

Container name: `normal-nginx`

First terminal:

```sh
/home/lind/lind_project/tests/lamp_stack/profiling/lind_setup_nginx_only.sh -s static_html_file_size_in_mbs

lind /bin/bash run_lamp_nginx_only.sh
```

Second terminal:

```sh
wrk -t1 -c1 -d30 --timeout 30 http://localhost:80/static.html
```

### Native:

Container name: `native-nginx`

First terminal:

```sh
./run_native.sh static_html_file_size_in_mbs
```

Second terminal:

```sh
wrk -t1 -c1 -d30 --timeout 30 http://localhost:80/home/lind/lind_project/static.html
```

## LAMP Stack

Could start testing if you see the line: 

```sh
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

