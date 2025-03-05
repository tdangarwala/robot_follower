#include "PersonTracker.h"


PersonTracker::PersonTracker(Mat& frame, Rect bbox): bounding_box(bbox)
{
    cvtColor(frame, prev_gray, COLOR_BGR2GRAY);
    criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03);
    
}

void PersonTracker::detectFeatures(){
    features.clear();

    Mat roi = prev_gray(bounding_box);
    goodFeaturesToTrack(prev_gray, features, 100, 0.3, 7);
}

void PersonTracker::updateBbox(Rect new_bounding_box){
    bounding_box = new_bounding_box;
}



std::vector<Point2f> PersonTracker::calculateLK(Mat& frame){
    Mat gray;
    cvtColor(frame, gray, COLOR_BGR2GRAY);

    std::vector<Point2f> new_features;
    std::vector<uchar> status;
    std::vector<float> err;

    std::vector<Point2f> valid_features;
    if(!features.empty()){
        calcOpticalFlowPyrLK(prev_gray, gray, features, new_features, status, err, Size(15,15), 2, criteria);
        prev_gray = gray.clone();

        
        for(size_t i = 0 ; i < new_features.size(); i++){
            if(status[i] == 1){
                valid_features.push_back(new_features[i]);
            }
        }
        features = valid_features;
        prev_gray = gray.clone();
    }
    

    return valid_features;
}

