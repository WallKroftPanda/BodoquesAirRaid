CC=g++
#SRC=src/enemigo.cpp src/maths_funcs.cpp src/gl_utils.cpp src/main.cpp
SRC=src/*.cpp
LIBS=-lGL -lGLEW -lglfw -lassimp
EXEC=prog

all: 
	${CC} ${SRC} ${LIBS} -o ${EXEC} 


clear:
	rm bin/*