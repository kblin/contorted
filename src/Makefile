CFLAGS=-Wall -ggdb
LIBS=
CC=gcc

dpsvr_SOURCES = dpsvr.c \
		dpsvr.h \
		dplay_common.c \
		dplay_common.h

dpsvr_OBJ = $(patsubst %.c,%.o,$(filter %.c, $(dpsvr_SOURCES)))
dpsvr_DEP = $(filter %.h, $(dpsvr_SOURCES))


.PHONEY: all clean

all: dpsvr

clean:
	rm -f dpsvr *.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<

dpsvr: $(dpsvr_OBJ) $(dpsvr_DEP)
	$(CC) $(LIBS) -o dpsvr $(dpsvr_OBJ)


