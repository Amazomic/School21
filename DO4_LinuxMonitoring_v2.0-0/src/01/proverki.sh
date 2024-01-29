if [ "$#" -ne 6 ]; then
  echo "должно быть 6 аргументов для корректной работы скрипта"
  exit 1
fi

if [[ ! $6 =~ ^[0-9]+[k][b]$ ]]; then
  echo "размер файлов должен быть записан именно в таком формате: 3kb, 23kb"
  exit 1
fi

if [ ${#folder_letters} -gt 7 ]; then
  echo "Error: букоф для папок cannot be more than 7 characters"
  exit 1
fi

if ! [[ $2 =~ ^[0-9]+$ ]]; then
  echo "количество папок только числами"
  exit 1
fi

if ! [[ $4 =~ ^[0-9]+$ ]]; then
  echo "количество файлов только числами"
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
  echo "не больше 100 кб"
  exit 1
fi

if ! [[ $3 =~ ^[a-zA-Z]+$ ]]; then
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

# check if there is at least 1GB of free space
  free_space=$(df -h / | tail -1 | awk '{print $4}')
    if [[ "$free_space" < "1.0G" ]]; then
      echo "осталось меньше гига"
      exit 1
    fi

while [ ${#folder_letters} -lt 3 ]; do
  folder_letters+=${folder_letters: -1}
done

while [ ${#name} -lt 3 ]; do
  name+=${name: -1}
done