CC = gcc
CFLAGS = -Wall -g
PATH1 = cd ../checker
PATH2 = cd ../src
CHECK_CMD = python3 checker.py

build: main

main: main.o operations.o
	$(CC) -o main $^ 

main.o: main.c
	$(CC) $(CFLAGS) -c $^

operations.o: operations.c
	$(CC) $(CFLAGS) -c $^

run: build
	./main

valgrind:
	clear
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./main

clean:
	rm -f *.o *.out main 

archive:
	zip -r tema1.zip *.c *.h ./main Makefile README 



check_help:
	clear
	$(PATH1) ; $(CHECK_CMD) --help ; $(PATH2)

check_valgrind:
	clear
	$(PATH1) ; $(CHECK_CMD) --valgrind ; $(PATH2)

check_output:
	clear	
	$(PATH1) ; $(CHECK_CMD) --output ; $(PATH2)

check_clear:
	clear
	$(PATH1) ; $(CHECK_CMD) --clear --valgrind; $(PATH2)

check_print:
	clear
	$(PATH1) ; $(CHECK_CMD) --print --valgrind; $(PATH2)

check_analyse:
	clear
	$(PATH1) ; $(CHECK_CMD) --analyse --valgrind; $(PATH2)

check_all:
	clear
	$(PATH1) ; $(CHECK_CMD) --all --output ; $(PATH2)

check_all_valgrind:
	clear
	$(PATH1) ; $(CHECK_CMD) --all --output --valgrind ; $(PATH2)


print_easy:
	clear
	$(PATH2) && make clean && make && ./main ../checker/input/sensors_print_easy_$(filter-out $@,$(MAKECMDGOALS)).dat < ../checker/input/commands_print_easy_$(filter-out $@,$(MAKECMDGOALS)).in > print_easy_$(filter-out $@,$(MAKECMDGOALS)).out && $(PATH2)
	
print_hard:
	clear
	$(PATH2) && make clean && make && ./main ../checker/input/sensors_print_hard_$(filter-out $@,$(MAKECMDGOALS)).dat < ../checker/input/commands_print_hard_$(filter-out $@,$(MAKECMDGOALS)).in > print_hard_$(filter-out $@,$(MAKECMDGOALS)).out && $(PATH2)
	
clear_easy:
	clear
	$(PATH2) && make clean && make && ./main ../checker/input/sensors_clear_easy_$(filter-out $@,$(MAKECMDGOALS)).dat < ../checker/input/commands_clear_easy_$(filter-out $@,$(MAKECMDGOALS)).in > clear_easy_$(filter-out $@,$(MAKECMDGOALS)).out && $(PATH2)
	
clear_hard:
	clear
	$(PATH2) && make clean && make && ./main ../checker/input/sensors_clear_hard_$(filter-out $@,$(MAKECMDGOALS)).dat < ../checker/input/commands_clear_hard_$(filter-out $@,$(MAKECMDGOALS)).in > clear_hard_$(filter-out $@,$(MAKECMDGOALS)).out && $(PATH2)
	
analyse:
	clear
	$(PATH2) && make clean && make && ./main ../checker/input/sensors_analyse_$(filter-out $@,$(MAKECMDGOALS)).dat < ../checker/input/commands_analyse_$(filter-out $@,$(MAKECMDGOALS)).in > analyse_$(filter-out $@,$(MAKECMDGOALS)).out && $(PATH2)
	
