cmake_minimum_required(VERSION 2.8)
project(imt1082)


file(GLOB SOURCES "*.cpp", "*.h")

add_executable(game ${SOURCES})