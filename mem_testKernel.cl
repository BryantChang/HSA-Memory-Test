__kernel void read_mem_only_gpu(__global int *input, int limit, __global int *sum)
{
	int id = get_global_id(0);
	if(id < limit) {
		atomic_add(sum, input[id]);
	}
}


__kernel void write_mem_only_gpu(__global int *output, int limit)
{
	int id = get_global_id(0);
	if(id == 0) {
		for(id = 0;id < limit;id++) {
			output[id] = id;
		}
	}
}