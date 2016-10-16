cmake_minimum_required(VERSION 3.0.0)

project(random_regex_string CXX)

add_definitions(-std=c++1z)

add_library(${PROJECT_NAME}
  random_regex_string.cpp

  ./nodes/group_regex_node.cpp
  ./nodes/or_regex_node.cpp
  ./nodes/range_random_regex_node.cpp
  ./nodes/range_regex_node.cpp
  ./nodes/negated_range_regex_node.cpp
  ./nodes/random_regex_node.cpp
  ./nodes/repeat_zero_regex_node.cpp
  ./nodes/optional_regex_node.cpp
  ./nodes/repeat_one_regex_node.cpp
  ./nodes/repeat_regex_node.cpp
  ./nodes/repeat_min_regex_node.cpp
  ./nodes/repeat_range_regex_node.cpp
  ./nodes/whitespace_regex_node.cpp
  ./nodes/literal_regex_node.cpp

  unicode/blocks.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC ./nodes)

add_executable(test_rrs test_main.cpp)
target_include_directories(test_rrs PUBLIC ./nodes)
target_link_libraries(test_rrs ${PROJECT_NAME})
