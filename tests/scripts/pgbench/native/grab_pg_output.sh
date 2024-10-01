#!/bin/bash

param=$1
repeat_count=$2

total_tps_excluding=0

for ((i = 1; i <= repeat_count; i++)); do
  echo "Running iteration $i of $repeat_count..."
  
  ./run_pg_nat.sh "$param" &> pg.log

  tps_excluding=$(grep "tps =" pg.log | grep "excluding" | awk '{print $3}')
  
  total_tps_excluding=$(echo "$total_tps_excluding + $tps_excluding" | bc)
  
  echo "TPS (excluding) for iteration $i: $tps_excluding"
done

average_tps_excluding=$(echo "$total_tps_excluding / $repeat_count" | bc -l)

echo "Average TPS (excluding): $average_tps_excluding"

