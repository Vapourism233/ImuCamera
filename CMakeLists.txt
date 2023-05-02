cmake_minimum_required(VERSION 3.22)
project(test1)

set(CMAKE_PROJECT_NAME=${CMAKE_PROJECT_NAME})
#set(CMAKE_CXX_STANDARD 14)
set(OpenCV_DIR "/home/jiang/opencv/build")
find_package(OpenCV REQUIRED)
include_directories(/home/jiang/Qt5.12.8/5.12.8/gcc_64/include)
include_directories(/usr/local/include/opencv4/opencv2)
include_directories(/home/jiang/CLionProjects/test1/lib)
link_directories(/home/jiang/Qt5.12.8/5.12.8/gcc_64/lib)
link_directories(${OPENCV_LIB} /home/jiang/opencv/build/lib)

link_libraries(AudioRender)
link_libraries(HCCore)
link_libraries(hcnetsdk)
link_libraries(hpr)
link_libraries(NPQos)
link_libraries(PlayCtrl)
link_libraries(SuperRender)
link_libraries(analyzedata)
link_libraries(AudioIntercom)
link_libraries(HCCoreDevCfg)
link_libraries(HCDisplay)
link_libraries(HCIndustry)
link_libraries(HCPlayBack)
link_libraries(StreamTransClient)
link_libraries(SystemTransform)
link_libraries(z)
link_libraries(crypto)
link_libraries(opencv_core)
link_libraries(ssl)
link_libraries(openal)
link_libraries(HCGeneralCfgMgr)
link_libraries(pthread)
add_executable(test1 main.cpp
        HelloMotionModule.h
        EasyProfile/BasicTypes.h
        EasyProfile/EasyObjectDictionary.h
        EasyProfile/EasyQueue.h
        EasyProfile/EasyProtocol.h
        EasyProfile/EasyProfile.h
        EasyProfile/EasyProtocol.cpp
        EasyProfile/EasyProfile.cpp
        EasyProfile/EasyObjectDictionary.cpp
        ui_HelloMotionModule.h
        HCNetSDK.h
        OCR.h
        OCR.cpp
        Plate.h
        Plate.cpp
        DetectRegions.h
        DetectRegions.cpp
        )
target_link_libraries(test1 ${OpenCV_LIBS})
SET(RESOURCES
        SVM.xml
        OCR.xml
        )