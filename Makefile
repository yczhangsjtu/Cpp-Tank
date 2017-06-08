# Project: Tank
# Makefile created by Dev-C++ 5.5.3

CPP      = g++
OBJ      = collision.o control.o draw.o main.o message.o rect.o tank.o material.o\
		   ai.o effects.o 3ds.o
LIBS     = -lGL -lGLU -lglut -l3ds -fPIC
INC			 = -I.
BIN      = Tank

$(BIN): $(OBJ)
	$(CPP) $(OBJ) -o $(BIN) $(LIBS)

3ds.o: 3ds.cpp
	$(CPP) -c $< -o $@ $(INC)

collision.o: collision.cpp collision.h
	$(CPP) -c $< -o $@  $(INC)

control.o: control.cpp control.h rect.h message.h
	$(CPP) -c $< -o $@  $(INC)

draw.o: draw.cpp draw.h effects.h
	$(CPP) -c $< -o $@  $(INC)

main.o: main.cpp main.h tank.h collision.h draw.h control.h ai.h effects.h
	$(CPP) -c $< -o $@  $(INC)

message.o: message.cpp message.h tank.h effects.h draw.h
	$(CPP) -c $< -o $@  $(INC)

rect.o: rect.cpp rect.h
	$(CPP) -c $< -o $@  $(INC)

tank.o: tank.cpp tank.h collision.h rect.h message.h effects.h
	$(CPP) -c $< -o $@  $(INC)

material.o: material.cpp material.h
	$(CPP) -c $< -o $@  $(INC)

ai.o: ai.cpp ai.h control.h tank.h collision.h main.h
	$(CPP) -c $< -o $@  $(INC)
	
effects.o: effects.cpp effects.h
	$(CPP) -c $< -o $@  $(INC)
