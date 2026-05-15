cmake_minimum_required(VERSION 3.31)

project(IkedaMap)

add_executable(ikeda)

find_package(FFTW3 REQUIRED)

target_sources(ikeda
  PRIVATE
    src/main.cpp
)

add_library(config)
target_sources(config
  PRIVATE
    src/config.cpp

  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      src
    FILES
      src/config.h
)

add_library(split_step)
target_sources(split_step
  PRIVATE
    src/split_step.cpp

  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      src
    FILES
      src/config.h
      src/split_step.h
)

target_link_libraries(ikeda
  PRIVATE
    config
    split_step
    fftw3
)
