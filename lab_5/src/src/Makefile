# Makefile for lab work 5
 
CC = gcc
CFLAGS = -std=c99
LDFLAGS = -L.
 
CLIBFLAGS = -c -fPIC
LDLIBFLAGS = -shared
 
EXECUTABLE1 = task1
EXECUTABLE2 = task2

SRC1 = main1.c
SRC2 = main2.c
OBJ1 = $(SRC1:.c=.o)
OBJ2 = $(SRC2:.c=.o)

SRCLIB1 = ./libs/lib1.c
ND_SRCLIB1 = $(notdir $(SRCLIB1))
OBJLIB1 = $(ND_SRCLIB1:.c=.o)
SOLIB1 = $(ND_SRCLIB1:.c=.so)
 
SRCLIB2 = ./libs/lib2.c
ND_SRCLIB2 = $(notdir $(SRCLIB2))
OBJLIB2 = $(ND_SRCLIB2:.c=.o)
SOLIB2 = $(ND_SRCLIB2:.c=.so)
 
all: $(SOLIB1) $(SOLIB2) $(EXECUTABLE1) $(EXECUTABLE2)
 
$(EXECUTABLE1): $(OBJ1)
	$(CC) $(LDFLAGS) $< $(SOLIB1) -o $@ -lm

# -Wl,-rpath,.
# export LD_LIBRARY_PATH=. (-n - delete)
 
$(EXECUTABLE2): $(OBJ2)
	$(CC)  $< -o $@ -ldl
 
$(SOLIB1): $(OBJLIB1)
	$(CC) $(LDLIBFLAGS) $< -o $@ -lm
 
$(OBJLIB1): $(SRCLIB1)
	$(CC) $(CLIBFLAGS) $<
 
$(SOLIB2): $(OBJLIB2)
	$(CC) $(LDLIBFLAGS) $< -o $@ -lm
 
$(OBJLIB2): $(SRCLIB2)
	$(CC) $(CLIBFLAGS) $< 

clean:
	rm *.o $(SOLIB1) $(SOLIB2) $(EXECUTABLE1) $(EXECUTABLE2)