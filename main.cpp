#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/opencv.hpp>
#include "maze.h"

int main(int argc, char* argv[]){
	
////////////////////////////////////////////////////////////////////////
//CHECK INPUT///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

	if(argc == 2 && std::string(argv[1]) == "--help"){
		std::cout << std::endl;
		std::cout << "Correct usage is " << argv[0] << " " << "option Maze_Image" << std::endl << std::endl;
		std::cout << "Options include:" << std::endl
		          << "-dfs for Depth First Search solver" << std::endl 
				  << "-bfs for Breadth First Search solver" << std::endl
				  << "-d for Djikstra solver" << std::endl
				  << "-a for A* solver" << std::endl << std::endl;

		std::cout << "NOTE: Insert note here..." << std::endl << std::endl;
		return 1; 
	}else if(argc != 3){
		std::cout << std::endl; 
		std::cout << "Correct usage is " << argv[0] << " " << "option Maze_Image" << std::endl;
		std::cout << "Type " << argv[0] << " --help to view options" << std::endl << std::endl; 
		return 1; 
	}else if(!(std::string(argv[1]) == "-dfs" || std::string(argv[1]) == "-bfs" 
			 || std::string(argv[1]) == "-d" || std::string(argv[1]) == "-a") ){
		std::cout << std::endl;
		std::cout << "Error: No correct option specified" << std::endl;
		std::cout << "Type " << argv[0] << " --help to view options" << std::endl << std::endl;
		return 1; 
	}

////////////////////////////////////////////////////////////////////////
//OPEN IMAGE////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

	cv::Mat img;
	img = cv::imread(argv[2], cv::IMREAD_COLOR);

	if(std::string(argv[1]) == "-dfs"){
		DFS maze(img);
		maze.solveMaze();
		maze.outputSolution(img);
	}else if(std::string(argv[1]) == "-bfs"){

	}

	return 0;
}
