CXX= g++
CXXFLAGS= -mwindows -std=c++20 -Wall -Wextra -Werror -Ofast -s -DSFML_STATIC
CXXLIBS= -lsfml-graphics-s -lsfml-window-s -lopengl32 -ljpeg -lfreetype -lgdi32 -lsfml-audio-s -lopenal32 -lFLAC -lvorbisenc -lvorbisfile -lvorbis -logg -lsfml-network-s -lws2_32 -lsfml-system-s -lwinmm
CXXLIBPATHS= -L"C:\Users\famma\Documents\My_Programs\SFML\SFML-2.5.1\extlibs\libs-mingw\x64" -L"C:\Users\famma\Documents\My_Programs\SFML\SFML-2.5.1\staticbuild\lib"

OBJECTS= .\make\main.o .\make\tetromino.o .\make\randbag.o .\make\icon.o .\make\dialog.o .\make\letterbox.o .\make\ghost.o .\make\highscore.o
SOURCES= main.cpp tetromino.cpp randbag.cpp data.rc

upx: $(OBJECTS) tetris.exe
	@upx tetris.exe > nul
	@echo Compressed target tetris.exe

tetris.exe: $(OBJECTS)
	@$(CXX) $(CXXFLAGS) $(CXXLIBPATHS) $(OBJECTS) $(CXXLIBS) -o tetris.exe
	@echo Linked target tetris.exe

.\make\main.o: $(SOURCES)
	@$(CXX) $(CXXFLAGS) $(CXXLIBPATHS) $(CXXLIBS) -c main.cpp -o .\make\main.o
	@echo Compiled main.o

.\make\tetromino.o: $(SOURCES)
	@$(CXX) $(CXXFLAGS) $(CXXLIBPATHS) $(CXXLIBS) -c tetromino.cpp -o .\make\tetromino.o
	@echo Compiled tetromino.o

.\make\randbag.o: $(SOURCES)
	@$(CXX) $(CXXFLAGS) $(CXXLIBPATHS) $(CXXLIBS) -c randbag.cpp -o .\make\randbag.o
	@echo Compiled randbag.o

.\make\icon.o: $(SOURCES)
	@windres data.rc -o .\make\icon.o
	@echo Generated icon.o

.\make\dialog.o: $(SOURCES)
	@$(CXX) $(CXXFLAGS) $(CXXLIBPATHS) $(CXXLIBS) -c dialog.cpp -o .\make\dialog.o
	@echo Compiled dialog.o

.\make\letterbox.o: $(SOURCES)
	@$(CXX) $(CXXFLAGS) $(CXXLIBPATHS) $(CXXLIBS) -c letterbox.cpp -o .\make\letterbox.o
	@echo Compiled letterbox.o

.\make\ghost.o: $(SOURCES)
	@$(CXX) $(CXXFLAGS) $(CXXLIBPATHS) $(CXXLIBS) -c ghost.cpp -o .\make\ghost.o
	@echo Compiled ghost.o

.\make\highscore.o: $(SOURCES)
	@$(CXX) $(CXXFLAGS) $(CXXLIBPATHS) $(CXXLIBS) -c highscore.cpp -o .\make\highscore.o
	@echo Compiled highscore.o

precomp:
	@$(CXX) $(CXXFLAGS) includes.hpp
	@echo Precompiled includes.hpp


clean:
	@del $(OBJECTS)
