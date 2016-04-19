## Content Based Video Retrieval using Histogram matching (opencv version 2.4.9) TASK 2

## Directory structure

```
.
├─ CMakeList.txt
├─ build
│   └── MakeFile (and all executables)
├─ data
│   └── xmls (for each video in database it saves shot's metadata(histogram, start time, end time) as xml file )
│   └── shots (input single shot videos for Testing)
├
└─ src (opencv source code)
    └── createMapping.cpp (takes database videos folder path as input argument and creates a mapping file in ../data/videosMap.xml ..
        └── .. mapping file contains original videos Names, directory and extensions.
    └── saveShots.cpp (takes mapping file ^ as input argument and computes histogram of each shot in video and saves metadata as xml)
    └── getVideosfromShotTHREAD.cpp (takes single-shot-video and mapping file of database as input arguments and returns all the videos..
        └── .. containing that shot and also its start/end frame number.
```

## HOW TO RUN 
```
* Go to "build" Directory and compile all cpp files by running   "cmake .." followed by "make"
* ./createMapping <path of directory containing videos>

* ./saveShots <videos mapping file.xml > 

* ./getVideosfromShotTHREAD <shot.mp4> <videos mapping file.xml>


```

##About
* Author: Omair Hassaan, Muhammad Hassan Ahmed
* Email: [omair.hassaan@gmail.com] [14030008@lums.edu.pk] [15060029@lums.edu.pk]
* Institute: Lahore University Of Management Sciences. LUMS. Pakistan.
