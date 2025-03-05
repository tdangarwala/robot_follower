#include <iostream>
#include <opencv2/opencv.hpp>

#include "PersonDetector.h"
#include "PersonTracker.h"

using namespace cv;




int main(){

    std::string modelPath = "/home/tapan/projects/robot_follower/models/ssd_mobilenet_v2_coco_2018_03_29/frozen_inference_graph.pb";
    std::string configPath = "/home/tapan/projects/robot_follower/models/ssd_mobilenet_v2_coco_2018_03_29.pbtxt";

    PersonDetector detector(modelPath,configPath);

                                
    VideoCapture cap(0);

    if(!cap.isOpened()){
        std::cout << "Error opening camera" << std::endl;
        return -1;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 320);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 240);
    cap.set(cv::CAP_PROP_FPS, 10);

    std::cout << "Camera initialized successfully" << std::endl;
    
    Mat frame;

    cap >> frame;
    if(frame.empty()){
        std::cout << "Error reading frame" << std::endl; 
    }

    Rect bounding_box;
    Position direction;
    std::tie(bounding_box, direction) = detector.processFrame(frame);
    std::cout << bounding_box.height << " " << bounding_box.width << std::endl;

    PersonTracker pt(frame, bounding_box);

    pt.detectFeatures();

    while(true){

        cap >> frame;
        if(frame.empty()){
            std::cout << "Error reading frame" << std::endl;
            continue; 
        }

        std::vector<Point2f> tracked_points = pt.calculateLK(frame);

        std::cout << tracked_points.size() << std::endl;
        if(tracked_points.size() < 10){
            std::cout << "Resetting person detection" << std::endl;
            Rect newBoundingBox;
            Position newDirection;
            std::tie(newBoundingBox, newDirection) = detector.processFrame(frame);
            bounding_box = newBoundingBox;
            pt.updateBbox(bounding_box);
            pt.detectFeatures();
        }
        
        imshow("Camera Feed", frame);
        
        if(waitKey(30) >= 0){
            break;
        }

    }


    cap.release();
    destroyAllWindows();

    return 0;
}