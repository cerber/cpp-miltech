cmake_minimum_required(VERSION 3.20)

# Окремий CMake-проєкт для домашньої роботи 06. Його можна збирати як
# через root CMakeLists.txt, так і напряму з цієї директорії.
project(balistics CXX)

# Код домашньої роботи збирається з тим самим стандартом, що й лекційні приклади.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Один executable для задачі з balistics.
add_executable(balistics_test src/main.cpp)

# Creates a static library named balistics
add_library(balistics STATIC src/balistics.cpp)

# Add the 'include' folder to the search path for balistics
target_include_directories(balistics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
# Базові compiler warnings допомагають побачити підозрілі місця до runtime.
target_compile_options(balistics PRIVATE -Wall -Wextra -pedantic)
target_compile_options(balistics_test PRIVATE -Wall -Wextra -pedantic)

# Link the balistics library to balistics_test
target_link_libraries(balistics_test PRIVATE balistics)
