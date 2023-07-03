#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mysql.h>
#include <time.h>
#include <signal.h>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp> // Include CascadeClassifier header

using namespace std;
using namespace cv;

vector<Rect> face_detect(Mat frame, CascadeClassifier& classifier);
void display_faces(Mat frame, vector<Rect> faces);
