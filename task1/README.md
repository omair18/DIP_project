## Content Based Video Retrieval using Histogram matching (opencv version 2.4.9) TASK1

## Directory structure

```
.
├─ CMakeList.txt
├─ build
│   └── MakeFile (and all executables)
├─ data
│   └── videos (videos for database creation and testing )
│   └── frames (saves frames from all videos (Task 1.1))
│   └── shots (saves shots from all videos (Task 2.1) )
├
└─ src (opencv source code)
    └── videoImagesDatabase.cpp (Task(1.1) Read all videos and save frames after each 15secs in ../data/frames also saves feature vector in data/Histograms.xml )
    └── videoRetrieval.cpp (Task 1.2, Reads a video and finds its duplicate/near match in database located at (../data/videos))
    └── saveShots.cpp (Saves shots from all videos in folder "data/shots")
    └── imgInVideoOut.cpp  (reads an image and finds the near match video from database)

```

## HOW TO RUN 
```
Note: This code assumes that all videos are of format  vid1.mp4, vid2.mp4 vid3,mp4 placed in folder "data/videos/" folder

* Go to "build" Directory and compile all cpp files by running   "cmake .." followed by "make"
* Task 1.1:   ./videoImagesDatabase ../data/videos/vid1.mp4 ../data/videos/vid2.mp4 ../data/videos/vid3.mp4 ../data/videos/vid4.mp4 ../data/videos/vid5.mp4

* Task 1.2:   ./videoRetrieval ../data/<any video from data/videos>

* Task 2.1:   ./saveShots ../data/videos/vid1.mp4 ../data/videos/vid2.mp4 ../data/videos/vid3.mp4 ../data/videos/vid4.mp4 ../data/videos/vid5.mp4

* Task 2.2:  ./imgInVideoOut ../data/frames/<any image>
```

##About
* Author: Omair Hassaan, Muhammad Hassan Ahmed
* Email: [omair.hassaan@gmail.com] [14030008@lums.edu.pk] [15060029@lums.edu.pk]
* Institute: Lahore University Of Management Sciences. LUMS. Pakistan.
