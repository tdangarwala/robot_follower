#include <iostream>
#include <string>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "CalibrateCamera.h"

using namespace cv;

struct MouseCallbackData {
    Mat* currentFrame;               // Pointer to current frame
    //std::vector<Mat>* capturedImages; // Pointer to vector storing images
    int maxImages;                        // Max number of images
    CalibrateCamera* calDataStore;
};

static void onMouse(int event, int x, int y, int, void* info){
    static int count = 0;

    MouseCallbackData* data = static_cast<MouseCallbackData*>(info);
    if(event == EVENT_RBUTTONDOWN && count < data->maxImages){
        data->calDataStore->addImage(data->currentFrame->clone());
        count++;
        if(count == data->maxImages){
            std::cout << data->calDataStore->runCalibration() << std::endl;
            destroyAllWindows();

            exit(0);
        }
    }
}


int main() {

    VideoCapture cap(0);

    if(!cap.isOpened()){
        std::cout << "Error opening camera" << std::endl;
        return -1;
    }

    Mat frame;
    //std::vector<Mat> temp;
    int maxImages = 20;
    CalibrateCamera imageStore;

    MouseCallbackData callbackData{&frame, maxImages, &imageStore};

    namedWindow("Camera Feed", WINDOW_AUTOSIZE);
    setMouseCallback("Camera Feed", onMouse, &callbackData);
    
    while(true){
        cap >> frame;
        if(frame.empty()){
            std::cout << "Error reading frame" << std::endl; 
        }

        imshow("Camera Feed", frame);
        
        if(waitKey(30) >= 0){
            break;
        }
    }

    return 0;
}