cmake_minimum_required(VERSION 3.28)
project(Ticket_System_2025_master)

set(CMAKE_CXX_STANDARD 20)

add_executable(Ticket_System_2025_master main.cpp
        user.h
        train.h
        ticket.h
        DataBase.h
        cmd.h
        user.cpp
        cmd.cpp
        train.cpp)
