cmake_minimum_required(VERSION 3.23)
project(pokemon-detector-sv)

set(CMAKE_CXX_STANDARD 17)

include(ExternalProject)

ExternalProject_Add(OpenCVDownload
        URL https://github.com/opencv/opencv/archive/refs/tags/4.7.0.tar.gz
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/libopencv_core.a
            <INSTALL_DIR>/lib/libopencv_imgcodecs.a
            <INSTALL_DIR>/lib/libopencv_imgproc.a
            <INSTALL_DIR>/lib/opencv4/3rdparty/liblibpng.a
            <INSTALL_DIR>/lib/opencv4/3rdparty/libzlib.a
        CMAKE_GENERATOR Ninja
        CMAKE_ARGS
            -DCMAKE_OSX_DEPLOYMENT_TARGET=10.13
            -DBUILD_SHARED_LIBS=OFF
            -DBUILD_opencv_apps=OFF
            -DBUILD_opencv_js=OFF
            -DBUILD_ANDROID_PROJECTS=OFF
            -DBUILD_ANDROID_EXAMPLES=OFF
            -DBUILD_DOCS=OFF
            -DBUILD_EXAMPLES=OFF
            -DBUILD_PACKAGE=OFF
            -DBUILD_PERF_TESTS=OFF
            -DBUILD_TESTS=OFF
            -DBUILD_WITH_DEBUG_INFO=ON
            -DBUILD_WITH_STATIC_CRT=OFF
            -DBUILD_WITH_DYNAMIC_IPP=OFF
            -DBUILD_FAT_JAVA_LIB=OFF
            -DBUILD_ANDROID_SERVICE=OFF
            -DBUILD_CUDA_STUBS=OFF
            -DBUILD_JAVA=OFF
            -DBUILD_OBJC=OFF
            -DBUILD_opencv_python3=OFF
            -DINSTALL_CREATE_DISTRIB=OFF
            -DINSTALL_BIN_EXAMPLES=OFF
            -DINSTALL_C_EXAMPLES=OFF
            -DINSTALL_PYTHON_EXAMPLES=OFF
            -DINSTALL_ANDROID_EXAMPLES=OFF
            -DINSTALL_TO_MANGLED_PATHS=OFF
            -DINSTALL_TESTS=OFF
            -DBUILD_opencv_calib3d=ON
            -DBUILD_opencv_core=ON
            -DBUILD_opencv_dnn=OFF
            -DBUILD_opencv_features2d=ON
            -DBUILD_opencv_flann=ON
            -DBUILD_opencv_gapi=OFF
            -DBUILD_opencv_highgui=OFF
            -DBUILD_opencv_imgcodecs=ON
            -DBUILD_opencv_imgproc=ON
            -DBUILD_opencv_ml=OFF
            -DBUILD_opencv_objdetect=ON
            -DBUILD_opencv_photo=OFF
            -DBUILD_opencv_stitching=OFF
            -DBUILD_opencv_video=OFF
            -DBUILD_opencv_videoio=OFF
            -DWITH_PNG=ON
            -DWITH_JPEG=OFF
            -DWITH_TIFF=OFF
            -DWITH_WEBP=OFF
            -DWITH_OPENJPEG=OFF
            -DWITH_JASPER=OFF
            -DWITH_OPENEXR=OFF
            -DWITH_FFMPEG=OFF
            -DWITH_GSTREAMER=OFF
            -DWITH_1394=OFF
            -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DWITH_PROTOBUF=OFF
            -DBUILD_PROTOBUF=OFF
            -DWITH_CAROTENE=OFF
            -DWITH_EIGEN=OFF
            -DWITH_OPENVX=OFF
            -DWITH_CLP=OFF
            -DWITH_DIRECTX=OFF
            -DWITH_VA=OFF
            -DWITH_LAPACK=ON
            -DWITH_QUIRC=OFF
            -DWITH_ADE=OFF
            -DWITH_ITT=OFF
            -DWITH_OPENCL=OFF
        )

ExternalProject_Get_Property(OpenCVDownload INSTALL_DIR)
add_library(OpenCV::Calib3d STATIC IMPORTED)
set_target_properties(OpenCV::Calib3d PROPERTIES IMPORTED_LOCATION "${INSTALL_DIR}/lib/libopencv_calib3d.a")

add_library(OpenCV::Core STATIC IMPORTED)
set_target_properties(OpenCV::Core PROPERTIES IMPORTED_LOCATION "${INSTALL_DIR}/lib/libopencv_core.a")

add_library(OpenCV::Features2d STATIC IMPORTED)
set_target_properties(OpenCV::Features2d PROPERTIES IMPORTED_LOCATION "${INSTALL_DIR}/lib/libopencv_features2d.a")

add_library(OpenCV::Flann STATIC IMPORTED)
set_target_properties(OpenCV::Flann PROPERTIES IMPORTED_LOCATION "${INSTALL_DIR}/lib/libopencv_flann.a")

add_library(OpenCV::Imgcodecs STATIC IMPORTED)
set_target_properties(OpenCV::Imgcodecs PROPERTIES IMPORTED_LOCATION "${INSTALL_DIR}/lib/libopencv_imgcodecs.a")

add_library(OpenCV::Imgproc STATIC IMPORTED)
set_target_properties(OpenCV::Imgproc PROPERTIES IMPORTED_LOCATION "${INSTALL_DIR}/lib/libopencv_imgproc.a")

add_library(OpenCV::Objdetect STATIC IMPORTED)
set_target_properties(OpenCV::Objdetect PROPERTIES IMPORTED_LOCATION "${INSTALL_DIR}/lib/libopencv_objdetect.a")

add_library(OpenCV::Libpng STATIC IMPORTED)
set_target_properties(OpenCV::Libpng PROPERTIES IMPORTED_LOCATION "${INSTALL_DIR}/lib/opencv4/3rdparty/liblibpng.a")

add_library(OpenCV::Zlib STATIC IMPORTED)
set_target_properties(OpenCV::Zlib PROPERTIES IMPORTED_LOCATION "${INSTALL_DIR}/lib/opencv4/3rdparty/libzlib.a")

set_target_properties(
        OpenCV::Calib3d
        OpenCV::Core
        OpenCV::Features2d
        OpenCV::Flann
        OpenCV::Imgcodecs
        OpenCV::Imgproc
        OpenCV::Objdetect
        OpenCV::Libpng
        OpenCV::Zlib
        PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include/opencv4
)
add_library(OpenCV INTERFACE)
target_link_libraries(OpenCV INTERFACE
        OpenCV::Calib3d
        OpenCV::Core
        OpenCV::Features2d
        OpenCV::Flann
        OpenCV::Imgcodecs
        OpenCV::Imgproc
        OpenCV::Objdetect
        OpenCV::Libpng
        OpenCV::Zlib
        )
if(APPLE)
    target_link_libraries(OpenCV INTERFACE "-framework Accelerate")
endif(APPLE)

add_dependencies(OpenCV OpenCVDownload)

add_library(pokemon-detector-sv STATIC)
target_sources(pokemon-detector-sv PRIVATE pokemon-detector-sv.cpp PUBLIC FILE_SET
        HEADERS BASE_DIRS ${PROJECT_SOURCE_DIR} FILES pokemon-detector-sv.h)
target_link_libraries(pokemon-detector-sv OpenCV)
add_executable(pokemon-detector-sv-exe main.cpp)
target_link_libraries(pokemon-detector-sv-exe OpenCV)

install(TARGETS pokemon-detector-sv FILE_SET HEADERS)
