cmake_minimum_required(VERSION 2.8)
project( Vid2Img )
find_package( OpenCV REQUIRED )

set(CMAKE_CXX_FLAGS "-g -std=c++11")

#add_executable( vid2img src/video_to_images.cpp )
#target_link_libraries( vid2img  ${OpenCV_LIBS} )

add_executable( videoImagesDatabase src/videoImagesDatabase.cpp )
target_link_libraries( videoImagesDatabase  ${OpenCV_LIBS} )

add_executable( videoRetrieval src/videoRetrieval.cpp )
target_link_libraries(videoRetrieval  ${OpenCV_LIBS} )

add_executable( saveShots src/saveShots.cpp )
target_link_libraries(saveShots  ${OpenCV_LIBS} )

#add_executable( rgb_hist src/rgb_hist.cpp )
#target_link_libraries(rgb_hist  ${OpenCV_LIBS} )

#add_executable( testcode src/test.cpp )
#target_link_libraries(testcode  ${OpenCV_LIBS} )

#add_executable( video_dictionary src/video_to_images_dictionary.cpp )
#target_link_libraries(video_dictionary  ${OpenCV_LIBS} )

add_executable( imgInVideoOut src/imgInVideoOut.cpp )
target_link_libraries(imgInVideoOut  ${OpenCV_LIBS} )


