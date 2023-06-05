# MAPH
Multi-Agent Path Finding with visualization. School project

## Условие

ТЕХНОЛОГИЧНО УЧИЛИЩЕ ЕЛЕКТРОННИ СИСТЕМИ към ТЕХНИЧЕСКИ УНИВЕРСИТЕТ - СОФИЯ

ПРОЕКТ ЗА ЛЯТНА ПРАКТИКА 10 КЛАС
Намиране на път в лабиринт с множество агенти

Даден е файл с лабиринт в следния формат:

На лабиринта символа # означава стена, през която не може да се премине.

Дадени са ни множество агенти, всеки със своята дестинация.
Пример: агент 1 с дестинация А, агент 2 с дестинация Б, агент 3 с дестинация В и агент 4 с дестинация Г


Всеки агент преминава от 1 клетка в съседна за 1 секунда. За да се придвижи по диагонал например горе-дясно трябва да извърши 2 движения - преместване нагоре + преместване надясно, което ще отнеме 2 секунди.

Във всяка клетка може да има по 1 агент в 1 момент от време. 

Агентите могат да се движат, но могат и да стоят на 1 място, например защото се налага да изчакат друг агент да им освободи клетката през която искат да минат.

Имплементирайте алгоритъм, който при даден лабиринт, агенти и техните дестинации генерира маршрут, при който всички агенти стигат до отредените им дестинации. 
Желателно е алгоритъмът да извежда такъв маршрут, че времето за което последния агент ще стигне до дестинацията си е възможно най-малко.

Вземете в предвид и примери, в които дестинацията на един агент може да е невъзможна за достигане, освен ако друг агент не се измести за да направи път.

Пример: агент 1 с дестинация А, агент 2 с дестинация Б, агент 3 с дестинация В и агент 4 с дестинация Г

## Разработка

1. Визуализация - всичко свързано с визуализацията е сложено във файловете "visualize.c" и "visualize.h":
    - За работата с поле се ползва матрица, помнеща какъв обект има в нея.
    
    - Видове полета:
        - празно (_v_free) - ' '
        - препядствие/tile (_v_tile) - '#'
        - агент (_v_agent) - от 'a' до 'z'
        - дестинация - от 'A' до 'Z'

2. Карта:
    - Генериране - картата има опцията да бъде генерирана на случаен принцип. Дава се размерите на мапа (size_x, size_y) и се дава брой на препядствия/tiles.
        - Използва се flood fill алгоритъма с който се проверява дали всики полета са достъпни. 

            1. Всички празни достъпни полета се запълват с момента (_v_tmp) стойност '.'
            2. Проверява се дали има някое останало поле със стойност празно (_v_free) - ' '
            3. Заедно с т.2 се връща стойноста празно (_v_free) на моментното проверено поле
    
    - Четене на мап от файл:
        - Ползване на готов preset от мапове - има готови подготвени мапове които може да се ползват на готово.

        - Даване на свой мап - можете да качите свой собствен мап. Този мап минава през fill flood алгоритъма за да бъде сигурно че може да бъде изплолзван правилно. Ако не го премине успешно, или ако съдържа ненужни/грешни символи, файлът ще бъде отказан. Правила за попълване:
            1. празно поле - ' ' или '.'
            2. препиадствие/tile - '#'
            * Полето може да бъде обградено със '|' и '-' символи. Това няма да промени фунционалността на четенето. Валидни примери:
                ```
                ----------------------
                |                    |        
                |                    |        
                |                    |        
                |                    |
                | ###############    |
                |                ### |
                |                   #|
                |                    |
                |                    |
                ----------------------
                ```
                или

                ```
                ....................
                ....................
                ....................
                ....................
                .###############....
                .###############....
                ................###.        
                ...................#        
                ....................        
                .................... 
                ```

            ! Агентите се слагат от програмата, не от потребителя. Буквите върху мап от файл ще се счетат за невалиден символ!
