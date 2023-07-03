#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mysql.h>
#include <time.h>
#include <signal.h>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp> // Include CascadeClassifier header
#include "../include/face_detector.h"

using namespace std;
using namespace cv;

CascadeClassifier face_classifier; // Declare face_cascade variable

MYSQL* dB;
VideoCapture camera;

void send_to_dB(string data, int type)
{
    time_t now = time(0);
    char* timeString = ctime(&now);
    timeString[strlen(timeString) - 1] = '\0'; 

    string command;
    command = "INSERT INTO PROJECT.FACES (Face_num, Time) VALUES(" + data + ",\"" + timeString + "\")";

    if (mysql_query(dB, command.c_str())) {
        fprintf(stderr, "%s\n", mysql_error(dB));
	mysql_close(dB);
	exit(1);
    }
}

int main(int argc, char* argv[])
{
    camera.open(0);
    if (!camera.isOpened()) {
        printf("ERROR: Could not open camera \n");
        return 1;
    }

    if ((!face_classifier.load("/usr/share/opencv4/haarcascades/haarcascade_frontalface_alt.xml"))) {
        cout << "--(!)Error loading face cascade\n";
        return -1;
    };

    if (mysql_library_init(0, NULL, NULL)) {
        printf("Could not initialize MySQL library\n");
        return 1;
    }

    dB = mysql_init(NULL);
    if (dB == NULL) {
        fprintf(stderr, "%s\n", mysql_error(dB));
        exit(1);
    }

    if (mysql_real_connect(dB, "localhost", "root", "mohsh2002", "PROJECT", 0, NULL, 0) == NULL) {
        fprintf(stderr, "%s\n", mysql_error(dB));
	mysql_close(dB);
	exit(1);
    }

    if (mysql_query(dB, "CREATE TABLE IF NOT EXISTS FACES (id INT AUTO_INCREMENT PRIMARY KEY, Face_num INT, Time DATETIME)")) {
        fprintf(stderr, "%s\n", mysql_error(dB));
	mysql_close(dB);
	exit(1);
    }

    if (mysql_query(dB, "USE PROJECT")) {
        fprintf(stderr, "%s\n", mysql_error(dB));
        mysql_close(dB);
        exit(1);
    }

    int previous_face_num = 0;
    int current_face_num;
    Mat frame;

    while (true)
    {
       // Read a frame from the webcam
       camera >> frame;

       // Detect faces in the frame
       vector<Rect> faces = face_detect(frame, face_classifier);
       current_face_num = faces.size();
       
       // Display the frame and the faces
       display_faces(frame, faces);
       
       if (current_face_num != previous_face_num) {
            previous_face_num = current_face_num;
            string face_num = to_string(current_face_num);
            send_to_dB(face_num, 1);
        }

       // Wait for a key press
       int c = waitKey(10);

       // Break if ESC is pressed
       if (c == 27)
           break;
    }

    mysql_close(dB);
    mysql_library_end();
    return 0;
}

