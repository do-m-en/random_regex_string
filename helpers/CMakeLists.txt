cmake_minimum_required(VERSION 3.0.0)

project(random_regex_string CXX)

add_definitions(-std=c++11)

add_executable(unicode_list_gen unicode_list_gen.cpp)
add_executable(unicode_utf8_list_gen unicode_utf8_list_gen.cpp)
add_executable(unicode_list_gen_formatted_array unicode_list_gen_formatted_array.cpp)
