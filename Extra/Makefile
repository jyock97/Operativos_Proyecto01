HEADFILES = my_thread.h scheduler.h my_mutex.h
FILES = my_thread.c scheduler.c my_mutex.c

THREADHEADERS = threadWebServer.h socket.h
THREADFILES = threadWebServer.c socket.c

GLOBALHEADERS = httpRequestHandler.h requestHandler.h
GLOBARLFILES = httpRequestHandler.c requestHandler.c

all:
	gcc $(HEADFILES) $(FILES) test.c -o test

extra:
	gcc -lpthread $(THREADHEADERS) $(GLOBALHEADERS) $(HEADFILES) $(FILES) $(GLOBARLFILES) $(THREADFILES) -o extra

run_extra:
	./extra -p 2023 -n 10 -w /home/julio/
