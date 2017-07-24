CC=g++
CCFLAGS=-std=c++11 -I.
EXEC=interactive_session

compile: interactive_session.cpp interactive_session.hpp
	${CC} ${CCFLAGS} interactive_session.cpp -o ${EXEC}

clean:
	rm -f ${EXEC}

