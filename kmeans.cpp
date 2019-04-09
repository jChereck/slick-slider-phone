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
	int xDims = mIn.numCols();
	Matrix mCent(K,xDims,0.0);

	for( int x = 0; x < xDims; x++ ){
		double min = mIn.minCol(x);
		double max = mIn.maxCol(x);
	
		mCent.randCol(x,min,max);
	}

	//Setup for K-means algorithm
	bool converged = false;

	//mX is mIn plus extra column to keep track of centroid each point is associated with
	Matrix mX(mIn.numRows(), xDims + 1, 0.0);
	mX.insert(mIn,0,0);

	//keep track of means of centroids
	double centMeans[K][xDims+1] = {};

	while( !converged ){
		//set flag to flip if any points change centroids
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
			if( mX.get(p,xDims) != closest ){
				mX.set(p,xDims,closest);
				converged = false;
			}

			//Sum up totals to find mean
			for( int x = 0; x < xDims; x++ ){
				centMeans[ (int) mX.get(p,xDims)][x] += mX.get(p,x);
			}
			centMeans[ (int) mX.get(p,xDims)][xDims]++;
				
		}

		/*
		for( int k = 0; k < K; k++ ){
			for( int x = 0; x < xDims + 1; x++ ){
				printf("%f\t",centMeans[k][x]);
			}
			printf("|\n");
		}	
		*/
	
		//move centroids
		for( int cent = 0; cent < K; cent++ ){
			for( int x = 0; x < xDims; x++ ){
				//if centroid has no points matched to it
				if( centMeans[cent][xDims] == 0.0 ){
					printf("\n\n CENTROID HAS BEEN RELOCATED \n\n");
					//change that centroid to a random point between min/max of inputs
					mCent.insert( mIn.extract( randMod(mIn.numRows()) ,0 ,1 ,0 ), cent, 0 );
					for( int x = 0; x < xDims; x++ ){
						double min = mIn.minCol(x);
						double max = mIn.maxCol(x);

						mCent.set( x, cent, (randUnit() * (max-min)) + min );
					}

				}	
				double newCoord = centMeans[cent][x]/centMeans[cent][xDims];
				mCent.set(cent, x, newCoord);
			}
		}

	}

	//print out data on converged points
	mCent.printfmt("Points:");
	double minD = pointDist(mCent, 0, mCent, 1);
	for( int x = 0; x < K; x++ ){
		for( int y = x + 1; y < K; y++ ){
			double newD = pointDist(mCent, x, mCent, y);
			if( newD < minD ){ minD = newD; }
		}
		
	}
	printf("K: %i  MinD: %f\n", K, minD);

	return 0.0;
}

double pointDist(Matrix mP1, int p1R, Matrix mP2, int p2R){
	return eucDist(mP1.get(p1R,0), mP1.get(p1R,1), mP2.get(p2R,0), mP2.get(p2R,1));
}

double eucDist(double x1, double y1, double x2, double y2){
	return sqrt( pow( (x1-x2), 2 ) + pow( (y1-y2), 2 ) );
}
