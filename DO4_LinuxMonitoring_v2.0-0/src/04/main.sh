#!/bin/bash

if [ $# -ne 0 ]; then
    echo "cкрипт должен быть запущен без аргументов"
    exit 1
fi


source shablon.sh

# Define the number of iterations
iterations=1000
echo Начинаем генерацию логов
# Loop through the number of log files to create
for i in {1..5}; do
    
    # Get a random number of entries for the day
    entries=$(((RANDOM % 900) + 100))
    seconds_to_add=$((RANDOM % 60))
    num=$((6 - $i))
    # Create the log file
    echo > "log_nginx_$num.log"
    # Define the starting time
    start_time="0$((RANDOM % 7)):$(((RANDOM % 50) + 10)):00"

    # Convert the starting time to a timestamp
    start_timestamp=$(date -d "$start_time" +%s)
    # Loop through the entries
    for j in $(seq 1 $entries); do
        # Get a random IP
        ip=$(((RANDOM % 255) + 1)).$(((RANDOM % 255) + 1)).$(((RANDOM % 255) + 1)).$(((RANDOM % 255) + 1))

        # Get a random code and method
        code=${codes[$((RANDOM % ${#codes[@]}))]}
        method=${methods[$((RANDOM % ${#methods[@]}))]}

        timestamp=$((start_timestamp + (j * seconds_to_add)))

        # Convert the timestamp to a date
        date1=$(date -d "@$timestamp" +"%T")
        # Get a random user agent
        agent=${user_agents[$((RANDOM % ${#user_agents[@]}))]}

        # Get a random date within the day
        date=$(date -d "$i days ago" +"%d/%b/%Y:$date1 %z")

        bytes=$(((RANDOM % 8999) + 1000))

        rand_page=${pages[$RANDOM % ${#pages[@]}]}

        # Write the entry to the log file
        echo "$ip - - [$date] \"$method https://edu.21-school.ru$rand_page HTTP/1.1\" $code $bytes \"-\" \"$agent\"" >>"log_nginx_$num.log"
        
    done
    echo создали лог: "log_nginx_$num.log"
done
echo Логи сгенерины, проверяй


# Коды ответа:
# 200 OK — успешный запрос. Если клиентом были запрошены какие-либо данные, то они находятся в заголовке и/или теле сообщения.
# 201 Created — в результате успешного выполнения запроса был создан новый ресурс.
# 400 Bad Request — сервер обнаружил в запросе клиента синтаксическую ошибку.
# 401 Unauthorized — для доступа к запрашиваемому ресурсу требуется аутентификация.
# 403 Forbidden — сервер понял запрос, но он отказывается его выполнять из-за ограничений в доступе для клиента к указанному ресурсу.
# 404 Not Found —  Сервер понял запрос, но не нашёл соответствующего ресурса по указанному URL. Основная причина — ошибка в написании адреса Web-страницы.
# 500 Internal Server Error — любая внутренняя ошибка сервера, которая не входит в рамки остальных ошибок класса.
# 501 Not Implemented — сервер не поддерживает возможностей, необходимых для обработки запроса.
# 502 Bad Gateway — сервер, выступая в роли шлюза или прокси-сервера, получил недействительное ответное сообщение от вышестоящего сервера.
# 503 Service Unavailable — сервер временно не имеет возможности обрабатывать запросы по техническим причинам (обслуживание, перегрузка и прочее).
