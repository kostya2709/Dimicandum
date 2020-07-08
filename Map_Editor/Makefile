FNAME=map_editor
CFLAGS= -c -Wall
LFLAGS= -o

alll: clean auxilary_files all
all: $(FNAME)

$(FNAME): $(FNAME).o
	@g++ $(LFLAGS) $(FNAME) main.o Buttons.o Game_Class.o Textures.o Music.o -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
	@./$(FNAME)

$(FNAME).o:
	@g++ $(CFLAGS)  main.cpp


auxilary_files:
	@g++ $(CFLAGS)  Buttons.cpp
	@g++ $(CFLAGS)  Game_Class.cpp
	@g++ $(CFLAGS)  Textures.cpp
	@g++ $(CFLAGS)  Music.cpp	


clean:
	@rm -rf $(FNAME) *.o