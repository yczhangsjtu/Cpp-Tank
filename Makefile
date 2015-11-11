# Project: Tank
# Makefile created by Dev-C++ 5.5.3

CPP      = g++
OBJ      = collision.o control.o draw.o main.o message.o rect.o tank.o material.o\
		   ai.o effects.o 3ds.o
LIBS     = -lGL -lGLU -lglut -l3ds
BIN      = Tank

$(BIN): $(OBJ)
	$(CPP) $(OBJ) -o $(BIN) $(LIBS)

3ds.o: 3ds.cpp
	$(CPP) -c $< -o $@ 

collision.o: collision.cpp collision.h
	$(CPP) -c $< -o $@ 

control.o: control.cpp control.h rect.h message.h
	$(CPP) -c $< -o $@ 

draw.o: draw.cpp draw.h effects.h
	$(CPP) -c $< -o $@ 

main.o: main.cpp main.h tank.h collision.h draw.h control.h ai.h effects.h
	$(CPP) -c $< -o $@ 

message.o: message.cpp message.h tank.h effects.h draw.h
	$(CPP) -c $< -o $@ 

rect.o: rect.cpp rect.h
	$(CPP) -c $< -o $@ 

tank.o: tank.cpp tank.h collision.h rect.h message.h effects.h
	$(CPP) -c $< -o $@ 

material.o: material.cpp material.h
	$(CPP) -c $< -o $@ 

ai.o: ai.cpp ai.h control.h tank.h collision.h main.h
	$(CPP) -c $< -o $@ 
	
effects.o: effects.cpp effects.h
	$(CPP) -c $< -o $@ 
