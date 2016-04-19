cmake_minimum_required(VERSION 2.8)
project( Vid2Img )
find_package( OpenCV REQUIRED )

set(CMAKE_CXX_FLAGS "-g -std=c++11")

add_executable( saveShots src/saveShots.cpp )
target_link_libraries(saveShots  ${OpenCV_LIBS} )

#add_executable(getVideosfromShot src/getVideosfromShot.cpp )
#target_link_libraries(getVideosfromShot  ${OpenCV_LIBS} )

add_executable(getVideosfromShotTHREAD src/getVideosfromShotTHREAD.cpp )
target_link_libraries(getVideosfromShotTHREAD  ${OpenCV_LIBS} )

add_executable(createMapping src/createMapping.cpp )
target_link_libraries(createMapping ${OpenCV_LIBS} )


