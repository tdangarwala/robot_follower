#include <iostream>
#include <opencv2/opencv.hpp>

#include "PersonDetector.h"

using namespace cv;


int main(){

    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    VideoCapture cap(0);

    if(!cap.isOpened()){
        std::cout << "Error opening camera" << std::endl;
        return -1;
    }
    
    PersonDetector detector;
    Mat frame;

    while(true){
        
        cap >> frame;
        if(frame.empty()){
            std::cout << "Error reading frame" << std::endl; 
        }

        auto [processedFrame, direction] = detector.processFrame(frame);

        imshow("Camera Feed", processedFrame);
        
        if(waitKey(30) >= 0){
            break;
        }

    }


    cap.release();
    destroyAllWindows();

    return 0;
}