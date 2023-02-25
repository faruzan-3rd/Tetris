CC = g++
CFLAGS = -std=c++17
TARGET = app

SRCS = src/main.cpp src/tetris/grid.cpp src/tetris/config.cpp src/tetris/assets.cpp\
		src/tetris/tetris.cpp src/tetris/blocks.cpp src/tetris/inputmanager.cpp\
		src/tetris/gameclock.cpp src/tetris/pouch.cpp src/tetris/UI.cpp


OBJS = $(SRCS:.cpp=.o)
INCDIR = -Isrc/include/ -I/usr/local/include/
LIBDIR = 
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lyaml-cpp

$(TARGET): $(OBJS)
	$(CC) -o $@ $(^F) $(LIBDIR) $(LIBS)

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $(INCDIR) -c $(@:.o=.cpp)

all: clean $(OBJS) $(TARGET)

clean:
	-rm -f $(OBJS) $(TARGET) *.d