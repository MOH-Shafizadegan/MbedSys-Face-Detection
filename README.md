# MbedSys-Face-Detection
An Embedded system project which contains face detection, working with MySQL, MQTT and Boost server

The main goal of this section of project is to stream video from laptop webcam and detect faces on the video using
trained model called Haar Cascade classifier. We also use MySQL database to save the time when number of faces
in the streamed video changed. In other part of this section we will use MQTT IOT protocol. This is a project with
various parts that we will explain them in the following.

- init_dB
In this folder we create and initialize a MySQL database for our project. A table will be added to the database in order to store our data. 

- DB
In this folder, the source cpp files are Database.cpp and face_detector.cpp. The goal is to detect faces in frames of a video stream using pre-trained Haar cascade classifier and update the database whenever number of detected faces changes.

- MQTT
This directory implements MQTT IOT protocol. In the Publisher folder, the code of the publisher side have been developed. The subscriber code is also available in the Subscriber folder

- Server
Using boost server, we have created a server to take a photo using the device webcam and display it to user by handling GET requests. Also using another GET request the user can access the latest n items of the database table.

We build all C++ codes of this project using standard CMakeLists.txt files.   
