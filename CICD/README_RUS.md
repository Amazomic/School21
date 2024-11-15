# Basic CI/CD

Разработка простого **CI/CD** для проекта *SimpleBashUtils*. Сборка, тестирование, развертывание.


## Contents

1. [Chapter I](#chapter-i)
2. [Chapter II](#chapter-ii) \
   2.1. [Основы CI/CD](#основы-cicd) \
   2.2. [Основы CI](#основы-ci) \
   2.3. [Основы CD](#основы-cd)
3. [Chapter III](#chapter-iii) \
   3.1. [Настройка gitlab-runner](#part-1-настройка-gitlab-runner)  
   3.2. [Сборка](#part-2-сборка)  
   3.3. [Тест кодстайла](#part-3-тест-кодстайла)   
   3.4. [Интеграционные тесты](#part-4-интеграционные-тесты)  
   3.5. [Этап деплоя](#part-5-этап-деплоя)  
   3.6. [Дополнительно. Уведомления](#part-6-дополнительно-уведомления)
4. [Chapter IV](#chapter-iv)


## Chapter I

![basic_ci_cd](misc/images/basic_ci_cd.JPG)

Планета Земля, Соединённое Королевство Великобритании и Северной Ирландии, Лондон, Оксфорд-стрит, офис компании “ASI”, наши дни.

За несколько дней после прибытия в порт Лондона вы успели обустроиться и немного осмотреть город,
после чего наступает день, в который вы условились прибыть на новую работу.

Сегодня вы приезжаете в кэбе к дверям офиса компании, ради который вы и оказались на Альбионе.
В полученном в день прибытия письме, были указаны код от двери, и номер Вашего кабинета.
Удивляясь пустым коридорам и гробовой тишине, вы спускаетесь на несколько этажей вниз, где без проблем находите свое рабочее место.

Там вы обнаруживаете недавно включенный компьютер и аппарат внутренней связи в печальном состоянии.
Стоит вам зайти и закрыть за собой дверь, из него доносится роботизированный голос.

`-` Добро пожаловать в компьютеризированный экспериментальный центр при лаборатории ASI.

`-` Анализ особенностей вашего организма завершен. Мы готовы приступать.

`-` Вы будете заниматься сопровождением одного из проектов нашего экспериментального центра.

`-` Вашим первым заданием будет создание **CI/CD** для всем знакомых утилит **cat** и **grep**.

`-` Перед началом работы хотим вам напомнить, что хотя основным принципом экспериментального центра является обучение в игровой форме, мы не гарантируем отсутствие увечий и травм.

`-` Из соображений вашей безопасности и безопасности окружающих, воздержитесь дотрагиваться до *бзз* чего бы то ни было вообще.


## Chapter II

`-` Ваше первое задание требует некоторого объяснения. Позвольте я по-быстрому введу вас в курс дела.

*Вы смогли разобрать лишь самую базовую информацию из дальнейшей речи, ведь она была ускорена, по ощущениям, раз в 5*

### Основы **CI/CD**

УВЫ... Если что-то постоянно делается «в ручную» – это либо будет работать плохо, либо вовсе не будет работать.

**CI/CD** - Это набор принципов и практик, которые позволяют чаще и надежнее развертывать изменения программного обеспечения.

Причины применения **CI/CD**:
- Командная разработка
- Длинный жизненный цикл ПО
- Сокращение релизного цикла
- Сложность развертывания и тестирования крупных систем
- Человеческий фактор

**CI/CD** pipeline - Это последовательность действий (скриптов) для определенной версии кода в репозитории,
которая запускается автоматически при совершении изменений.

### Основы **CI**

**CI** (Continuous Integration) — в дословном переводе «непрерывная интеграция».
Имеется в виду интеграция отдельных кусочков кода приложения между собой.
**CI** обычно выполняет 2 задачи, описанные далее.

- BUILD
    - Проверяем, собирается ли вообще код
    - Готовим артефакты для следующих стадий

- TEST
    - Тесты кодстайла
    - Модульные тесты
    - Интеграционные тесты
    - Прочие тесты, которые у вас есть
    - Отчеты о тестах

### Основы **CD**

**CD** (Continuous Delivery) — это расширение непрерывной интеграции,
поскольку оно автоматически развертывает все изменения кода в тестовой и/или производственной среде после этапа сборки.
**CD** может выполнять задачи, описанные далее.

- PUBLISH (В случае применения докера для развёртывания)
    - Собираем образы контейнеров
    - Пушим образы туда, откуда их будем потом брать для развертывания
- UPDATE CONFIGS
    - Обновляем конфигурацию на машинах
- DEPLOY STAGING
    - Развертывание тестовой среды для ручных тестов, QA, и прочих не автоматизируемых проверок
    - Может запускаться как вручную, так и автоматически при успешном прохождении стадий CI
- DEPLOY PRODUCTION
    - Разворачиваем новую версию системы на "продакшн"
    - Этот этап желательно запускать вручную, а не автоматически
    - По желанию можно настроить только для определенной ветки репозитория (master, release и т.п.)


`-` Вот. Если у вас будут вопросы, прокрутите в голове на замедленной скорости то, что я сказала. Скоро вернусь.


## Chapter III

В качестве результата работы вы должны сохранить два дампа образов виртуальных машин, описанных далее. \
**p.s. Ни в коем случае не сохранять дампы в гит!**

### Part 1. Настройка **gitlab-runner**

`-` Раз вы решили заняться CI/CD, должно быть, вы очень, очень любите тестировать. Я тоже это люблю. Так что приступим.
Если вам потребуется какая-либо информация, рекомендую искать ответы в официальной документации.

**== Задание ==**

##### Поднять виртуальную машину *Ubuntu Server 20.04 LTS*
*Будьте готовы, что в конце проекта нужно будет сохранить дамп образа виртуальной машины*

##### Скачать и установить на виртуальную машину **gitlab-runner**

##### Запустить **gitlab-runner** и зарегистрировать его для использования в текущем проекте (*DO6_CICD*)
- Для регистрации понадобятся URL и токен, которые можно получить на страничке задания на платформе.

### Part 2. Сборка

`-` Предыдущее испытание было создано, чтобы повышать в людях уверенность в себе.
Теперь я подкорректировала тесты, сделав их более сложными и менее льстивыми.

**== Задание ==**

#### Написать этап для **CI** по сборке приложений из проекта *C2_SimpleBashScripts*:

##### В файле _gitlab-ci.yml_ добавить этап запуска сборки через мейк файл из проекта _C2_

##### Файлы, полученные после сборки (артефакты), сохранять в произвольную директорию со сроком хранения 30 дней.

### Part 3. Тест кодстайла

`-` Поздравляю, вы выполнили абсолютно бессмысленную задачу. Шучу. Она была нужна для перехода ко всем последующим.

**== Задание ==**

#### Написать этап для **CI**, который запускает скрипт кодстайла (*clang-format*):

##### Если кодстайл не прошел, то "зафейлить" пайплайн

##### В пайплайне отобразить вывод утилиты *clang-format*

### Part 4. Интеграционные тесты

`-` Отлично, тест на кодстайл написан. [ТИШЕ] Говорю с тобой тет-а-тет. Не говори ничего коллегам.
Между нами: ты справляешься очень хорошо. [ГРОМЧЕ] Переходим к написанию интеграционных тестов.

**== Задание ==**

#### Написать этап для **CI**, который запускает ваши интеграционные тесты из того же проекта:

##### Запускать этот этап автоматически только при условии, если сборка и тест кодстайла прошли успешно

##### Если тесты не прошли, то "зафейлить" пайплайн

##### В пайплайне отобразить вывод, что интеграционные тесты успешно прошли / провалились

### Part 5. Этап деплоя

`-` Для завершения этого задания вы должны перенести исполняемые файлы на другую виртуальную машину, которая будет играть роль продакшена. Удачи.

**== Задание ==**

##### Поднять вторую виртуальную машину *Ubuntu Server 20.04 LTS*

#### Написать этап для **CD**, который "разворачивает" проект на другой виртуальной машине:

##### Запускать этот этап вручную при условии, что все предыдущие этапы прошли успешно

##### Написать bash-скрипт, который при помощи **ssh** и **scp** копирует файлы, полученные после сборки (артефакты), в директорию */usr/local/bin* второй виртуальной машины
*Тут вам могут помочь знания, полученные в проекте DO2_LinuxNetwork*

- Будьте готовы объяснить по скрипту, как происходит перенос.

##### В файле _gitlab-ci.yml_ добавить этап запуска написанного скрипта

##### В случае ошибки "зафейлить" пайплайн

В результате вы должны получить готовые к работе приложения из проекта *C2_SimpleBashScripts* (s21_cat и s21_grep) на второй виртуальной машине.

##### Сохранить дампы образов виртуальных машин
**p.s. Ни в коем случае не сохранять дампы в гит!**
- Не забудьте запустить пайплайн с последним коммитом в репозитории.

### Part 6. Дополнительно. Уведомления

`-` Здесь написано, что ваше следующее задание выполняется специально для нобелевских лауреатов.
Здесь не сказано, за что они получили премию, но точно не за умение работать с **gitlab-runner**.

**== Задание ==**

##### Настроить уведомления о успешном/неуспешном выполнении пайплайна через бота с именем "[ваш nickname] DO6 CI/CD" в *Telegram*

- Текст уведомления должен содержать информацию об успешности прохождения как этапа **CI**, так и этапа **CD**.
- В остальном текст уведомления может быть произвольным.


## Chapter IV

`-` Хорошо. По завершении серии заданий сотруднику следует пройти в комнату отдыха.

Пока у вас выдалась свободная минута в комнате отдыха вы, попутно размышляя о странности происходящего, решаете проверить почту.

Не успели вы достать телефон, как в комнату отдыха зашёл ещё один человек.

`-` Привет, что-то я тебя тут раньше не видел.

`-` Было бы странно, если бы видел. Я тут первый день, хах.

`-` О, первый день! Ну и как тебе наш "босс"? - последние слова прозвучали с явной усмешкой

`-` Это был босс? Фух, всё-таки он не одному мне кажется странным... и слегка грубым? Я уж думал, что вы все в Англии такие.

`-` Ахах, ни в коем случае, друг. Это просто розыгрыш над новичками, но не волнуйся завтра всё будет в порядке. Кстати говоря, вон идет и настоящий босс, кажется к тебе. Ну удачи, свидимся еще.

Незнакомец быстро пропал и в комнату зашел невысокий мужчина в дорогом костюме, с небольшой залысиной, навскидку лет этак 50-60-ти. Не дожидаясь ваших слов, он с тонкой, едва уловимой, улыбкой произнес:

`-` Ах, Томас, видимо это вы. Поистине великолепное выполнение тестовой работы. Надеюсь вы не испугались нашей милой подруги ASI младшей, она высоко отозвалась о вас. Итак, позвольте мне подробнее рассказать чем мы тут вообще занимаемся и какова ваша роль в нашей компании...


💡 [Нажми тут](https://forms.yandex.ru/u/6357f54d3e9d0836923d1203/), **чтобы поделиться с нами обратной связью на этот проек**т. Это анонимно и поможет команде Педаго сделать твоё обучение лучше.
