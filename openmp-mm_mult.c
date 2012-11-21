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
	int nprocs, iam, chunk = 10, i , j;
	double *mat1, *mat2, *sol;
	double time, start, finish, Mflops;

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
		mat1 = (double *) malloc(mat_size2*10);
		mat2 = (double *) malloc(mat_size2*10);
		sol = (double *) malloc(mat_size2*10);

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

#if defined (_OPENMP)
		start = omp_get_wtime();
#endif

		// note: nprocs is shared so we can use it later
		#pragma omp parallel shared(mat1, mat2, sol, nprocs) private(iam, i, j)
		{
#if defined (_OPENMP)
			iam = omp_get_thread_num();
			if (iam == 0) {
				nprocs = omp_get_num_threads();
			}
#endif

			// Where the magic occurs.
			#pragma omp for schedule (static, chunk)
			for (i = 0; i < mat_size; i++)
				for (j = 0; j < mat_size; j++)
					sol[i*mat_size+j] += mat1[i*mat_size+j] * mat2[j*mat_size+i];
		}

#if defined (_OPENMP)
		finish = omp_get_wtime();
#endif

		time = finish - start;
		if(time==0.) {
			printf("No hay suficiente precision\n");
		}
		else {
			Mflops=((2.*mat_size2)/time)/1000000.;
			printf("\n>>> Threads = %d\t\tSize = %d\t\tSeconds = %.6lf", nprocs, mat_size, time);
			printf("\n> Mflops = %.6lf\t\tMflops/thread = %.6lf",Mflops,Mflops/nprocs);
#if defined (_OPENMP)
			printf("\n> Precision omp_get_wtick = %lf\n",omp_get_wtick());
#endif
		}

		/* For testing purposes only.
		for (i = 0; i < mat_size; i++) {
			for (j = 0; j < mat_size; j++)
				printf("\t%f",sol[i*mat_size+j]);
			printf("\n");
		}*/

		// Free memory like a boss.
		free(mat1);
		free(mat2);
		free(sol);
	}

	return EXIT_SUCCESS;
}
