/**
 * Beau Albritton
 * Vector addition CUDA program.
 * 
 * This program performs vector addition on the GPU
 * using nvidia's CUDA language. Initializes two vectors (float *)
 * x and y, such that 
 * x[i] is a uniformly distributed random number within (0,1)
 * y is the complement of x[i] or just y = 1-x[i].
 * 
 * Allocates memory on both host (cpu) and device (gpu), copying
 * input vectors from the cpu to gpu. Then, it performs
 * parallel vector addition using a CUDA kernel `vecadd`
 * 
 * GPU time is measured in MS and max absolute error (from expected)
 * is computed after the kernel finishes.
 */
#include <stdlib.h>
#include <cuda_runtime.h>
#include <stdio.h>
#include <math.h>
#define N (256*256*256)
__global__
void vecadd(float* x, float* y, float* z, int n);

int main()
{
	//Vectors stored on the GPU (device)
	float* d_x, *d_y, *d_z;
	//Vectors stored on the CPU (host)
	float* h_x, *h_y, *h_z;
	int n = N;

	//using blocksize 256 for each thread block
	const int blockSize = 256;//256 threads for each thread block
	//Number of blocks per thread
	int numBlocks = (n + blockSize - 1)/blockSize;	

	//Calling malloc to allocate memory for CPU vectors. 
	//size of float (in bytes) multiplied by N (defined as 256^3)
	h_x = (float *) malloc(n*sizeof(float));
	h_y = (float *) malloc(n*sizeof(float));
	h_z = (float *) malloc(n*sizeof(float));

	//cudaMalloc allocates memory on the GPU instead.
	cudaMalloc( (void **) &d_x, n*sizeof(float));
	cudaMalloc( (void **) &d_y, n*sizeof(float));
	cudaMalloc( (void **) &d_z, n*sizeof(float));

	//Setting each x[i] in the vector x to a random number within (0,1)
	for(int i = 0; i < n; ++i)
	{
		h_x[i] = drand48();
		//y is the complement 
		h_y[i] = 1.0f-h_x[i];

	}
	//Copying the results computed in the CPU to the GPU
	cudaMemcpy(d_x, h_x, n*sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_y, h_y, n*sizeof(float), cudaMemcpyHostToDevice);
	
	//Creating events to measure GPU time. Recording starttime first
	cudaEvent_t start,stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start,0);

	//Calling the kernel so everything runs in parallel
	vecadd<<<numBlocks,blockSize>>>(d_x,d_y,d_z,N);

	//Now recording finish time
	cudaEventRecord(stop,0);
	cudaEventSynchronize(stop);

	//Then getting elapsed time from start and stop events
	float elapsedTime;
	cudaEventElapsedTime(&elapsedTime, start, stop);

	//synchronizing host & device
	cudaDeviceSynchronize();
	//copying calculated result of z[i] from GPU to the CPU (for error checking)
	cudaMemcpy(h_z, d_z, n*sizeof(float), cudaMemcpyDeviceToHost);
	//Then destroying events
	cudaEventDestroy(start);
	cudaEventDestroy(stop);	

	//Now checking for max absolute error
	float errorSum=0.0f;

	for(int i = 0; i < n; ++i)
	{
		errorSum += fabs(1-h_z[i]);		
	}
	//printing
	printf("GPU time elasped: %.4f ms\n", elapsedTime);
	printf("Max Errors: %f", errorSum);

	//Now freeing up all the memory allocated on both cpu & gpu

	free(h_x); free(h_y); free(h_z);
	cudaFree(d_x); cudaFree(d_y); cudaFree(d_z);

	return 0;
}

__global__
void vecadd(float* x, float* y, float* z, int n)
{
	//Computes the current index, effectively z[i] since kernel
	//Runs in parallel. Pretty much asking "what block am I in?"
	//and "Within that block, which thread?"
	int tid = blockIdx.x * blockDim.x + threadIdx.x;
	//ensure no out of bounds
	if(tid < n)
	{
		//summing the two (should be 1 since y = 1-x[i])
		z[tid] = x[tid]+y[tid];
	}
}
