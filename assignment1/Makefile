# Compiler
CC = gcc

# Compiler flags
CFLAGS = -pthread

# Source file
SRC = pagerank.c

# Executable file
EXE = pagerank

# Output CSV file
CSV = pagerank.csv

# Determine the operating system
ifeq ($(OS),Windows_NT)
    RM = del /F
	EXE = pagerank.exe
else
    RM = rm -f
endif

all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	$(RM) $(EXE) $(CSV)
