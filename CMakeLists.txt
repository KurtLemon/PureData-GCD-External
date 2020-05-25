cmake_minimum_required(VERSION 3.8)
project(gcd)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES m_pd.h gcd.c)
add_library(GCD ${SOURCE_FILES})