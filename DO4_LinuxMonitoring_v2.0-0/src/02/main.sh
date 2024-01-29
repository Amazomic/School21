#!/bin/bash

folder_letters=$1
file_letters=$2
file_size=${3%Mb}
date=$(date +"%d%m%y")
name=$(echo $2 | cut -d "." -f1)
rashirenie=$(echo $2 | cut -d "." -f2)

source proverki.sh

touch log.txt

num_iterations=$(((RANDOM % 100) + 1))

# create the nested folders
for ((i = 1; i <= $num_iterations; i++)); do

  path=$(sudo bash -c "find / -maxdepth 2 -type d -writable 2>/dev/null | sort -R | tail -1 | grep -v -e "bin" -e "sbin" -e "run" -e "sys"")

  last_char=${1: -1} #takes last char
  dir_name+=$last_char

  if [ $((i % 200)) -eq 0 ]; then
    dir_name=""
    folder_letters+=${3:0:1}
  fi

  sudo mkdir -p $path/$folder_letters$dir_name"_"$date &>/dev/null

  if test -d "$path/$folder_letters$dir_name"_"$date"; then
    echo $path/$folder_letters$dir_name"_"$date/ $(date +"%Y-%m-%d %T") >>log.txt
    echo создали папку $path/$folder_letters$dir_name"_"$date/
  fi

  last_name=""
  rashirenie=$(echo $2 | cut -d "." -f2)
  last_char_ext=""
  pristavka_file""
  # create the files in each folder
  for ((j = 1; j <= RANDOM; j++)); do

    last_name+=${name: -1} #takes last char
    name_gen=$name$last_name

    last_char=${file_name: -1} #takes last char
    file_name+=$last_char

    if [ $((j % 10000)) -eq 0 ]; then
      rashirenie+=${rashirenie: -1}

    fi

    if [ $((j % 100)) -eq 0 ]; then
      last_name=${name: -1}
      name_gen=$name$last_name
      last_char=${file_name: -1}
      pristavka_file+=${name:0:1}
      file_name+=$last_char
    fi

    sudo dd if=/dev/zero of=$path/$folder_letters$dir_name"_"$date/$pristavka_file$name_gen"_"$date.$rashirenie bs=1M count=$file_size &>/dev/null

    echo $path/$folder_letters$dir_name"_"$date/$pristavka_file$name_gen"_"$date.$rashirenie $(date +"%Y-%m-%d %T") $3 >>log.txt
    echo создали файл $path/$folder_letters$dir_name"_"$date/$pristavka_file$name_gen"_"$date.$rashirenie

    # check if there is at least 1GB of free space
    free_space=$(df -h / | tail -1 | awk '{print $4}')
    if [[ "$free_space" < "1.0G" ]]; then
      echo "осталось меньше гига"
      exit 1
    fi

  done
done
