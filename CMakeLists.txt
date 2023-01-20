cmake_minimum_required(VERSION 3.16)
project(pokemon-detector-sv)

include(ExternalProject)

string(REPLACE ";" "$<SEMICOLON>" CMAKE_OSX_ARCHITECTURES_ "${CMAKE_OSX_ARCHITECTURES}")
set(CMAKE_CXX_STANDARD 20)

if(MSVC)
  if(${CMAKE_GENERATOR_PLATFORM} STREQUAL x64
     AND ${MSVC_VERSION} GREATER_EQUAL 1910
     AND ${MSVC_VERSION} LESS_EQUAL 1939)
    set(OCV_LIB_PATH x64/vc17/staticlib)
    set(OCV_LIB_PATH_3RD x64/vc17/staticlib)
    set(OCV_LIB_SUFFIX 470)
  else()
    message(FATAL_ERROR "Unsupported MSVC!")
  endif()
else()
  set(OCV_LIB_PATH lib)
  set(OCV_LIB_PATH_3RD lib/opencv4/3rdparty)
  set(OCV_LIB_SUFFIX "")
endif()

ExternalProject_Add(
  o
  URL https://github.com/umireon/opencv/archive/refs/tags/4.7.1.tar.gz
  BUILD_BYPRODUCTS
    <INSTALL_DIR>/${OCV_LIB_PATH}/${CMAKE_STATIC_LIBRARY_PREFIX}opencv_core${OCV_LIB_SUFFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}
    <INSTALL_DIR>/${OCV_LIB_PATH}/${CMAKE_STATIC_LIBRARY_PREFIX}opencv_features2d${OCV_LIB_SUFFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}
    <INSTALL_DIR>/${OCV_LIB_PATH}/${CMAKE_STATIC_LIBRARY_PREFIX}opencv_imgcodecs${OCV_LIB_SUFFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}
    <INSTALL_DIR>/${OCV_LIB_PATH}/${CMAKE_STATIC_LIBRARY_PREFIX}opencv_imgproc${OCV_LIB_SUFFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}
    <INSTALL_DIR>/${OCV_LIB_PATH_3RD}/${CMAKE_STATIC_LIBRARY_PREFIX}libpng${CMAKE_STATIC_LIBRARY_SUFFIX}
    <INSTALL_DIR>/${OCV_LIB_PATH_3RD}/${CMAKE_STATIC_LIBRARY_PREFIX}zlib${CMAKE_STATIC_LIBRARY_SUFFIX}
  CMAKE_GENERATOR ${CMAKE_GENERATOR}
  INSTALL_COMMAND cmake --install <BINARY_DIR> --config ${CMAKE_BUILD_TYPE}
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
             -DCMAKE_BUILD_TYPE=Release
             -DCMAKE_GENERATOR_PLATFORM=${CMAKE_GENERATOR_PLATFORM}
             -DOPENCV_FORCE_3RDPARTY_BUILD=ON
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
             -DBUILD_opencv_calib3d=OFF
             -DBUILD_opencv_core=ON
             -DBUILD_opencv_dnn=OFF
             -DBUILD_opencv_features2d=ON
             -DBUILD_opencv_flann=OFF
             -DBUILD_opencv_gapi=OFF
             -DBUILD_opencv_highgui=OFF
             -DBUILD_opencv_imgcodecs=ON
             -DBUILD_opencv_imgproc=ON
             -DBUILD_opencv_ml=OFF
             -DBUILD_opencv_objdetect=OFF
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
             -DWITH_IPP=OFF
             -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES_})

ExternalProject_Get_Property(o INSTALL_DIR)

add_library(OpenCV::Core STATIC IMPORTED)
set_target_properties(
  OpenCV::Core
  PROPERTIES
    IMPORTED_LOCATION
    ${INSTALL_DIR}/${OCV_LIB_PATH}/${CMAKE_STATIC_LIBRARY_PREFIX}opencv_core${OCV_LIB_SUFFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}
)

add_library(OpenCV::Features2d STATIC IMPORTED)
set_target_properties(
  OpenCV::Features2d
  PROPERTIES
    IMPORTED_LOCATION
    ${INSTALL_DIR}/${OCV_LIB_PATH}/${CMAKE_STATIC_LIBRARY_PREFIX}opencv_features2d${OCV_LIB_SUFFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}
)

add_library(OpenCV::Imgcodecs STATIC IMPORTED)
set_target_properties(
  OpenCV::Imgcodecs
  PROPERTIES
    IMPORTED_LOCATION
    ${INSTALL_DIR}/${OCV_LIB_PATH}/${CMAKE_STATIC_LIBRARY_PREFIX}opencv_imgcodecs${OCV_LIB_SUFFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}
)

add_library(OpenCV::Imgproc STATIC IMPORTED)
set_target_properties(
  OpenCV::Imgproc
  PROPERTIES
    IMPORTED_LOCATION
    ${INSTALL_DIR}/${OCV_LIB_PATH}/${CMAKE_STATIC_LIBRARY_PREFIX}opencv_imgproc${OCV_LIB_SUFFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}
)

add_library(OpenCV::Libpng STATIC IMPORTED)
set_target_properties(
  OpenCV::Libpng
  PROPERTIES
    IMPORTED_LOCATION
    ${INSTALL_DIR}/${OCV_LIB_PATH_3RD}/${CMAKE_STATIC_LIBRARY_PREFIX}libpng${CMAKE_STATIC_LIBRARY_SUFFIX}
)

add_library(OpenCV::Zlib STATIC IMPORTED)
set_target_properties(
  OpenCV::Zlib
  PROPERTIES
    IMPORTED_LOCATION
    ${INSTALL_DIR}/${OCV_LIB_PATH_3RD}/${CMAKE_STATIC_LIBRARY_PREFIX}zlib${CMAKE_STATIC_LIBRARY_SUFFIX}
)

if(MSVC)
  set(OpenCV_INCLUDE_PATH ${INSTALL_DIR}/include)
else()
  set(OpenCV_INCLUDE_PATH ${INSTALL_DIR}/include/opencv4)
endif(MSVC)

set_target_properties(
  OpenCV::Core OpenCV::Features2d OpenCV::Imgcodecs OpenCV::Imgproc OpenCV::Libpng OpenCV::Zlib
  PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${OpenCV_INCLUDE_PATH})
add_library(OpenCV INTERFACE)
target_link_libraries(OpenCV INTERFACE OpenCV::Features2d OpenCV::Imgcodecs OpenCV::Imgproc
                                       OpenCV::Core OpenCV::Libpng OpenCV::Zlib)
if(APPLE)
  target_link_libraries(OpenCV INTERFACE "-framework Accelerate")
endif(APPLE)

install(
  FILES $<TARGET_PROPERTY:OpenCV::Features2d,IMPORTED_LOCATION>
        $<TARGET_PROPERTY:OpenCV::Imgcodecs,IMPORTED_LOCATION>
        $<TARGET_PROPERTY:OpenCV::Imgproc,IMPORTED_LOCATION>
        $<TARGET_PROPERTY:OpenCV::Core,IMPORTED_LOCATION>
        $<TARGET_PROPERTY:OpenCV::Libpng,IMPORTED_LOCATION>
        $<TARGET_PROPERTY:OpenCV::Zlib,IMPORTED_LOCATION>
  TYPE LIB)

add_dependencies(OpenCV o)

add_library(libpokemon-detector-sv STATIC)
target_sources(
  libpokemon-detector-sv
  PRIVATE pokemon-detector-sv.cpp
          SceneDetector.cpp
          PokemonRecognizer.cpp
          PokemonRecognizerGenerated.cpp
          EntityCropper.cpp
          SelectionRecognizer.cpp
          SelectionRecognizerGenerated.cpp
          ResultRecognizer.cpp)

install(FILES ${PROJECT_SOURCE_DIR}/pokemon-detector-sv.h TYPE INCLUDE)
target_link_libraries(libpokemon-detector-sv OpenCV)
target_include_directories(libpokemon-detector-sv PUBLIC ${PROJECT_SOURCE_DIR})

add_executable(pokemon-detector-sv-exe main.cpp)
target_link_libraries(pokemon-detector-sv-exe OpenCV)

add_executable(CalculateSelectScreenHist CalculateSelectScreenHist.cpp SceneDetector.cpp)
target_link_libraries(CalculateSelectScreenHist OpenCV)

add_executable(CalculatePokemonHist CalculatePokemonHist.cpp)
target_link_libraries(CalculatePokemonHist OpenCV)

add_executable(PokemonRecognizerBench PokemonRecognizerBench.cpp PokemonRecognizer.cpp
                                      PokemonRecognizerGenerated.cpp)
target_link_libraries(PokemonRecognizerBench OpenCV)

add_executable(AKAZETrainer AKAZETrainer.cpp)
target_link_libraries(AKAZETrainer OpenCV)

add_executable(EntityCropperPlayground EntityCropperPlayground.cpp EntityCropper.cpp)
target_link_libraries(EntityCropperPlayground OpenCV)

add_executable(SelectionRecognizerBench SelectionRecognizerBench.cpp SelectionRecognizer.cpp
                                        SelectionRecognizerGenerated.cpp)
target_link_libraries(SelectionRecognizerBench OpenCV)

add_executable(ResultRecognizerBench ResultRecognizerBench.cpp ResultRecognizer.cpp)
target_link_libraries(ResultRecognizerBench OpenCV)

install(TARGETS libpokemon-detector-sv)
