# Compiler
CC = gcc

# Compiler flags
CFLAGS = -pthread

# Source file
SRC = app.c

# Executable file
EXE = app.exe

# Output CSV file
CSV = pagerank.csv

# Determine the operating system
ifeq ($(OS),Windows_NT)
    RM = del /F
else
    RM = rm -f
endif

all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	$(RM) $(EXE) $(CSV)
