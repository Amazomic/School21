#!/bin/bash

source proverka.sh

while true; do
  # Get CPU usage
  cpu=$(top -bn1 | awk '/Cpu\(s\)/ {print $2}')

  # Get memory usage
  mem_total=$(grep MemTotal /proc/meminfo | awk '{print $2}')
  mem_free=$(grep MemFree /proc/meminfo | awk '{print $2}')
  mem_used=$((mem_total-mem_free))

  # Get disk usage
  disk_total=$(df / | awk '/\// {print $2}')
  disk_free=$(df / | awk '/\// {print $4}')
  disk_used=$((disk_total-disk_free))

  # Write data to HTML file in Prometheus format
  echo "# HELP system_cpu_usage CPU usage in percent" > metrics
  echo "# TYPE system_cpu_usage gauge" >> metrics
  echo "system_cpu_usage $cpu" >> metrics
  echo "" >> metrics

  echo "# HELP system_memory_usage Memory usage in KB" >> metrics
  echo "# TYPE system_memory_usage gauge" >> metrics
  echo "system_memory_usage{type=\"used\"} $mem_used" >> metrics
  echo "system_memory_usage{type=\"free\"} $mem_free" >> metrics
  echo "" >> metrics

  echo "# HELP system_disk_usage Disk usage in KB" >> metrics
  echo "# TYPE system_disk_usage gauge" >> metrics
  echo "system_disk_usage{type=\"used\"} $disk_used" >> metrics
  echo "system_disk_usage{type=\"free\"} $disk_free" >> metrics

  # Sleep for 3 seconds before updating metrics again
  sleep 3
done
