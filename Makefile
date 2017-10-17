H_FILES = gameController.h netController.h genetico.h mythreads/my_mutex.h mythreads/my_thread.h mythreads/scheduler.h
C_FILES = gameController.c netController.c main.c genetico.c mythreads/my_mutex.c mythreads/my_thread.c mythreads/scheduler.c

all:
	gcc $(H_FILES) $(C_FILES) -o tecRoyale

run:
	./tecRoyale game.ini
