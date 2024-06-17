# Compilation

Install requirement:

```
pip install jinja2
```

Compile LAMP Stack:

```
/home/lind/lind_project/tests/lamp_stack/profiling/compile.sh
```

Make nodes for device files:

```
mkdir src/safeposix-rust/tmp/dev
sudo mknod src/safeposix-rust/tmp/dev/tty c 5 0
sudo chmod 666 src/safeposix-rust/tmp/dev/tty
sudo mknod src/safeposix-rust/tmp/dev/zero c 1 5
sudo chmod 666 src/safeposix-rust/tmp/dev/zero
sudo mknod src/safeposix-rust/tmp/dev/random c 1 8
sudo chmod 666 src/safeposix-rust/tmp/dev/random
sudo mknod src/safeposix-rust/tmp/dev/null c 1 3
sudo chmod 666 src/safeposix-rust/tmp/dev/null
```

Make related directory:


Load LAMP Stack:

Run Nginx:

```
/home/lind/lind_project/tests/lamp_stack/profiling/run_lamp_nginx_only.sh
```

