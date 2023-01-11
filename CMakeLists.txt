cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
project(IniParser)
add_executable(IniParser "main.cpp" "ReaderEvent.h" "Constants.h" "ReaderEvent.cpp" "ReadIniFile.cpp" "CommentEvent.h" "CommentEvent.cpp" "ReadFinishEvent.h" "ReadFinishEvent.cpp" "ErrorEvent.h" "ErrorEvent.cpp"   "CharUtils.h" "CharUtils.cpp" "FilePos.cpp" "FilePos.h")