CC = g++
CFLAGS = -Wall -g -std=c++17
TARGET = calibration

OPENCV_CFLAGS = $(shell pkg-config --cflags opencv4)
OPENCV_LIBS = $(shell pkg-config --libs opencv4)

$(TARGET): calibrate.o CalibrateCamera.o
	$(CC) $(CFLAGS) calibrate.o CalibrateCamera.o -o $(TARGET) $(OPENCV_LIBS)

calibrate.o: calibrate.cpp CalibrateCamera.h
	$(CC) $(CFLAGS) $(OPENCV_CFLAGS) -c calibrate.cpp 

CalibrateCamera.o: CalibrateCamera.cpp CalibrateCamera.h
	$(CC) $(CFLAGS) $(OPENCV_CFLAGS) -c CalibrateCamera.cpp

clean:
	rm -f *.o $(TARGET)