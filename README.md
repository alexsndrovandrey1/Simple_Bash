# Simple_Bash

## Разработка утилит Bash по работе с текстом: cat, grep.

### cat - используется для чтения, объединения и вывода содержимого файлов. Она принимает один или несколько файлов в качестве аргументов и выводит их содержимое на стандартный вывод.

* -b или --number-nonblank: Нумерует только непустые строки в выводе.
* -e или -E: Добавляет символ $ в конце каждой строки, чтобы указать конец строки.
* -n или --number: Нумерует все строки в выводе.
* -s или --squeeze-blank: Сжимает пустые строки в выводе, оставляя только одну пустую строку между блоками текста.
* -t: Отображает символы табуляции в виде ^I.
* -T: Табуляция не распознается, а отображается как символ.
* -v: Отображает непечатаемые символы в виде их экранированных последовательностей.

### grep - используется для поиска текста в файлах или потоках данных с использованием регулярных выражений. Она принимает шаблон поиска и один или несколько файлов в качестве аргументов и выводит строки, содержащие совпадения с шаблоном.

* -e pattern: Указывает шаблон поиска.
* -i: Игнорирует регистр символов при поиске.
* -v: Выводит строки, которые НЕ совпадают с шаблоном.
* -c: Выводит только количество строк, совпадающих с шаблоном.
* -l: Выводит только имена файлов, содержащих совпадения.
* -n: Выводит номера строк, содержащих совпадения.
* -h: Предотвращает вывод имен файлов при поиске в нескольких файлах.
* -s: Подавляет сообщения об ошибках.
* -f file: Читает шаблоны из файла вместо командной строки.
* -o: Выводит только совпадающую часть строки

## Запуск

1. Для запуска потребуется компилятор gcc, установить его можно следующим образом:

> Для ОС Windows:
`https://gcc.gnu.org/releases.html`

> Для ОС Linux:
`sudo apt install gcc`

2. Склонируйте репозиторий в удобную для вас диреторию

3. Для демонстрации работы cat:
    
    3.1. Перейдите в `/src/cat`

    3.2. И в терминале пишем: `make` или `make build`.

    3.3. Для тестирования используется `tets_cat.sh` - это скрипт, который прогонит 3 тестовых файла со всеми комбинациями флагов. И для этого нужно написать `make test`

4. Дополнительные возможности Makefile
   
   - `make verter` - команда для запуска автоматизированной проверки проекта (build and format). Для этого нужен Docker
   
   - `make check` - команда для проверки формата (выдаст ошибки по google формату если есть).
   
   - `make format` - команда для исправления ошибок формата (исправит файлы).
   
   - `make clean` - удаление исполняемых файлов.

5.  Для демонстрации работы grep:
    
    5.1. Перейдите в `/src/grep`

    5.2. И в терминале пишем: `make` или `make build`.

    5.3. Для тестирования используется `diff_grep.sh` - это скрипт, который прогонит тестовые файлы со всеми комбинациями флагов. И для этого нужно написать `make test`

6. Дополнительные возможности Makefile
   
   - `make verter` - команда для запуска автоматизированной проверки проекта (build and format). Для этого нужен Docker
   
   - `make check` - команда для проверки формата (выдаст ошибки по google формату если есть).
   
   - `make format` - команда для исправления ошибок формата (исправит файлы).
   
   - `make clean` - удаление исполняемых файлов.
