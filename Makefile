H_FILES = gameController.h netController.h
C_FILES = gameController.c netController.c main.c

all:
	gcc $(H_FILES) $(C_FILES) -lpthread -o tecRoyale

run:
	./tecRoyale
