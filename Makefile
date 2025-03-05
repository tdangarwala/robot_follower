CC = g++
CFLAGS = -Wall -g -std=c++17
TARGET = robot_follower

OPENCV_CFLAGS = $(shell pkg-config --cflags opencv4)
OPENCV_LIBS = $(shell pkg-config --libs opencv4)

$(TARGET): main.o PersonDetector.o PersonTracker.o
	$(CC) $(CFLAGS) main.o PersonDetector.o PersonTracker.o -o $(TARGET) $(OPENCV_LIBS)

main.o: main.cpp PersonDetector.h PersonTracker.h
	$(CC) $(CFLAGS) $(OPENCV_CFLAGS) -c main.cpp

PersonDetector.o : PersonDetector.cpp PersonDetector.h
	$(CC) $(CFLAGS) $(OPENCV_CFLAGS) -c PersonDetector.cpp

PersonTracker.o : PersonTracker.cpp PersonTracker.h
	$(CC) $(CFLAGS) $(OPENCV_CFLAGS) -c PersonTracker.cpp

clean:
	rm *.o $(TARGET)