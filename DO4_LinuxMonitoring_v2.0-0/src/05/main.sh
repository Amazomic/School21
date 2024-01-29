#!/bin/bash


source proverki.sh

# Depending on the parameter value, perform different actions
case "$1" in
    1)
        awk '{print $0}' $log_file | sort -k9
        ;;
    2)
        awk '{print $1}' $log_file | sort | uniq 
        ;;
    3)
        awk '($9 ~ /^4/ || $9 ~ /^5/) {print $0}' $log_file 
        ;;
    4)
        awk '($9 ~ /^4/ || $9 ~ /^5/) {print $1}' $log_file | sort | uniq 
        ;;
    *)
        echo "должно быть число от 1 до 4"
        exit 1
        ;;
esac

echo
echo Я знаю что этого нет в задании но может хочешь что бы я перенаправил вывод в файл что бы ты мог нормально проверить?[y/n]
read choice

if [ "$choice" == "y" ]; then
case "$1" in
    1)
        awk '{print $0}' $log_file | sort -k9 > log.log
        ;;
    2)
        awk '{print $1}' $log_file | sort | uniq > log.log
        ;;
    3)
        awk '($9 ~ /^4/ || $9 ~ /^5/) {print $0}' $log_file > log.log
        ;;
    4)
        awk '($9 ~ /^4/ || $9 ~ /^5/) {print $1}' $log_file | sort | uniq > log.log
        ;;
    *)
        echo "Invalid parameter. Please provide a valid parameter (1, 2, 3, or 4)."
        ;;
esac
else
exit 1
fi