/*
 * File:   main.c
 * Author: Denis M.
 *
 * A very simple OpenMP code for a Matrix-Matrix multiplication.
 *
 * Created on November 4, 2012, 5:03 PM
 */

// Basic Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// OpenMP Include
#include <omp.h>

int main(int argc, char *argv[])
{
    short iN, fN, incN;
	int nprocs = 0, iam = 0, mat_size, i, j, k;
	float time, start, finish;
	double *mat1, *mat2, *sol, mflops;

	if(argc<4) {
		printf("\n\nUSAGE: %s size_initial size_final size_increment\n\n",argv[0]);
		return -1;
	}

	iN=atoi(argv[1]);
	fN=atoi(argv[2]);
	incN=atoi(argv[3]);

	for(mat_size=iN; mat_size<=fN; mat_size+=incN) {
		int mat_size2 = mat_size * mat_size;

		// Allocating memory to the three matrix.
		mat1 = (double *) malloc(sizeof(double)*mat_size2);
		mat2 = (double *) malloc(sizeof(double)*mat_size2);
		sol = (double *) malloc(sizeof(double)*mat_size2);
		if(mat1==NULL || mat2==NULL || sol==NULL)
			printf("\nError in Matrix allocation. Ask Morpheus.\n");

		// Generating random values between 0 and 1 for mat1 and mat2.
		// About sol, we will simply fill it with zeroes.
		#pragma omp for schedule (dynamic, 20)
		for (i = 0; i < mat_size2; i++) {
			mat1[i] = (double)rand()/RAND_MAX;
			mat2[i] = (double)rand()/RAND_MAX;
			sol[i] = 0;
		}

		// note: nprocs is shared because we need it for later use.
		#pragma omp parallel shared(nprocs) private(iam)
		{
		iam=omp_get_thread_num();
		if(iam==0)
			nprocs=omp_get_num_threads();
		}

		// Clock START.
		start = omp_get_wtime();

		// This block contains the matrix multiplication code.
		{
			// This (long x) optimizes the loops a little.
			long x;
			double temp;

			// Where the magic begins.
			#pragma omp parallel for schedule(runtime)
			for (i = 0; i < mat_size; i++) {
				x = i*mat_size;
				for (j = 0; j < mat_size; j++) {
					// Using 'reduction' with a temp variable
					// optimizes the calculation a lot!
					temp = 0;
					#pragma omp parallel for reduction(+:temp)
					for (k = 0; k < mat_size; k++)
						temp = mat1[x+k] * mat2[k*mat_size+j];
					sol[x+j] = temp;
				}
			}
		}

		// Clock STOP.
		finish = omp_get_wtime();

		time = finish - start;

		if(time==0.) {
			printf("\nNot enough precission.\n");
		}
		else {
			// >>> mflops = (operations/time)/1000000
			// > operations = (first for()) + (second for()) + (third for())
			// (third for()) = (2*mat_size) <- because we have two floating point operations
			mflops = ((double)mat_size2*(2.*(double)mat_size)/time)/1000000.;
			printf("\n>>> Threads = %d\t\tSize = %d\t\tSeconds = %.6lf", nprocs, mat_size, time);
			printf("\n> Mflops = %.6f\t\tMflops/thread = %.6f",mflops,mflops/nprocs);
			printf("\n> Precision omp_get_wtick = %lf\n",omp_get_wtick());
		}

		// Free memory like a boss.
		free(mat1);
		free(mat2);
		free(sol);
	}

	return EXIT_SUCCESS;
}
