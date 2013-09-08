/*
*
* Author: Ayush Shrestha
* License: GPL v2
*
* Summary:
* Force Directed Edge Bundling (FDEB) algorithm imlemented for parallel coordinates. I followed the paper 
* "Force-Directed Edge Bundling for Graph Visualization" by Holten and Wijk for this. The paper can be 
* found at http://www.win.tue.nl/~dholten/papers/forcebundles_eurovis.pdf
* This implementation has not been optimized so the running time is O(n^3). The most of the time is consumed 
* in the computation of the forces. There is ample room for parallelization - I just didn't have time to do it 
* yet. Maybe I'll do that in one of the coming weekends.
*
*/

#include "fdeb.h"

using namespace std;

// The main data structure
float*** mMatrix;

//
// This functions segments a given line. Since its parallel coordinates, the height in each of the axes and
// distance is sufficient to calculate the slope. a, b represent the heights and w represents the space between
// the axes. s is the number of segments. So if s = 5, the line would be divided into 5 line segments resulting
// in  s+1 coordinates.
//
float** segmentLine(float a, float b, float w, int s){
	float slope = (b-a)/w;
	float intercept = a;	
	float incx = w/s;	

	float** r = (float **)malloc((s+1) * sizeof(float*));
	for(int j=0;j<=s;j++){
		float* s = (float*)malloc(3*sizeof(float));
		s[x] = incx * j;
		s[y] = (slope * s[0]) + intercept;
		s[kp]	= K/(pow(pow(b-a,2)+w*w,0.5));	
		
		r[j] = s;
	}
	return r;
}


//
// This function calculates the forces due to the lines being at angles. Plus it ensures that the force is not
// computed when the lines are orthogonal to each other.
//
float calcCa(float** line1, float** line2, int segments, int width){
	float v1y = line1[segments][y] - line1[0][y];
	float v2y = line2[segments][y] - line2[0][y];
	float w2 = width*width;
	return (w2+v1y*v2y)/(pow(w2+pow(v1y,2),0.5)*pow(w2+pow(v2y,2),0.5));
}


// The main function to calculate teh force - the one contributing to O(n^3)
void calcForce(float*** g, int lines, int segments){
	float t;
	for(int i=0;i<lines;i++){
		for(int j=1;j<segments;j++){
			for(int k=0;k<lines;k++){
				if(i!=k){
					t = -1.0/(g[i][j][y]-g[k][j][y]);
					if(fabs(t)<1.0){
						g[i][j][y] += t*calcCa(g[i], g[k], segments, WIDTH);	
					}
				}
			}
			g[i][j][y] += g[i][j][kp]*(g[i][j-1][y] - 2*g[i][j][y] + g[i][j+1][y]);
		}
	}
}

// OpenGL function to draw
void draw(){
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glBegin(GL_LINES);
		glColor3f(1.0f,1.0f,0.0f);
		for(int i=0; i<LINES;i++){
			for(int j=1;j<=SEGMENTS;j++){
				//printf("%d\t",j);
				glVertex2f(mMatrix[i][j-1][x],mMatrix[i][j-1][y]);
				glVertex2f(mMatrix[i][j][x],mMatrix[i][j][y]);
			}
		}
		
		glColor3f(1.0f,0.0f,0.0f);
		for(int i=0; i<LINES;i++){
			//printf("\n%.2f, %.2f",mMatrix[i][SEGMENTS][x],mMatrix[i][SEGMENTS][y]);
			glVertex2f(mMatrix[i][0][x],mMatrix[i][0][y]);
			glVertex2f(mMatrix[i][SEGMENTS][x],mMatrix[i][SEGMENTS][y]);
		}

	glEnd();
	glFlush();
}

int main(int argc, char** argv){
	float ***genpts;
	
	genpts = (float***)malloc(LINES * sizeof(float**));

	// generate points
	printf("\nGenerating lines...");
	for(int i=0;i<LINES;i++){
		genpts[i] = segmentLine(rand()%HEIGHT,rand()%HEIGHT, WIDTH, SEGMENTS);
	}
	printf("OK");
	
	
	printf("\nCalculating Forces...");
	
	// Calculating the forces for given number of iterations - see fdeb.h
	for(int i=0;i<ITERATIONS;i++){
		calcForce(genpts, LINES, SEGMENTS);
	}
	printf("OK");
	
	mMatrix = genpts;
	
	printf("\nDrawing Lines...");
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_SINGLE);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(WIDTH+5,HEIGHT+5);
	glutCreateWindow("Edge Bundled");
	
	glutDisplayFunc(draw);	
	
	glViewport(0,0,WIDTH+5,HEIGHT+5);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();	
	
	glOrtho(0,HEIGHT,0,WIDTH,1,-1);
		
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();		
	
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	
	glutMainLoop();
	printInnerYValues(genpts, LINES, SEGMENTS);
	
	// free the memory
	for(int i=0;i<LINES;i++){
		for(int j=0;j<2;j++)
			free(genpts[i][j]);
		free(genpts[i]);
	}
	free(genpts);
	return 0;
}




