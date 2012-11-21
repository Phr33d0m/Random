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
    int mat_size, iN, fN, incN;
	int nprocs, iam, chunk = 50;
	long i, j, k;
	double *mat1, *mat2, *sol;
	double time, start, finish, Mflops;
	time = start = finish = Mflops = 0.;

	if(argc<4) {
		printf("\n\nUSAGE: %s size_initial size_final size_increment\n\n",argv[0]);
		return -1;
	}

	iN=atoi(argv[1]);
	fN=atoi(argv[2]);
	incN=atoi(argv[3]);

	for(mat_size=iN; mat_size<=fN; mat_size+=incN) {
		int mat_size2 = mat_size * mat_size;
		// Allocating memory to the three matrixes.
		mat1 = (double *) malloc(sizeof(double)*mat_size2);
		mat2 = (double *) malloc(sizeof(double)*mat_size2);
		sol = (double *) malloc(sizeof(double)*mat_size2);
		if(mat1==NULL || mat2==NULL || sol==NULL)
			printf("\nError in Matrix allocation. Ask Morpheus.\n");

		// Generating random values between 0 and 1 for mat1 and mat2.
		// About sol, we will simply fill it with zeroes.
		#pragma omp for schedule (dynamic, chunk)
		for (i = 0; i < mat_size2; i++)
			mat1[i] = (double)rand()/RAND_MAX;

		#pragma omp for schedule (dynamic, chunk)
		for (i = 0; i < mat_size2; i++)
			mat2[i] = (double)rand()/RAND_MAX;

		#pragma omp for schedule (dynamic, chunk)
		for (i = 0; i < mat_size2; i++)
			sol[i] = 0;

		start = omp_get_wtime();

		// note: nprocs is shared so we can use it later
		#pragma omp parallel shared(mat1, mat2, sol, nprocs, chunk) private(iam, i, j, k)
		{
			iam = omp_get_thread_num();
			if(iam==0)
				nprocs = omp_get_num_threads();

			// Where the magic occurs.
			#pragma omp for schedule (dynamic)
			for (i = 0; i < mat_size; i++) {
				// This (long x) optimizes the loops a little.
				// Test case: mat1(2000x2000) x mat2(2000x2000)
				//    before: 18.219063
				//     after: 16.466386 seconds
				long x = i*mat_size;
				for (j = 0; j < mat_size; j++)
					for (k = 0; k < mat_size; k++)
						sol[x+j] += mat1[x+k] * mat2[k*mat_size+j];
			}
		}

		finish = omp_get_wtime();

		time = finish - start;
		if(time==0.) {
			printf("\nNot enough precission.\n");
		}
		else {
			Mflops=((2.*mat_size2)/time)/1000000.;
			printf("\n>>> Threads = %d\t\tSize = %d\t\tSeconds = %.6lf", nprocs, mat_size, time);
			printf("\n> Mflops = %.6lf\t\tMflops/thread = %.6lf",Mflops,Mflops/nprocs);
			printf("\n> Precision omp_get_wtick = %lf\n",omp_get_wtick());
		}

		// Free memory like a boss.
		free(mat1);
		free(mat2);
		free(sol);
	}

	return EXIT_SUCCESS;
}
