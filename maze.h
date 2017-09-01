#include <opencv2/opencv.hpp>
#include <list>

#ifndef _maze_h_
#define _maze_h_

//Helper Node Class
//Nodes are used to simplify maze down to segments where interesting
//things happen, namely splits in the path

class Node{
public:
	//constructor
	Node() : up_(NULL), down_(NULL), left_(NULL), right_(NULL),
			 sink_(0), visited_(0), i(0), j(0) {}

	//Member Variables
	Node* up_;
	Node* down_;
	Node* left_;
	Node* right_;
	bool sink_;
	bool visited_;
	unsigned int i; unsigned int j; //coordinates of pixel

};


class Maze{
protected:
	Node* source_;
	int size_;
	std::list<Node*> nodes;
	std::list<Node*> solution;

	//functions
	Node* getSource() {return source_;}

public:
	//Constructor and Destructor
	Maze(); //Empty constructor does nothing
	~Maze();
	Maze(cv::Mat& I); //constructor converts image to graph

	//member functions
	virtual void solveMaze() = 0;
	void printSolution();
	void outputSolution(cv::Mat& I);
};


class DFS : public Maze{
public:
	DFS(cv::Mat& I) : Maze(I) {}
	void solveMaze();
protected:
	void solveMaze(Node* ptr, bool& flag); //recursive solve maze function
};


class BFS : public Maze{
public:
	BFS(cv::Mat& I) : Maze(I) {}
	void solveMaze();
};


class Dijkstra : public Maze{
public:
	Dijkstra(cv::Mat& I) : Maze(I) {}
	void solveMaze();

};

class Astar : public Maze{
public:
	Astar(cv::Mat& I) : Maze(I) {}
	void solveMaze();
};

#endif		/* _maze_h_ */
