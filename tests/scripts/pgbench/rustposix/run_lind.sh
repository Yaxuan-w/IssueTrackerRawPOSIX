#!/bin/bash

echo -e "Resetting lind fs"
cd /home/lind/lind_project/lind/lindenv/fs
rm *
cd /home/lind/lind_project/tests/lamp_stack/pgbench

echo -e "Loading lind"
/home/lind/lind_project/src/mklind install &> /dev/null

echo -e "Loading postgres and bash"
/home/lind/lind_project/tests/lamp_stack/pgbench/load.sh > /dev/null

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <param>"
    exit 1
fi

param=$1

file="/home/lind/lind_project/tests/lamp_stack/pgbench/run_pgbench.sh"

if [ ! -f "$file" ]; then
    echo "File $file not found!"
    exit 1
fi

if [ "$param" == "10" ]; then
	sed -i '7s|.*|usr/local/pgsql/bin/pgbench -t 10 -U lind postgres|' "$file"
fi

if [ "$param" == "100" ]; then
	sed -i '7s|.*|usr/local/pgsql/bin/pgbench -t 100 -U lind postgres|' "$file"
fi

if [ "$param" == "1000" ]; then
	sed -i '7s|.*|usr/local/pgsql/bin/pgbench -t 1000 -U lind postgres|' "$file"
fi

if [ "$param" == "10000" ]; then
	sed -i '7s|.*|usr/local/pgsql/bin/pgbench -t 10000 -U lind postgres|' "$file"
fi

if [ "$param" == "100000" ]; then
	sed -i '7s|.*|usr/local/pgsql/bin/pgbench -t 100000 -U lind postgres|' "$file"
fi

lindfs cp /home/lind/lind_project/tests/lamp_stack/pgbench/run_pgbench.sh /run_pgbench.sh 

echo -e "Initializing postgres"
lind /bin/bash init_postgres.sh > /dev/null

echo -e "\nRunning pgbench"
lind /bin/bash run_pgbench.sh
