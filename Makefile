CFLAGS := -Ofast
CFLAGS += -DSDL_MAIN_HANDLED

LDLIBS := -lSDL2

OBJS := gameoflife.o

.PHONY: all clean

all: gameoflife.exe
	./$<

gameoflife.exe: $(OBJS)
	$(CC) -o $@ $(CFLAGS) $^ $(LDLIBS)