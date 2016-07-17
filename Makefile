.PHONY: clean

# CFLAGS  := -Werror -g -Wno-format
CFLAGS  := -g
LD      := gcc
LDFLAGS := ${LDFLAGS}

APPS    := qList 

all: ${APPS}

qList: qList.o main.o
	${LD} -o $@ $^ ${LDFLAGS}

clean:
	rm -f *.o ${APPS}
