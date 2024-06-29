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
mkdir /home/lind/lind_project/src/safeposix-rust/tmp/dev
sudo mknod /home/lind/lind_project/src/safeposix-rust/tmp/dev/tty c 5 0
sudo chmod 666 /home/lind/lind_project/src/safeposix-rust/tmp/dev/tty
sudo mknod /home/lind/lind_project/src/safeposix-rust/tmp/dev/zero c 1 5
sudo chmod 666 /home/lind/lind_project/src/safeposix-rust/tmp/dev/zero
sudo mknod /home/lind/lind_project/src/safeposix-rust/tmp/dev/random c 1 8
sudo chmod 666 /home/lind/lind_project/src/safeposix-rust/tmp/dev/random
sudo mknod /home/lind/lind_project/src/safeposix-rust/tmp/dev/null c 1 3
sudo chmod 666 /home/lind/lind_project/src/safeposix-rust/tmp/dev/null
sudo mknod /home/lind/lind_project/src/safeposix-rust/tmp/dev/urandom c 1 9
sudo chmod 666 /home/lind/lind_project/src/safeposix-rust/tmp/dev/urandom
```

Load LAMP Stack:

Run Nginx:

```
/home/lind/lind_project/tests/lamp_stack/profiling/run_lamp_nginx_only.sh
```

Change python script:

Move  
