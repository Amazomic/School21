#!/bin/bash

path=$1
num_folders=$2
folder_letters=$3
num_files=$4
file_letters=$5
file_size=${6%kb}
date=$(date +"%d%m%y")
name=$(echo $5 | cut -d "." -f1)
rashirenie=$(echo $5 | cut -d "." -f2)

source proverki.sh

# create the main directory
mkdir -p $path
echo $path $(date +"%Y-%m-%d %T") >log.txt
echo засераем тут $path

pristavka=""
# create the nested folders
for ((i = 1; i <= num_folders; i++)); do

  last_char=${3: -1} #takes last char
  dir_name+=$last_char

 if [ $((i % 150)) -eq 0 ]; then
      pristavka+=${3:0:1}
      dir_name=""
    fi

  mkdir -p $path/$pristavka$folder_letters$dir_name"_"$date
 
  echo $path/$pristavka$folder_letters$dir_name"_"$date $(date +"%Y-%m-%d %T") >> log.txt
  echo создали папку $path/$pristavka$folder_letters$dir_name"_"$date

  last_name=""
  rashirenie=$(echo $5 | cut -d "." -f2)
  last_char_ext=""
  pristavka_file=""
  # create the files in each folder
  for ((j = 1; j < num_files; j++)); do

    last_name+=${name: -1} #takes last char
    name_gen=$name$last_name

    last_char=${file_name: -1} #takes last char
    file_name+=$last_char

    if [ $((j % 10000)) -eq 0 ]; then
      pristavka_file=${name:0:1}
      rashirenie+=${rashirenie: -1}
      last_name=${name: -1}
    fi

     if [ $((j % 100)) -eq 0 ]; then
      last_name=${name: -1}
      name_gen=$name$last_name
      last_char=${file_name: -1}
      pristavka_file+=${name:0:1}
      file_name+=$last_char
    fi

    dd if=/dev/zero of=$path/$pristavka$folder_letters$dir_name"_"$date/$pristavka_file$name_gen"_"$date.$rashirenie bs=1024 count=$file_size &>/dev/null

    echo $path/$pristavka$folder_letters$dir_name"_"$date/$pristavka_file$name_gen"_"$date.$rashirenie $(date +"%Y-%m-%d %T") $6 >>log.txt
    echo создали файл $path/$pristavka$folder_letters$dir_name"_"$date/$pristavka_file$name_gen"_"$date.$rashirenie

    # check if there is at least 1GB of free space
  free_space=$(df -h / | tail -1 | awk '{print $4}')
    if [[ "$free_space" < "1.0G" ]]; then
      echo "осталось меньше гига"
      exit 1
    fi

  done

done