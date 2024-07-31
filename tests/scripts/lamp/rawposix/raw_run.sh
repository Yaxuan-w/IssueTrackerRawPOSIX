#!/bin/bash

/home/lind/lind_project/clear_raw.sh

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <param>"
    exit 1
fi

param=$1

file="/home/lind/lind_project/src/safeposix-rust/tmp/app.py"

if [ ! -f "$file" ]; then
    echo "File $file not found!"
    exit 1
fi

if [ "$param" == "128KB" ]; then
    sed -i '8s/.*/html_size_128KBs = 2 ** 0/' "$file"
fi

if [ "$param" == "512" ]; then
    sed -i '8s/.*/html_size_128KBs = 2 ** 2/' "$file"
fi

if [ "$param" == "2" ]; then
    sed -i '8s/.*/html_size_128KBs = 2 ** 4/' "$file"
fi

if [ "$param" == "8" ]; then
    sed -i '8s/.*/html_size_128KBs = 2 ** 6/' "$file"
fi

if [ "$param" == "32" ]; then
    sed -i '8s/.*/html_size_128KBs = 2 ** 8/' "$file"
fi

if [ "$param" == "128MB" ]; then
    sed -i '8s/.*/html_size_128KBs = 2 ** 10/' "$file"
fi

echo "Modification based on param $param completed."

lind /bin/bash init_postgres.sh
lind /bin/bash run_lamp.sh
