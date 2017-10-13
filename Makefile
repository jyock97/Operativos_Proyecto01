H_FILES = gameController.h netController.h genetico.h
C_FILES = gameController.c netController.c main.c genetico.c

all:
	gcc $(H_FILES) $(C_FILES) -lpthread -o tecRoyale

run:
	./tecRoyale
