# 3DViewer v2.1

Разработать программу 3DViewer v2.1


## Contents

1. [Chapter I](#chapter-i) \
   1.1. [Introduction](#introduction)
2. [Chapter II](#chapter-ii) \
   2.1. [Information](#information)
3. [Chapter III](#chapter-iii) \
   3.1. [Part 1](#part-1-3dviewer-v21) \
   3.2. [Part 2](#part-2-дополнительно-настройки) \
   3.3. [Part 3](#part-3-дополнительно-запись) \
   3.4. [Part 4](#part-4-дополнительно-текстурирование)


## Chapter I

![3dviewer2.1](misc/images/3DViewer_v2.1.PNG)

*-- О, да. Я еще помню как мы тогда создали первое сгенерированное компьютерной графикой изображение, которое выглядело в точности как настоящий физический Фольксваген Жук! Однако, самым сложным здесь было далеко не придание реалистичности компьютерной модели, нет. С этим алгоритм Буя справился прекрасно. Самым сложным здесь было создать саму модель.* \
*Нам повезло, у жены Ивана, Марши, был прекрасный белый Жук 71 года выпуска. И вот, перед нами четырьмя стоит задача: полностью измерить каждый миллиметр этой иконы глобальной культуры своего времени и перенести его в компьютер. Никаких этих современных сканеров, камер не было. И знаешь что мы использовали?*

*-- Боюсь представить.*

*-- Обычные линейки. Размечали точки и полигоны при помощи линеек на самой машине. Благо Жук симметричен и нам хватило разметить только его половину. Мы с Джимом, будучи повыше, занимались крышей, багажником, капотом. А Буй с Рафаэлем отвечали за нижние части машины. Это был долгий и кропотливый процесс. А Марша все это время каталась по городу с нашей разметкой, представляешь?*

*-- Забавно.*

*-- Да. Но даже после столь долгих мучений, при переносе полученных точек в компьютер и объединении разных частей машины все равно нашлись несостыковки, пробелы, ошибки. Еще бы, когда работаешь с простой линейкой на столь сложном объекте этого следовало ожидать.*

*-- То есть еще время ушло на то, чтобы совместить все на компьютере?*

*-- Несколько месяцев на самом деле. Вышло даже так, что создание изображения Жука оказалось дороже самой машины. Но..*

Для продолжения просмотра видеоролика необходимо просмотреть 5-ти минутную рекламу. Продолжить?

## Introduction

В рамках данного проекта вам предстоит произвести модификацию приложения, разрабатываемого в проекте 3D Viewer v2.0. Новая версия должна осуществлять визуализацию трехмерного объекта не только в каркасном виде, но и с закраской граней.


## Chapter II

## Information

### Историческая справка

В 1960-ом году Уильям Феттер, инженер из компании Boeing, проектирующий самолеты впервые ввел термин "компьютерная графика" для обозначения своей деятельности в технической документации. Деятельность заключалась в создании графических изображений с применением компьютера, а уже в 1964 Уильям создал первую общеизвестную каркасную модель человека, которую позже в 70-х использовали в телерекламе.

В 1971 году Анри Гуро, в 1972 году Джим Блинн, в 1973 году Буй Туонг Фонг разработали модели закраски плоских граней каркасной модели - полигонов. Эти алгоритмы позволили не ограничиваться только каркасными изображениями трехмерных объектов, но и осуществлять их более приближенную к реальности визуализацию "твердых объектов" (solid modeling). Однако эти подходы естественным образом привели к новым задачам, требующим решений. Основной проблемой является задача удаления невидимых поверхностей при проецировании трехмерной сцены на плоскость экрана. Закрашенные объекты в большинстве случаев не являются прозрачными, поэтому закрашиваться грани полигональных моделей должны согласно их видимости относительно точки обзора. То есть при визуализации трехмерной модели, невидимые полигоны не должны быть в конечном итоге видны. Эта задача не является столь тривиальной и для нее было предложено огромное количество решений. Можно выделить два основных подхода, использующих действительно различную концепцию:

- *Z-buffer*, впервые описанный в 1974 году Вольфгангом Штрассером подход, основной идей которого является буферизация глубины каждого пикселя.

- *Ray casting*, впервые описанный в 1982 году, вдохновленный реальным процессом синтеза изображения в глазу человека и использующем понятие лучей.

### Визуализация "твердых тел"

Упрощенный пайплайн визуализации твердых тел включает в себя два этапа:

- Получение двумерных проекций полигонов трехмерного объекта.

- Попиксельная отрисовка интенсивностей согласно видимым или неперекрываемым полигонам объектов.

Помимо интенсивности, поверхность каждого объекта еще может обладать целым набором визуальных свойств, которые требуют дополнительных вычислений. Например, далее может производиться наложение текстур на поверхность объекта согласно его UV-развертке.


## Chapter III

## Part 1. 3DViewer v2.1

Разработать программу для визуализации модели в трехмерном пространстве.

- Программа должна быть разработана на языке C++ стандарта C++17
- Код программы должен находиться в папке src
- При написании кода необходимо придерживаться Google Style
- Сборка программы должна быть настроена с помощью Makefile со стандартным набором целей для GNU-программ: all, install, uninstall, clean, dvi, dist, tests. Установка должна вестись в любой другой произвольный каталог
- Программа должна быть разработана в соответствии с принципами объектно-ориентированного программирования, структурный подход запрещен
- Должно быть обеспечено полное покрытие unit-тестами модулей, связанных с загрузкой моделей и аффинными преобразованиями
- В один момент времени должна быть только одна модель на экране.
- Программа должна предоставлять возможность:
    - Загружать модель из файла формата obj (поддержка списка вершин, поверхностей и нормалей).
    - Перемещать модель на заданное расстояние относительно осей X, Y, Z.
    - Поворачивать модель на заданный угол относительно своих осей X, Y, Z.
    - Масштабировать модель на заданное значение.
    - Переключать тип отображения объекта: каркасная модель, плоское затенение, мягкое затенение (методом Гуро или методом Фонга).
    - Задавать источник освещения, его базовую интенсивность (через три компоненты: R, G, B) и положение.
- В программе должен быть реализован графический пользовательский интерфейс, на базе любой GUI-библиотеки с API для C++  
  * Для Linux: GTK+, CEF, Qt, JUCE
  * Для Mac: GTK+, CEF, Qt, JUCE, SFML, Nanogui, Nngui
- Графический пользовательский интерфейс должен содержать:
    - Кнопку для выбора файла с моделью и поле для вывода его названия.
    - Зону визуализации модели.
    - Кнопку/кнопки и поля ввода для перемещения модели.
    - Кнопку/кнопки и поля ввода для поворота модели.
    - Кнопку/кнопки и поля ввода для масштабирования модели.
    - Информацию о загруженной модели - название файла, кол-во вершин и ребер.
- Программа должна корректно обрабатывать и позволять пользователю просматривать модели с деталями до 100, 1000, 10 000, 100 000, 1 000 000 вершин без зависания (зависание - это бездействие интерфейса более 0,5 секунды).
- Программа должна быть реализована с использованием паттерна MVC, то есть:
    - не должно быть кода бизнес-логики в коде представлений
    - не должно быть кода интерфейса в контроллере и в модели
    - контроллеры должны быть тонкими
- Необходимо использовать минимум три различных паттерна проектирования (например, фасад, стратегия и команда)
- Классы должны быть реализованы внутри пространства имен `s21`
- Для осуществления аффинных преобразований могут использоваться матрицы из библиотеки из предыдущего проекта s21_matrix+

## Part 2. Дополнительно. Настройки

- Программа должна позволять настраивать тип проекции (параллельная и центральная)
- Программа должна позволять настраивать тип (сплошная, пунктирная), цвет и толщину ребер, способ отображения (отсутствует, круг, квадрат), цвет и размер вершин
- Программа должна позволять выбирать цвет фона
- Программа должна позволять выбирать базовый цвет объекта
- Настройки должны сохраняться между перезапусками программы

## Part 3. Дополнительно. Запись

- Программа должна позволять сохранять полученные ("отрендеренные") изображения в файл в форматах bmp и jpeg
- Программа должна позволять по специальной кнопке записывать небольшие "скринкасты" - текущие пользовательские аффинные преобразования загруженного объекта в gif-анимацию (640x480, 10fps, 5s)
- Программа должна позволять по специальной кнопке сохранять короткое превью модели - gif-анимацию (640x480, 10fps, 5s) с вращением объекта вокруг одной оси.

## Part 4. Дополнительно. Текстурирование

- Программа должна позволять наносить текстуру на объект (texture mapping), имеющий uv-развертку (uv-map)
- При рендеринге полигоны модели должны быть закрашены в соответствии с uv-преобразованиями
- При загрузке модели с uv-разметкой, должна стать доступной кнопка "Нанести текстуру", которая позволяет выбрать файл текстуры
- Разрешение файла текстуры не может превышать 1024x1024 пикселей
- Файл текстуры имеет расширение BMP
- Текстура может быть только квадратной, координаты всегда нормируются к значениям от 0 до 1
- Текстура не должна отображаться в режиме каркасной модели
- После загрузки текстуры в интерфейсе программы должна стать доступной кнопка "Снять текстуру", после которой текстура выгружается из памяти программы и перестает отображаться на модели
- UV-карту модели считывать из obj-файла
- Программа должна позволять сохранять UV-карту модели поверх выбранной текстуры в отдельный BMP-файл; на текстуре должны быть отображены указанным цветом ребра развертки модели

💡 [Нажми тут](https://forms.yandex.ru/cloud/64181b936938722405a1286e/), **чтобы поделиться с нами обратной связью на этот проект**. Это анонимно и поможет команде Педаго сделать твоё обучение лучше.
