#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>
#include <cstring>
#include <stdio.h>
#include <ctime>

using namespace cv;
using namespace std;

// Function that detects faces in a frame and returns a vector of faces
vector<Rect> face_detect(Mat frame, CascadeClassifier& classifier)
{
    Mat frame_gray;
    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);
    vector<Rect> faces;
    classifier.detectMultiScale(frame_gray, faces);
    return faces;
}

// Function that displays a frame and a vector of faces on the screen
void display_faces(Mat frame, vector<Rect> faces)
{
    int num_faces = faces.size();
    
    namedWindow("Capture - Face detection");

    // Get current time
    time_t now = time(0);
    char* timeString = ctime(&now);
    timeString[strlen(timeString) - 1] = '\0';

    // Display number of detected faces
    stringstream ss;
    ss << "Number of detected Faces: " << num_faces;
    putText(frame, ss.str(), Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2);

    // Display current time
    putText(frame, timeString, Point(10, 50), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2);

    for (size_t i = 0; i < faces.size(); i++)
    {
        rectangle(frame, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar(0, 0, 255), 3, LINE_8);

        // Display the number of the detected face
        stringstream ss_face;
        ss_face << "Face #" << i + 1;
        putText(frame, ss_face.str(), Point(faces[i].x, faces[i].y), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2);
    }

    imshow("Capture - Face detection", frame);
}

