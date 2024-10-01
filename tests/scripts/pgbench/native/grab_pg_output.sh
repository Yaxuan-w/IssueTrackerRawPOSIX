#!/bin/bash

param=$1
repeat_count=$2
json_file="tps_data.json"

total_tps_excluding=0

echo "{" > $json_file

for ((i = 1; i <= repeat_count; i++)); do
  echo "Running iteration $i of $repeat_count..."
  
  ./run_pg_nat.sh "$param" &> pg.log

  tps_excluding=$(grep "tps =" pg.log | grep "excluding" | awk '{print $3}')
  
  total_tps_excluding=$(echo "$total_tps_excluding + $tps_excluding" | bc)
  
  echo "TPS (excluding) for iteration $i: $tps_excluding"
  
  # JSON format: [ith iteration]: [tps_excluding]
  if [ "$i" -eq "$repeat_count" ]; then
    echo "  \"$i\": $tps_excluding" >> $json_file
  else
    echo "  \"$i\": $tps_excluding," >> $json_file
  fi
done

average_tps_excluding=$(echo "$total_tps_excluding / $repeat_count" | bc -l)

echo "}" >> $json_file

