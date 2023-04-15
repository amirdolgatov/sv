cmake_minimum_required(VERSION 3.23)
project(SV_Project)

set(CMAKE_CXX_STANDARD 20)

add_executable(SV_Project main.cpp sv_classes.cpp EthernetFrame.h SV_attributes.h SV_Frame.h SV_Frame.cpp raw_socket.h Raw_Socket.cpp " Sender.h")
