## Part 1. Использование готового манифеста

**== Задание ==**

1) Запустить окружение Kubernetes с памятью 4GB 

 ![screen](img/1.png)


2) Применить манифест из директории `/src/example` к созданному окружению Kubernetes

 ![screen](img/2.png)


3) Запустить стандартную панель управления Kubernetes с помощью команды `minikube dashboard`

 ![screen](img/3.png)
 ![screen](img/4.png)


4) Прокинуть тунели для доступа к развернутым сервиса с помощью команды `minikube services`

 ![screen](img/5.png)


5) Удостовериться в работоспособности развернутого приложения, открыв в браузере страницу приложения (сервис apache)


 ![screen](img/6.png)
 ![screen](img/7.png)



## Part 2. Написание собственного манифеста

**== Задание ==**

1) Написать собственные yml-файлы манифестов для приложения из первого проекта (`/src/services`), реализующие следующее:
   - карту конфигурации со значениями хостов БД и сервисов
   - секреты с паролем и логином к БД и ключами межсервисной авторизации (их можно найти в файлах `application.properties`)
   - поды и сервисы для всех модулей приложения: postgres, rabbitmq и 7 сервисов приложения. Для всех сервисов использовать единственную реплику.

    ![screen](img/9.png)

*  Сами конфигурации можно просмотреть в папке k8s
   
2) Запустить приложение путем последовательного применения манифестов командой `kubectl apply -f <манифест>.yaml`.

    ![screen](img/8.png)

3) Проверить статус созданных объектов (секреты, конфигурационная карта, поды и сервисы) в кластере с помощью команд `kubectl get <тип_объекта> <имя_объекта>` и `kubectl describe <тип_объекта> <имя_объекта>`. Результат отобразить в отчете.

    ![screen](img/10.png)

    ![screen](img/11.png)

    ![screen](img/12.png)

    ![screen](img/13.png)

    ![screen](img/14.png)

    ![screen](img/15.png)

    ![screen](img/16.png)

    ![screen](img/17.png)

    ![screen](img/18.png)

    ![screen](img/19.png)

    ![screen](img/20.png)

    ![screen](img/21.png)    

    ![screen](img/22.png)

    ![screen](img/23.png)

    ![screen](img/24.png)

    ![screen](img/25.png)

    ![screen](img/26.png)

    ![screen](img/27.png)

    ![screen](img/28.png)

    ![screen](img/29.png)

    ![screen](img/30.png)

    ![screen](img/31.png)

    ![screen](img/32.png)


4) Проверить наличие правильных значений секретов, применив, например, команду: `kubectl get secret my-secret -o jsonpath='{.data.password}' | base64 --decode` для декодирования секрета.

    ![screen](img/33.png)       


5) Проверить логи приложения, запущенного в кластере командой `kubectl logs <имя_контейнера>`. Скриншот отобразить в отчете.

    ![screen](img/34.png)

    ![screen](img/35.png)

    ![screen](img/36.png)

    ![screen](img/37.png)

    ![screen](img/38.png)

    ![screen](img/39.png)    

    ![screen](img/40.png)

    ![screen](img/41.png)

    ![screen](img/42.png)

    ![screen](img/43.png)

    ![screen](img/44.png)

    ![screen](img/45.png)

    ![screen](img/46.png)

    ![screen](img/47.png)

    ![screen](img/48.png)

    ![screen](img/49.png)

    ![screen](img/50.png)

    ![screen](img/51.png)

    ![screen](img/52.png)

    ![screen](img/53.png)

    ![screen](img/54.png)

    ![screen](img/55.png)


6) Прокинуть тунели для доступа к gateway service и session service.

    ![screen](img/56.png)

    ![screen](img/57.png)


7) Запустить функциональные тесты postman и удостовериться в работоспособности приложения.

    ![screen](img/58.png)


8) Запустить стандартную панель управления Kubernetes с помощью команды `minikube dashboard`. Отобразить в отчете следующую информацию в виде скриншотов с дашборда: текущее состояние узлов кластера, список запущенных Pod, а также другие метрики, такие как загрузка ЦП и память, логи Pod, конфигурации и секреты.
   
   
    ![screen](img/62.png)
    ![screen](img/59.png)
    ![screen](img/60.png)
    ![screen](img/61.png)
    ![screen](img/63.png)


9) Обновить приложение (добавив новую зависимость в pom-файл), и пересобрать приложение со следующими стратегиями развертывания (замерить время переразвертывания приложения для каждого случая и отметить результаты в отчете):

*  обычный без доп зависсимостей

    ![screen](img/64.png)

   - пересоздание (recreate)

    ![screen](img/65.png)
    ![screen](img/66.png)


   - последовательное обновление (rolling) 

    ![screen](img/67.png)
    ![screen](img/68.png)


*  с зависимостями в pom.xml из DevOps_9

    ![screen](img/69.png)

   - пересоздание (recreate)

    ![screen](img/70.png)


   - последовательное обновление (rolling) 

    ![screen](img/71.png)



