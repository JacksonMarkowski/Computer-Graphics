EXE=project

# Main target
all: $(EXE)

#  MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall
LIBS=-lglut32cu -lglu32 -lopengl32
CLEAN=del *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
CFLG=-O3 -Wall -Wno-deprecated-declarations
LIBS=-framework GLUT -framework OpenGL
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall
LIBS=-lglut -lGLU -lGL -lm
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE) *.o *.a
endif

# Dependencies
CG_Project.o: CG_Project.cpp CSCIx229.h Entity.cpp Spaceship.cpp Terrain.cpp Barn.cpp Fence.cpp Cow.cpp Tree.cpp Grass.cpp Camera.cpp
Entity.o: entity.cpp entity.h
Spaceship.o: Spaceship.cpp Spaceship.h
Terrain.o: Terrain.cpp Terrain.h
Barn.o: Barn.cpp Barn.h
Fence.o: Fence.cpp Fence.h
Cow.o: Cow.cpp Cow.h
Tree.o: Tree.cpp Tree.h
Grass.o: Grass.cpp Grass.h
fatal.o: fatal.c CSCIx229.h
loadtexbmp.o: loadtexbmp.c CSCIx229.h
print.o: print.c CSCIx229.h
project.o: project.c CSCIx229.h
errcheck.o: errcheck.c CSCIx229.h
object.o: object.c CSCIx229.h

#  Create archive
CSCIx229.a:fatal.o loadtexbmp.o print.o project.o errcheck.o object.o
	ar -rcs $@ $^

# Compile rules
.c.o:
	gcc -c $(CFLG) $<
.cpp.o:
	g++ -c $(CFLG) -std=c++11 $<

#  Link
project:CG_Project.o CSCIx229.a Entity.o Spaceship.o Terrain.o Barn.o Fence.o Cow.o Tree.o Grass.o
	g++ -O3 -o $@ $^   $(LIBS)

#  Clean
clean:
	$(CLEAN)
