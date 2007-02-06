CONNECT_TEST_SOURCES=connect_test.cpp
CONNECT_TEST_HEADERS=
CONNECT_TEST_RESSOURCE=connect_test.resp
CC=cl

all: connect_test.exe

connect_test.exe: $(CONNECT_TEST_SOURCES) $(CONNECT_TEST_HEADERS)
	$(CC) @$(CONNECT_TEST_RESSOURCE)

clean:
	rm -rf connect_test.exe *.obj
