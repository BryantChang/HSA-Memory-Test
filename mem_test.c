#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/param.h> 
#include <time.h>
#include "mem_testKernel.h"
#define MB 1024 * 1024
#define NSECPERSEC 1000000000L

void usage()
{
	printf("usage: ./mem_test <cpu/gpu> <read-r/write-w> <the size of memory  MB>\n");
}

long int get_nanosecs( struct timespec start_time, struct timespec end_time) {
   long int nanosecs;
   if ((end_time.tv_nsec-start_time.tv_nsec)<0) nanosecs = 
      ((((long int) end_time.tv_sec- (long int) start_time.tv_sec )-1)*NSECPERSEC ) +
      ( NSECPERSEC + (long int) end_time.tv_nsec - (long int) start_time.tv_nsec) ;
   else nanosecs = 
      (((long int) end_time.tv_sec- (long int) start_time.tv_sec )*NSECPERSEC ) +
      ( (long int) end_time.tv_nsec - (long int) start_time.tv_nsec );
   return nanosecs;
}

void read_mem_only_cpu(int *input, int limit, int *sum) 
{
	int i = 0;
	for(i = 0;i < limit; i++){
		*sum += input[i];
	}
}

void write_mem_only_cpu(int *output, int limit)
{
	int i = 0;
	for(i = 0;i < limit;i++){
		output[i] = i;//write the memory
	}
}



int main(int argc, char **argv)
{
	if(argc < 4) {
		usage();
		return -1;
	}
	struct timespec start_time, end_time;
	long int nanosecs;
	int memsize,memsize_mb;
	int limit,i,sum;
	double mb_per_sec;
	memsize_mb = atoi(argv[3]);
	memsize = memsize_mb * MB;
	limit  = memsize / sizeof(int);
	sum = 0;
	int *input = (int*)malloc(sizeof(int) * limit);
	printf("the size of memory is %d MB\n", memsize_mb);
	if(!strcmp(argv[2], "r")) {
		if(!strcmp(argv[1], "cpu")) {
			printf("you choose the read only test on cpu\n");
			printf("init the memory...\n");
			for(i = 0;i < limit;i++) {
				input[i] = 1;
			}
			printf("init compelete now cpu is reading the mem and comput....\n");
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start_time);
			read_mem_only_cpu(input, limit, &sum);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&end_time);
			nanosecs = get_nanosecs(start_time,end_time);
			mb_per_sec = memsize_mb * 1.0 / nanosecs * NSECPERSEC;
			printf("the sum is %d\n", sum);
			printf("cpu read memory speed is %lfMB per second\n", mb_per_sec);
			printf("all done\n");
		}else if(!strcmp(argv[1], "gpu")){
			printf("you choose the read only test on gpu\n");
			printf("init the memory...\n");
			for(i = 0;i < limit;i++) {
				input[i] = 1;
			}
			SNK_INIT_LPARM(lparm,limit);
			printf("init compelete now cpu is reading the mem and comput....\n");
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start_time);
			read_mem_only_gpu(input, limit, &sum, lparm);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&end_time);
			nanosecs = get_nanosecs(start_time,end_time);
			mb_per_sec = memsize_mb * 1.0 / nanosecs * NSECPERSEC;
			printf("the sum is %d\n", sum);
			printf("gpu read memory speed is %lfMB per second\n", mb_per_sec);
			printf("all done\n");
		}
	}else if(!strcmp(argv[2], "w")) {
		if(!strcmp(argv[1], "cpu")) {
			printf("you choose the write only test on cpu\n");
			printf("now the cpu is writting the memory\n");
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start_time);
			write_mem_only_cpu(input, limit);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&end_time);
			nanosecs = get_nanosecs(start_time,end_time);
			mb_per_sec = memsize_mb * 1.0 / nanosecs * NSECPERSEC;
			printf("cpu write memory speed is %lfMB per second\n", mb_per_sec);
			printf("all done\n");

		}else if(!strcmp(argv[1], "gpu")){
			printf("you choose the write only test on gpu\n");
			printf("now the gpu is writting the memory\n");
			SNK_INIT_LPARM(lparm,1);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start_time);
			write_mem_only_gpu(input, limit, lparm);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&end_time);
			nanosecs = get_nanosecs(start_time,end_time);
			mb_per_sec = memsize_mb * 1.0 / nanosecs * NSECPERSEC;
			printf("gpu write memory speed is %lfMB per second\n", mb_per_sec);
			printf("all done\n");
		}
	}
	free(input);
	return 0;
}