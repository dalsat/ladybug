# MakeFile del progetto di informatica grafica 2006/2006
# author: Tommaso Dal Sasso  n.matr.: 507276

# general settings
COMPILE= g++
OUTPUT= progetto
DEL= rm -f
MODELDIR= Modello

# libraries
LIBS= -lglut
LIBPATH=
#-L /usr/X11R6/lib


# targets
all: clean main.o gun.o bullet.o enemy.o camera.o
	$(COMPILE) main.o Gun.o Bullet.o Enemy.o Camera.o -o $(OUTPUT) $(LIBS) $(LIBPATH)
#	progetto
	
main.o:
	$(COMPILE) -c main.cpp
	
gun.o:
	$(COMPILE) -c Modello/Gun.cpp
	
bullet.o:
	$(COMPILE) -c Modello/Bullet.cpp

enemy.o:
	$(COMPILE) -c Modello/Enemy.cpp
	
camera.o:
	$(COMPILE) -c Camera.cpp
	

clean:
	$(DEL) -f *.o *~ $(OUTPUT)
	
uninstall: clean
	$(DEL) -f progetto