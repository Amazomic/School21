if [ "$#" -ne 3 ]; then
  echo "Error: 3 arguments are required"
  exit 1
fi

if [[ ! $3 =~ ^[0-9]+[M][b]$ ]]; then
  echo "третий аргумент должен быть от 1 до 100Mb например 50Mb"
  exit 1
fi

if [ ${#folder_letters} -gt 7 ]; then
  echo "Error: букоф для папок cannot be more than 7 characters"
  exit 1
fi

if [ ${#name} -gt 7 ]; then
  echo "Error: имя файла  cannot be more than 7 characters"
  exit 1
fi

if [ ${#rashirenie} -gt 3 ]; then
  echo "Error: расширение cannot be more than 3 characters"
  exit 1
fi

if [[ $file_size -gt 100 ]]; then
  echo "не больше 100 Mb"
  exit 1
fi

# check if there is at least 1GB of free space
free_space=$(df -h / | tail -1 | awk '{print $4}')
if [[ "$free_space" < "1.0G" ]]; then
  echo "и так места мало"
  exit 1
fi

if ! [[ $1 =~ ^[a-zA-Z]+$ ]]; then
  echo "имена папок только из пендосовских букв"
  exit 1
fi

if ! [[ $name =~ ^[a-zA-Z]+$ ]]; then
  echo "имя файла только из пендосовских букв"
  exit 1
fi

if ! [[ $rashirenie =~ ^[a-zA-Z]+$ ]]; then
  echo "расширений только из пендосовских букв"
  exit 1
fi

while [ ${#folder_letters} -lt 5 ]; do
  folder_letters+=${folder_letters: -1}
done

while [ ${#name} -lt 5 ]; do
  name+=${name: -1}
done