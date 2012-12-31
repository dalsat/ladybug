cmake_minimum_required (VERSION 2.8)
project (Cocci_model CXX)

#find_package (glut REQUIRED)
set (MODEL_SRC bullet enemy gun)
add_library(model ${MODEL_SRC})
