if [ $# -ne 1 ]; then
echo "Скрипт работает с одним параметром"
exit 1
fi

# Get the log file path
log_file="../04/log_nginx_1.log ../04/log_nginx_2.log ../04/log_nginx_3.log ../04/log_nginx_4.log ../04/log_nginx_5.log"

for file in $log_file; do
if [ ! -f $file ]; then
echo "похоже вы не сгенерили логи из 4го задания"
exit 1
fi
done