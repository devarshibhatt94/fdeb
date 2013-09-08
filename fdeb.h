#include<cstdlib>
#include<cmath>
#include<stdio.h>
#include<iostream>
#include<sstream>
#include<string>
#include<GL/glut.h>
#include<sys/time.h>

#define WIDTH 800			// width of the graph
#define HEIGHT 600			// height of the graph
#define LINES 50			// number of location lines for random generation and testing purposes
#define SEGMENTS 99			// a location line will be segmented to these many segments. i.e. there will be segments+1 points
#define K 100				// spring constant
#define ITERATIONS 200			// number of iterations to calculate the force


// for the ease of use in the arrays
// x: x value
// y: y value
// kp: K/|P| where K is the spring constant and |P| is the initial length of the line
enum {x,y,kp};

struct timeval start, end;

// Wanted to profile the code

void startTimer(){
	gettimeofday(&start,NULL);
}

void stopTimer(){
	gettimeofday(&end,NULL);
	
	long seconds = end.tv_sec - start.tv_sec;
	printf("\nTime taken: %lds",seconds);
}


//
// printing values in the matrix
//
void printArray(float*** g, int lines, int segments, int points){
	for(int i=0;i<lines;i++){
		for(int j=0;j<segments;j++){
			printf("(");
			for(int k=0;k<points;k++){
				printf("%.2f,",g[i][j][k]);
			}
			printf(")");
		}
	printf("\n");
	}
}


//
// only prints the inner y values of each row since the end points are constant and they dont move.
//
void printInnerYValues(float*** g, int lines, int segments){
	for(int i=0;i<lines;i++){
		for(int j=1;j<segments;j++){
			printf("%.2f\t",g[i][j][y]);
		}
	printf("\n");
	}
}



