#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>
#include<unistd.h>
#include<sys/types.h>
#include<limits.h>
static const long Num_To_Add = 1000000000;
static const double Scale = 10.0 / RAND_MAX;

long add_serial(const char* numbers) {
    long sum = 0;
    for (long i = 0; i < Num_To_Add; i++) {
        sum += numbers[i];
    }
    return sum;
}

long add_parallel(const char* numbers) {
    long sum = 0;
 for (long i = 0; i < Num_To_Add; i++) {
        sum += numbers[i];
    }
    return sum;
}

int main() {
    char *numbers = malloc(sizeof(long) * Num_To_Add);

    long chunk_size = Num_To_Add / omp_get_max_threads();
    int th=omp_get_max_threads();
    //To Check the total number of threads in a system 
    printf("Total number of Threads in system are %d",th);

        int p = omp_get_thread_num();
        unsigned int seed = (unsigned int) time(NULL) + (unsigned int) p;
        long chunk_start = p * chunk_size;
        long chunk_end = chunk_start + chunk_size;
        #pragma omp parallel
        {
		for (long i = chunk_start; i < chunk_end; i++)
		{
        	numbers[i] = (char) (rand() * Scale);
        }
		}
        //To Check which numbers are passed
    	/*for(long i=0;i<sizeof(numbers);i++)
    	{
    		printf("%d\n",numbers[i]);
		}*/

    struct timeval start, end;

    printf("\nTiming sequential...\n");
    gettimeofday(&start, NULL);
    long sum_s = add_serial(numbers);
    gettimeofday(&end, NULL);
    printf("Took %f seconds\n\n", end.tv_sec - start.tv_sec + (double) (end.tv_usec - start.tv_usec) / 1000000);
    printf("Timing parallel...\n");
	#pragma omp parallel num_threads(omp_get_max_threads()) 
    gettimeofday(&start, NULL);
    long sum_p = add_parallel(numbers);
    gettimeofday(&end, NULL);
    printf("Took %f seconds\n\n", end.tv_sec - start.tv_sec + (double) (end.tv_usec - start.tv_usec) / 1000000);

    printf("Sum serial: %ld\nSum parallel: %ld", sum_s, sum_p);

    free(numbers);
    return 0;
}

