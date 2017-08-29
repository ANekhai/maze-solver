#include "maze.h"
#include <opencv2/opencv.hpp>
#include <list>
#include <string>
#include <deque>
#include <utility>


//Empty constructor does nothing
Maze::Maze() {}

//DESTRUCTOR
Maze::~Maze(){
	std::list<Node*>::iterator itr;
	for(itr = nodes.begin(); itr != nodes.end(); ++itr){
		delete *itr; 
	}

}

//CONSTRUCTOR
Maze::Maze(cv::Mat& I){
	//set size of graph to be zero
	size_ = 0; 

	//Store number of rows, columns, and channels in image
	int rows, cols, chans;
	rows = I.rows;
	cols = I.cols;
	chans = I.channels();

	//create pointers to access rows in image
	uchar *curr_row, *row_above, *row_below;

	//create queues to store nodes nodes horizontally and vertically above
	std::deque<Node*> vertical;
	std::deque<Node*> new_vertical;
	std::deque<Node*> horizontal;

	//create pointers to use to link nodes together
	Node* above = NULL;
	Node* left = NULL;


	for (unsigned int i = 0; i < rows; ++i){

		//get pointer to i-th row
		curr_row = I.ptr<uchar>(i);
		if (i != 0) row_above = I.ptr<uchar>(i-1);
		if (i != rows-1) row_below = I.ptr<uchar>(i+1);

		for (unsigned int j = 0; j < cols; ++j){
			//skip all black squares
			if(curr_row[j*chans] == 0) continue;

			//check if row is first or last
			if (i == 0){ //check for first row
				//create new node
				Node* ptr = new Node;
				nodes.push_back(ptr);
				ptr->i = i; ptr->j = j; 
				source_ = ptr;
				//add to queue to link to other node later
				new_vertical.push_back(ptr);
				continue; 
			}else if(i == rows-1){ //check for last row
				//create leaf node for graph
				Node* ptr = new Node;
				nodes.push_back(ptr); 
				ptr->i = i; ptr->j = j;
				ptr->sink_ = 1; 

				//link leaf node vertically
				above = vertical.front();
				above->down_ = ptr;
				ptr->up_ = above; 
				continue;
			}

			//check if in first or last column
			if(j == 0){
				//On left boundary of image
				//Check for seven cases that can occur:
				//(Note: Ws represent maze squares, Bs represent walls)
				//
				// 1.    2.    3.    4.    5.    6.	   7.
				// W     W 	   W 	 B     W     B 	   B 
				// WB or WW or WB or WB or WW or WW	or WW
				// W 	 W 	   B 	 W     B     W 	   B 

				//Check for case 1 as no new node is created
				if(row_above[j*chans] != 0 && row_below[j*chans] != 0 && curr_row[(j+1)*chans] == 0){
					new_vertical.push_back(vertical.front());
					vertical.pop_front();
					continue;
				}

				//create new node
				Node* ptr = new Node;
				nodes.push_back(ptr);
				ptr->i = i; ptr->j = j;

				//check if pixel above is white
				if (row_above[j*chans] != 0){
					//link to node above
					above = vertical.front();
					vertical.pop_front();

					above->down_ = ptr;
					ptr->up_ = above;
				}

				//check if pixel below is white
				if (row_below[j*chans] != 0){
					//add node to next vertical queue
					new_vertical.push_back(ptr);
				}

				//check if pixel to the right is white
				if(curr_row[(j+1)*chans] != 0){
					//add node to horizontal queue
					horizontal.push_back(ptr);
				}



			}else if(j == cols-1){
				//On right boundary of image
				//check for seven cases that can occur:
				//(Note: Ws represent maze squares, Bs represent walls)
				//
				//1.	2.	  3.	4.	  5. 	6.	  7.
				// W	 W     W	 B	   W	 B	   B
				//BW or WW or BW or BW or WW or WW or WW
				// W	 W     B     W	   B     W     B

				//Check for case 1 as no new node is created
				if(row_above[j*chans] != 0 && row_below[j*chans] != 0 && curr_row[(j-1)*chans] == 0){
					new_vertical.push_back(vertical.front());
					vertical.pop_front();
					continue;
				}

				//create new node
				Node* ptr = new Node;
				nodes.push_back(ptr);
				ptr->i = i; ptr->j = j; 


				//check if pixel above is white
				if (row_above[j*chans] != 0){
					//link to node above
					above = vertical.front();
					vertical.pop_front();

					above->down_ = ptr;
					ptr->up_ = above;
				}

				//check if pixel below is white
				if (row_below[j*chans] != 0){
					//add node to next vertical queue
					new_vertical.push_back(ptr);
				}

				//check if pixel to the left is white
				if(curr_row[(j-1)*chans] != 0){
					//link horizontally
					left = horizontal.front();
					horizontal.pop_front();
					left->right_ = ptr;
					ptr->left_ = left;
				}


			}else{ 
				//pixel being checked is not at edge of image

				//check for one of 15 cases that can occur:
				//(Note: Ws represent maze squares, Bs represent walls)
				//
				//1.     2. 	3.	   4.	  5.     6.		7.	   8.	
				// W      W      W      W 	   W 	  W 	 W 		W
				//WWW or WWB or BWW or BWB or WWW or WWB or BWW or BWB
				// W      W      W      W      B 	  B 	 B      B
				//
				//9.     10.	11.    12.    13.    14.    15.
				// B 	  B 	 B 		B      B      B      B
				//WWW or WWB or BWW	or BWB or WWW or WWB or BWW
				// W 	  W 	 W 		W 	   B	  B      B 

				//check for Case 4 as no new node is created
				if(row_above[j*chans] != 0 && row_below[j*chans] != 0 && 
					curr_row[(j-1)*chans] == 0 && curr_row[(j+1)*chans] == 0){

					new_vertical.push_back(vertical.front());
					vertical.pop_front();
					continue;
				}

				//check for Case 13 as no new node is created
				if(row_above[j*chans] == 0 && row_below[j*chans] == 0 && 
				   curr_row[(j-1)*chans] != 0 && curr_row[(j+1)*chans] != 0){

					continue;
				}

				//create new node
				Node* ptr = new Node;
				nodes.push_back(ptr);
				ptr->i = i; ptr->j = j;

				//check if pixel above is white
				if (row_above[j*chans] != 0){
					//link to node above
					above = vertical.front();
					vertical.pop_front();

					above->down_ = ptr;
					ptr->up_ = above;
				}

				//check if pixel below is white
				if (row_below[j*chans] != 0){
					//add node to next vertical queue
					new_vertical.push_back(ptr);
				}

				//check if pixel to the left is white
				if(curr_row[(j-1)*chans] != 0){
					//link horizontally
					left = horizontal.front();
					horizontal.pop_front();
					left->right_ = ptr;
					ptr->left_ = left;
				}

				//check if pixel to the right is white
				if(curr_row[(j+1)*chans] != 0){
					//add node to horizontal queue
					horizontal.push_back(ptr);
				}

			}

		}

		//update vertical queue for next row
		vertical.clear();
		vertical = new_vertical;
		new_vertical.clear();
		//clear horizontal queue for next row
		horizontal.clear();
	}

	size_ = nodes.size();

}

void Maze::printSolution(){
	std::cout << "SOLUTION:" << std::endl;

	std::list<Node*>::reverse_iterator itr;
	for (itr = solution.rbegin(); itr != solution.rend(); ++itr){
		std::cout << (*itr)->i << ", " << (*itr)->j << std::endl;
	}
}

void drawPath(cv::Mat& I, int i, int prev_i, int j, char dir){
	if(i < prev_i){
		int temp = i;
		i = prev_i;
		prev_i = temp;
	}

	int chans = I.channels();

	for(int k = prev_i; k <= i; ++k){
		if(dir == 'V'){
			uchar *row = I.ptr<uchar>(k);
			row[j*chans+1] = 256;
		}else{
			uchar *row = I.ptr<uchar>(j);
			row[k*chans+1] = 256;
		}
	}
}

void Maze::outputSolution(cv::Mat& I){
	std::list<Node*>::iterator itr = solution.begin();
	int prev_i = (*itr)->i, prev_j = (*itr)->j;
	int i, j; 
	while(itr != solution.end()){
		i = (*itr)->i; j = (*itr)->j;

		if(i-prev_i != 0){
			drawPath(I, i, prev_i, j, 'V');
		}else if(j-prev_j != 0){
			drawPath(I, j, prev_j, i, 'H');
		}

		prev_i = i; prev_j = j;
		++itr; 
	}

	imwrite("output.bmp", I);
}

void DFS::solveMaze(){
	bool flag = false;
	Node* ptr = getSource();
	solveMaze(ptr, flag);
}

void DFS::solveMaze(Node* ptr, bool& flag){
	ptr->visited_ = true;

	if(ptr->sink_){
		flag = true;
	}

	//Recursively traverse graph
	if(ptr->left_ && !flag){ 
		if(!ptr->left_->visited_) solveMaze(ptr->left_, flag);
	}if(ptr->right_ && !flag){
		if(!ptr->right_->visited_) solveMaze(ptr->right_, flag);
	}if(ptr->down_ && !flag){
		if(!ptr->down_->visited_) solveMaze(ptr->down_, flag);
	}if(ptr->up_ && !flag){ 
		if(!ptr->up_->visited_)solveMaze(ptr->up_, flag);
	}

	if(flag){
		solution.push_back(ptr);
	}
}
/*
void BFS::solveMaze(){
	Node* ptr = getSource(), next;
	std::deque<std::pair<Node*, std::string> > queue;
	solution.push_back(ptr);
	std::pair<Node*, std::string> curr_path, next_path;

	//Add source node to queue
	curr_path.first = ptr; curr_path.second = "";
	queue.push_back(curr_path);

	while(!ptr->sink_){
		if(queue.size() == 0){
			std::cerr << "NO SOLUTION FOUND" << std::endl;
			exit(1);
		}

		//pop first element and update current node

		//add all unvisited nodes to queue

		break;
	}


}*/
