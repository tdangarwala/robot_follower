#include <stdio.h>
#include <iostream>

#include "CalibrateCamera.h"


CalibrateCamera::CalibrateCamera(){}

void CalibrateCamera::addImage(const Mat& cal_img){
    inputImages.push_back(cal_img);
}

double CalibrateCamera::runCalibration(){
    //vector to store images taken from camera of checkerboard
    std::vector<Mat> inputImages;
    //vector to store vectors of 3d points for each checkerboard
    std::vector<std::vector<Point3f>> objpoints;

    //vector to store vectors of 2d points for each checkerboard
    std::vector<std::vector<Point2f>> imgpoints;

    int CHECKERBOARD[2]{2,6};

    std::vector<cv::Point3f> objp;
    for(int i = 0; i < CHECKERBOARD[1]; i++)
    {
        for(int j = 0; j < CHECKERBOARD[0]; j++){
            objp.push_back(cv::Point3f(j,i,0));
        }
            
    }

    CalibrationOuput params;
    Mat gray;
    bool success;
    std::vector<Point2f> corner_pts;

    for(size_t i = 0; i < inputImages.size(); i++){
        Mat temp = inputImages[i];
        
        cvtColor(temp, gray, COLOR_BGR2GRAY);

        success = findChessboardCorners(gray, Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);

        if(success){
            TermCriteria criteria(TermCriteria::EPS | TermCriteria::MAX_ITER, 30, 0.001);
       
            // refining pixel coordinates for given 2d points.
            cornerSubPix(gray,corner_pts,Size(11,11), Size(-1,-1),criteria);

            objpoints.push_back(objp);
            imgpoints.push_back(corner_pts);
        }
    }

    /*
   * Performing camera calibration by 
   * passing the value of known 3D points (objpoints)
   * and corresponding pixel coordinates of the 
   * detected corners (imgpoints)
  */
  cv::calibrateCamera(objpoints, imgpoints, Size(gray.rows,gray.cols), params.cameraMatrix, params.distCoeffs, params.R, params.T);

  return params.cameraMatrix.at<double>(0,0);
}