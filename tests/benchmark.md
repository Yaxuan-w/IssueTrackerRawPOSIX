# Benchmark

## LAMP Stack

Change variables in `app.py`

## NGINX-ONLY

Generated related files (in `/home/lind/lind_project/tests/lamp_stack/profiling/flask_app`):

- 128KB

```
python2.7 gen_static_html.py -s 17 -o /home/lind/lind_project/src/safeposix-rust/tmp/static.html
```

- 512KB

```
python2.7 gen_static_html.py -s 19 -o /home/lind/lind_project/src/safeposix-rust/tmp/static.html
```

- 2MB

```
python2.7 gen_static_html.py -s 21 -o /home/lind/lind_project/src/safeposix-rust/tmp/static.html
```

- 8MB

```
python2.7 gen_static_html.py -s 23 -o /home/lind/lind_project/src/safeposix-rust/tmp/static.html
```

- 32MB

```
python2.7 gen_static_html.py -s 25 -o /home/lind/lind_project/src/safeposix-rust/tmp/static.html
```

You can use `ls -lh /home/lind/lind_project/src/safeposix-rust/tmp/static.html` to check the file size.

Run nginx from terminal:

```
lind /bin/bash run_lamp_nginx_only.sh
```

Run command line on second terminal:

```
wrk -t1 -c1 -d30 --timeout 30 http://localhost:80/static.html
```

## PGBENCH


