#!/bin/bash

source proverka.sh

# Set the cleaning method based on the first parameter
cleaning_method=$1

# Perform the cleaning based on the method selected

if [ $cleaning_method -eq 1 ]; then

  file_names=$(awk '{print $1}' ../02/log.txt | grep "^\/.*\/$")
  count=0
  while read -r line; do

    if test -d $line; then
      rm -rf "$line"
      echo удалили папку с файлами "$line"
    else
      ((count++))
    fi
  done <<<"$file_names"
  if [[ count -gt 0 ]]; then
    echo "созданные папки удалены, остальные уже не существуют"
  fi

elif [ $cleaning_method -eq 2 ]; then
  # Clean based on date and time of creation
  echo "Enter start time (YYYY-MM-DD HH:MM):"
  read start_time
  echo "Enter end time (YYYY-MM-DD HH:MM):"
  read end_time

  files_for_delete=$(find / -type f -newermt "$start_time" ! -newermt "$end_time" 2>/dev/null)

  file_names=$(awk '{print $1}' ../02/log.txt)

  for file in $files_for_delete; do
    for name in $file_names; do
      if [[ $file == $name ]]; then
        echo "Удалили файл: $file"
        rm -v $file &>/dev/null
      fi
    done
  done

elif
  [ $cleaning_method -eq 3 ]
then
  # Clean based on name mask
  echo "Enter name mask (example: asdff_240123):"
  read name_mask

  file_names=$(awk '{print $1}' ../02/log.txt | grep $name_mask)
  echo "Удалили файлы: $file_names"
  rm -rf $file_names
else
echo нужно ввести число от 1 до 3
exit 1
fi