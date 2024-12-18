3DViewer v2.1

Программа 3DViewer представляет собой приложение для визуализации каркасных моделей
в трехмерном пространстве. Она разработана на языке C с использованием стандарта C17
и компилятора GCC. Программа использует дополнительные библиотеки и модули QT
для реализации графического пользовательского интерфейса. В программе может быть одновременно открыта только одна модель.

Графический пользовательский интерфейс программы состоит из следующих элементов:

 Кнопка для выбора файла с моделью и поле для вывода названия выбранного файла.
 Зона визуализации каркасной модели, где отображается сама модель.
 Кнопки и поля ввода для перемещения модели по осям X, Y, Z.
 Кнопки и поля ввода для поворота модели вокруг осей X, Y, Z.
 Кнопки и поля ввода для масштабирования модели.
 Информация о загруженной модели, включающая название файла, количество вершин и ребер модели.
 Приложение позволяет загружать каркасные модели из файлов формата obj, перемещать модель по осям X, Y, Z, вращать модель вокруг осей X, Y, Z и масштабировать модель на заданное значение.
 3DViewer предоставляет удобный интерфейс для визуализации и манипуляции с 3D-моделями, позволяя пользователям исследовать и взаимодействовать с моделями в трехмерном пространстве.
 Возможность сохранять скриншот модели и gif 

Команды установки и удаления:

- Установка 3DViewer: make install
- Удаление 3DViewer: make uninstall

Дополнительные команды:

- Проверка стиля кода: make clang
- Запуск тестов: make test
- Отчет Gcov: make gcov
- Проверка на утечки: make leak
- Создание архива: make dist
- Очистка дистрибутива, удаление ненужных файлов: make clean

