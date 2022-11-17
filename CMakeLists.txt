cmake_minimum_required(VERSION 3.0.0)

project(zeroCodeServer C CXX)

add_subdirectory(./library/mysqlconnectpoll)

add_subdirectory(./src)
