CFLAGS += -DSDL_MAIN_HANDLED

LDLIBS := -lSDL2

OBJS := gameoflife.o

.PHONY: all clean

all: gameoflife
	./$<

gameoflife: $(OBJS)
	$(CC) -o $@ $(CFLAGS) $^ $(LDLIBS)
