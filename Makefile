all:mem_test
mem_test:mem_test.c mem_testKernel.o
	gcc -I. mem_test.c mem_testKernel.o  -L/opt/hsa/lib -lhsa-runtime64  -o $@
mem_testKernel.o:
	@snack.sh -c -opt 3 -vv mem_testKernel.cl
mem_testKernel:
	@snack.sh -c -opt 3 -vv mem_testKernel.cl
clean:
	rm -rf *.o
	rm -rf mem_testKernel.h