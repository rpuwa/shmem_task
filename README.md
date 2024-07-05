# shmem_task

## Описание решения
- Программа предназначена для запуска на Linux.
- Код написан на языке С++ с использованием библиотек языка С.
- Запуск программ производился из разных окон терминала WSL.
### ring_buffer
- Буфер служит для передачи отметок времени между процессами.
- Имеет размер 2040 байт и укладывается в 2КБ, должен помещаться в L1 кэш процессора.
- Имеет всего 2 операции, доступные пользователю: `read` и `write`, которые в случае успеха возвращают `true`, иначе `false`.
#### Потокобезопасность
- Потокобезопасность реализована через механизм активного ожидания положительного возврата операции над буфером, чтобы лишний раз не использовать системные вызовы.
- Она полагается на то что буфер используется исключительно при условии SWSR, поскольку не реализованы никакие средства защиты от параллельного выполнения как писателя, так и читателя.
### writer
- Принимает на вход в качестве аргумента время в микросекундах, на которое процесс уходит спать после каждой операции записи (так задаётся частота записи в буфер).
- Перед каждой операцией измеряет текущее время и записывает его в буфер.
#### Команда для компиляции:
`g++ -lstdc++ writer.cpp ring_buffer.cpp -march=native -O2 -o writer`
#### Пример команды запуска:
`./writer 10`
### reader
- После каждой операции измеряет текущее время и записывает разницу со считанным значением в массив.
- После считывания всех операций выводит получившиеся значения в стандартный поток вывода.
#### Команда для компиляции:
`g++ -lstdc++ reader.cpp ring_buffer.cpp -march=native -O2 -o reader`
#### Пример команды запуска (с перенаправлением вывода в файл):
`./reader > result.txt`
