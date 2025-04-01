#include <iostream>
#include <opencv2/opencv.hpp>

#include "PersonDetector.h"
#include "PersonTracker.h"

using namespace cv;




int main(){

    std::string modelPath = "/home/tapan/projects/robot_follower/models/ssd_mobilenet_v2_coco_2018_03_29/frozen_inference_graph.pb";
    std::string configPath = "/home/tapan/projects/robot_follower/models/ssd_mobilenet_v2_coco_2018_03_29.pbtxt";
    
    KalmanDistanceFilter distance_filter(0.1,0.1);
    PersonDetector detector(modelPath,configPath, distance_filter);

    //toggle comment if you need to run calibration routine
    //detector.calibrateDistance();
    
                            
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
    DetectionOutput perception_output;
    bool valid_detection = false;

    while(!valid_detection){
        cap >> frame;
        if(frame.empty()){
            std::cout << "Error reading frame" << std::endl; 
        }

        perception_output = detector.processFrame(frame);
        std::cout << perception_output.boundingBox.width << " " << perception_output.boundingBox.height << std::endl;
        if (perception_output.boundingBox.width > 0 && perception_output.boundingBox.height > 0) {
            distance_filter.init(perception_output.distance);
            valid_detection = true;
            continue;
        } else {
            std::cout << "No person detected. Retry " << std::endl;
        
            imshow("Camera Feed", frame);
            waitKey(100);  // Short delay between retries
        }
    }
    
    PersonTracker pt(frame, perception_output.boundingBox);

    pt.detectFeatures();

    int count = 0;

    while(true){

        cap >> frame;
        if(frame.empty()){
            std::cout << "Error reading frame" << std::endl;
            continue; 
        }

        std::vector<Point2f> tracked_points = pt.calculateLK(frame);
        std::cout << tracked_points.size() << " " << count << std::endl;

        bool newMeasurement = false;
        double measuredDistance = -1;
        if(tracked_points.size() < 10){
            std::cout << "Resetting person detection" << std::endl;
            DetectionOutput newDetection;
            newDetection = detector.processFrame(frame);

            if (newDetection.boundingBox.width > 0 && newDetection.boundingBox.height > 0) {
                perception_output.boundingBox = newDetection.boundingBox;
                measuredDistance = newDetection.distance;  // Get the new measurement
                newMeasurement = true;
                distance_filter.init(perception_output.distance);
                pt.updateBbox(perception_output.boundingBox);
                pt.detectFeatures();
            }
        }

        // Use the filter properly
        if (newMeasurement) {
            // We have a new measurement, so update the filter with it
            perception_output.distance = distance_filter.process(measuredDistance, true);
        } else {
            // No new measurement, just predict based on previous state
            perception_output.distance = distance_filter.predict();
    }
        
        std::cout << "Distance: " << perception_output.distance << std::endl;
        
        count++;
        imshow("Camera Feed", frame);
        
        if(waitKey(30) >= 0){
            break;
        }

        

    }


    cap.release();
    destroyAllWindows();

    return 0;
}