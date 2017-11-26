cmake_minimum_required (VERSION 2.6)
project (mumu)

FILE(GLOB_RECURSE MAIN_SRC "./src/*")

add_executable(mumu ${MAIN_SRC})