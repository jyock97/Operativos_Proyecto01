H_FILES = gameController.h netController.h my_mutex.h my_thread.h scheduler.h
C_FILES = gameController.c netController.c main.c my_mutex.c my_thread.c scheduler.c

all:
	gcc $(H_FILES) $(C_FILES) -o tecRoyale

run:
	./tecRoyale
