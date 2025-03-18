#include "PersonTracker.h"


PersonTracker::PersonTracker(Mat& frame, Rect bbox): bounding_box(bbox)
{
    cvtColor(frame, prev_gray, COLOR_BGR2GRAY);
    criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03);
    
}

void PersonTracker::detectFeatures(){
    features.clear();

    std::cout << "detectFeatures " << prev_gray.cols << " " << prev_gray.rows << std::endl;
    Mat roi = prev_gray(bounding_box);
    goodFeaturesToTrack(roi, features, 100, 0.3, 7);

    // Adjust feature coordinates to global frame coordinates
    for (auto& pt : features) {
        pt.x += bounding_box.x;
        pt.y += bounding_box.y;
    }
}

void PersonTracker::updateBbox(Rect new_bounding_box){

    // Validate the bounding box before assigning it
    new_bounding_box.x = std::max(0, new_bounding_box.x);
    new_bounding_box.y = std::max(0, new_bounding_box.y);
    
    // Ensure the box doesn't extend beyond the image boundaries
    new_bounding_box.width = std::min(new_bounding_box.width, prev_gray.cols - new_bounding_box.x);
    new_bounding_box.height = std::min(new_bounding_box.height, prev_gray.rows - new_bounding_box.y);

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

