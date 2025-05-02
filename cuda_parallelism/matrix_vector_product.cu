/**
 * Beau Albritton
 * Matrix-vector Product
 * 
 * This program performs matrix-vector multiplication
 * on the GPU using nvidia's CUDA language.
 * Computes the product y = M * x in parallel,
 * where M is an n x n matrix
 * (one dimensional pointer indexing with M[i*n] for the row)
 * and x is a n x 1 vector.
 * 
 * Each entry in the matrix M and the vector x are initialized to be random
 * numbers in (0,1). 
 * 
 * This program uses unified memory via cudaMallocManaged()
 * for both device and host memory allocation, which removes
 * the need to copy from device to host.
 * 
 * Each thread computes a single row of the resulting vector y.
 * 
 * Execution time is measured on both the device (GPU) and the host (CPU).
 * 
 * The relative error between the two is then calculated.
 */
#include <stdlib.h>
#include <sys/time.h>
#include <cuda_runtime.h>
#include <stdio.h>
#include <math.h>
#define N (5000)

//defining functions
__global__
void mxv(float* M, float* x, float* y, int n);
void cpu_mxv(float* M, float* x, float* y, int n);
float relative_error(float* u, float* v,int n);

int main()
{
	//Unified memory pointers
	float* M, *x, *y;
	float* h_y; //Cpu float* for relative error calculation later (don't  want to override *y as the two need to be compared)
	int n = N;
	//Using blocksize 256 for each thread block
	const int blockSize = 256;
	//Number of blocks per thread
	int numBlocks = (n+blockSize-1)/blockSize;

	//cudaMalloc now manages memory for both host and device
	cudaMallocManaged(&x,n*sizeof(float));
	cudaMallocManaged(&y, n*sizeof(float));
	cudaMallocManaged(&M,n*n*sizeof(float));
	//Allocating memory for cpu vector
	h_y = (float *) malloc(n*sizeof(float));

	//Initializing each item in the vector x to a uniformly distributed random number in (0,1)
	for(int i =0; i < n; ++i)
	{
		x[i] = drand48();
		for(int j = 0; j < n; ++j)
		{
			//same for matrix M
			M[i*n+j] = drand48();
		}

	}
	//Creating events to measure GPU time. Recording starttime first
	cudaEvent_t start,stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start,0);
	//calling kernel for gpu
	mxv<<<numBlocks,blockSize>>>(M,x,y,n);
	//Recording finish time
	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);

	//Calculating elapsed time on the GPU
	float gpuElapsedTime;
	cudaEventElapsedTime(&gpuElapsedTime, start, stop);
	//Synchronize device and host
	cudaDeviceSynchronize();
	cudaEventDestroy(start);
	cudaEventDestroy(stop);

	//Now calling same algorithm on the CPU
	//and measuring it
	struct timeval t1, t2;
	float cpuElapsedTime;
	//startTime
	gettimeofday(&t1, NULL);

	//doing some work (calling cpu MXV)

	cpu_mxv(M,x,h_y,n);
	//finishTime
	gettimeofday(&t2, NULL);

	//Simple elapsed time calculations (to ms)
	cpuElapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;
	cpuElapsedTime += (t2.tv_usec - t1.tv_usec) / 1000;

	//Calculating relative error
	float relError = relative_error(y,h_y,n);

	//Then printing
	printf("cpu time %f ms\n", cpuElapsedTime);
	printf("gpu time %f ms\n", gpuElapsedTime);
	printf("relative error %.3f", relError);

	//Free ma bois
	cudaFree(M);
	cudaFree(x);
	cudaFree(y);

	return 0;
}

__global__
void mxv(float* M, float* x, float* y, int n)
{
	float sum = 0.0;
	//Computes the current row. Pretty much asking "what block am I in?"
	//and "Within that block, which thread?"
	int row = threadIdx.x + blockDim.x *blockIdx.x;

	if(row < n)
	{
		//then for whatever row, multiply that by n (since M is an n x n vector and can only be represented by a 1D pointer float*)
		int idx = row * n;
		for(int col = 0; col < n; ++col)
		{
			sum = sum + M[idx + col] * x[col];
		}
		//y[i] is equal to the matrix-vector product
		y[row]=sum;
	}

}
void cpu_mxv(float* M, float* x, float* y, int n)
{
	//need outer foor loop for cpu since program isn't running in parallel.
	//otherwise exact same algorithm in mxv for gpu.
	for(int row = 0; row < n; ++row)
	{
		float sum = 0;
		for(int col = 0; col < n; ++col)
		{
			int idx = row * n + col;
			sum = sum+ M[idx] * x[col];
		}
		y[row] = sum;
	}
}
/**
 * Computes the relative error between two vectors U,V of size n.
 * Relative error given by || u - v || / || v ||
 * 
 * Simply put, the magnitude of the difference of the vectors U and V
 * divided by the overall magnitude of V.
 */
float relative_error(float* u, float* v,int n)
{
	//Sum for magnitude of the difference (for later square rooting)
	float magnitudeSum = 0.0f;
	//Sum for the mangitude of vec V
	float vSum = 0.0f;
	for(int i=0; i < n; ++i)
	{
		//Then square it and add it to sum according to the Euclidean norm (or just 2-norm)
		//https://en.wikipedia.org/wiki/Norm_(mathematics)#Euclidean_norm
		magnitudeSum += pow(u[i]-v[i], 2);
		vSum += pow(v[i],2);
	}
	//Finally square root 
	float magnitudeU_V = sqrtf(magnitudeSum);
	float magnitudeV = sqrtf(vSum);
	//Return the result 
	return magnitudeU_V / magnitudeV;
}
