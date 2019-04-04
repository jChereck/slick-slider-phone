#include "rand.h"
#include "mat.h"
#include <math.h>

double kmeans(Matrix mIn, int K);
double pointDist(Matrix mP1, int p1R, Matrix mP2, int p2R);
double eucDist(double x1, double y1, double x2, double y2);

int main(int argc, char *argv[]){

	//Initialize randomization for generating K centroids
	initRand();


	//Input validation
	if(argc != 3){
		printf("Please enter K and T on the command line and nothing else\n");
		return 1;
	}

	//Parse K and T from command line input
	int K = atoi(argv[1]);
	int T = atoi(argv[2]);

	//Read points matrix
	Matrix mIn("Raw Points Matrix");
	mIn.read();

	//Run K-means algorithm T times
	for( int times = 0; times < T; times++){

		kmeans(mIn, K);

	}

}


double kmeans(Matrix mIn, int K){

	//Initialize K centroids
	Matrix mCent(K,2,0.0);
	double x_min = mIn.minCol(0);
	double x_max = mIn.maxCol(0);
	double y_min = mIn.minCol(1);
	double y_max = mIn.maxCol(1);
	
	mCent.randCol(0,x_min,x_max);
	mCent.randCol(1,y_min,y_max);

	printf("xmin %f xmax%f\tymin%f ymax%f\n",x_min, x_max, y_min, y_max);
	mCent.print();
	


	//Setup for K-means algorithm
	bool converged = false;

	//mX is mIn plus third row to keep track of centroid each point is associated with
	Matrix mX(mIn.numRows(), 3, -1.0);
	mX.insert(mIn,0,0);

	while( !converged ){
		//set flag to flip of any points change centroids
		converged = true;
		
		//Match points to centroids
		for( int p = 0; p < mX.numRows(); p++ ){
			//initialize to a distance >= min dist
			double minDist = pointDist(mX, p, mCent, 0);
			//keep track of closest centroid
			int closest = 0;
			
			for( int c = 0; c < mCent.numRows(); c++ ){
				
				int curDist = pointDist(mX, p, mCent, c);
				if( minDist > curDist ){
					minDist = curDist;
					closest = c;
				}
			}
			
			//Update closest centroid if changed
			if( mX.get(p,2) != closest ){
				mX.set(p,2,closest);
				converged = false;
			}
				
		}
	
		//move centroids
		double centMeans[K][3] = {}
		for( int p = 0; p < mX.numRows(); p++ ){
			centMeans[mX.get(p,2)][1] += mX.get(p,1

	}

	//print out data on converged points
	printf("Done!\n");
	mCent.print();

	return 0.0;
}

double pointDist(Matrix mP1, int p1R, Matrix mP2, int p2R){
	return eucDist(mP1.get(p1R,0), mP1.get(p1R,1), mP2.get(p2R,0), mP2.get(p2R,1));
}

double eucDist(double x1, double y1, double x2, double y2){
	return sqrt( pow( (x1-x2), 2 ) + pow( (y1-y2), 2 ) );
}
