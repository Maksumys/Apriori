##Решение тестовой задачи Apriori


### Сборка Linux/MacOs:

	mkdir build
	cd build
	cmake ..
	make

### Справка:

	./test_app --help

### Пример запуска:

	./test_app --file association_rules.data --count 1000 --sort desc --support 360