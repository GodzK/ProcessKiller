CC = gcc
CFLAGS = -Iinclude
LIBS = -lpsapi
SRC = src/main.c src/process_utils.c src/logger.c
OUT = taskmanager.exe

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(SRC) -o $(OUT) $(CFLAGS) $(LIBS)

clean:
	rm -f $(OUT)