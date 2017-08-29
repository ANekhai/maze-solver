cmake_minimum_required(VERSION 2.6)
project( MazeSolver )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set (project_sources 
	main.cpp
	maze.cpp
)

add_executable( MazeSolver ${project_sources} )
target_link_libraries( MazeSolver ${OpenCV_LIBS} )