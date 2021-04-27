CC = g++
all :
		$(CC) -w -framework Carbon -framework OpenGL -framework GLUT -Wall *.cpp *.c
