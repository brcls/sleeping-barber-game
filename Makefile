PROJECT_NAME=teste
SOURCE_FILES=*.cpp
HEADER_FILES=*.h
OUTPUT_FILE=./${PROJECT_NAME}
CFLAGS=-Wall -Wextra -Werror -g -pthread
#-fsanitize=address
VFLAGS=--leak-check=full --show-leak-kinds=all --track-origins=yes
OFLAGS = -lglut -lGLU -lGL 

all:
	g++ -o ${PROJECT_NAME} ${SOURCE_FILES} ${CFLAGS} ${OFLAGS}
debug: all
	gdb ${OUTPUT_FILE}
valgrind: all
	valgrind ${VFLAGS} ${OUTPUT_FILE}
run: all
	${OUTPUT_FILE} 
zip: all
	zip -r ${PROJECT_NAME} . -i \*cpp \*h Makefile
