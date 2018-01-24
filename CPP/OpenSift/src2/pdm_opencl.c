#include <OpenCL/cl.h>
__kernel void test(__global float*v){
    int i = get_global_id(0);
    v[i] *= 2;
}
