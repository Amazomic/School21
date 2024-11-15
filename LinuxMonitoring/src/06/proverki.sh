logpath="../04/log_nginx_1.log ../04/log_nginx_2.log ../04/log_nginx_3.log ../04/log_nginx_4.log ../04/log_nginx_5.log"

if [ $# -ne 0 ]; then
echo "Скрипт работает без параметров"
exit 1
fi

for file in $logpath; do
if [ ! -f $file ]; then
echo "похоже вы не сгенерили логи из 4го задания"
exit 1
fi
done


if ! [ -x "$(command -v goaccess)" ]; then
  echo "GoAccess не установлен, погади немного ща всё будет"
  sudo apt-get install goaccess  &>/dev/null
  echo "всё установили поехали дальше"
fi