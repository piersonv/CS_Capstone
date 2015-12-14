#ifdef CUDA
#include "homography.h"

__device__ double dev_correlation[1];
__device__ double dev_first_signal_sum[3];
__device__ double dev_second_signal_sum[3];

__global__ void calculate_correlation_CUDA(int signal_size, const Color * first_signal, const Color * second_signal, double * signal_correlationR_thread, double * signal_correlationG_thread, double * signal_correlationB_thread, double * first_signalR_ncc, double * first_signalG_ncc, double * first_signalB_ncc, double * second_signalR_ncc, double * second_signalG_ncc, double * second_signalB_ncc)
{
  unsigned int tid = threadIdx.x + blockIdx.x * blockDim.x;

  double signal_correlationR = 0;
  double signal_correlationG = 0;
  double signal_correlationB = 0;

  signal_correlationR_thread[tid] = first_signal[tid].r*second_signal[tid].r;
  signal_correlationG_thread[tid] = first_signal[tid].g*second_signal[tid].g;
  signal_correlationB_thread[tid] = first_signal[tid].b*second_signal[tid].b;

  first_signalR_ncc[tid] = first_signal[tid].r * first_signal[tid].r;
  first_signalG_ncc[tid] = first_signal[tid].g * first_signal[tid].g;
  first_signalB_ncc[tid] = first_signal[tid].b * first_signal[tid].b;

  second_signalR_ncc[tid] = second_signal[tid].r * second_signal[tid].r;   
  second_signalG_ncc[tid] = second_signal[tid].g * second_signal[tid].g;    
  second_signalB_ncc[tid] = second_signal[tid].b * second_signal[tid].b;

  if(tid == 0){
    for (int i = 0; i < signal_size; i++){
      signal_correlationR += signal_correlationR_thread[i];
      signal_correlationG += signal_correlationG_thread[i];
      signal_correlationB += signal_correlationB_thread[i];

      dev_first_signal_sum[0] += first_signalR_ncc[i];
      dev_first_signal_sum[1] += first_signalG_ncc[i];
      dev_first_signal_sum[2] += first_signalB_ncc[i];

      dev_second_signal_sum[0] += second_signalR_ncc[i];
      dev_second_signal_sum[1] += second_signalG_ncc[i];
      dev_second_signal_sum[2] += second_signalB_ncc[i];
    }
    dev_correlation[0] = (signal_correlationR+signal_correlationG+signal_correlationB)/3;
  }
}

double calculate_normalized_correlation_CUDA(int signal_size, const Color * first_signal, const Color * second_signal)
{  
  //CUDA Variables
  double * signal_correlationR_thread;
  double * signal_correlationG_thread;
  double * signal_correlationB_thread;

  double * first_signalR_ncc;
  double * first_signalG_ncc;
  double * first_signalB_ncc;
  double * second_signalR_ncc;
  double * second_signalG_ncc;
  double * second_signalB_ncc;

  Color * dev_first_signal;
  Color * dev_second_signal;

  //CUDA Allocation
  cudaMalloc(&signal_correlationR_thread, signal_size*sizeof(double));
  cudaMalloc(&signal_correlationG_thread, signal_size*sizeof(double));
  cudaMalloc(&signal_correlationB_thread, signal_size*sizeof(double));

  cudaMalloc(&first_signalR_ncc, signal_size*sizeof(double));
  cudaMalloc(&first_signalG_ncc, signal_size*sizeof(double));
  cudaMalloc(&first_signalB_ncc, signal_size*sizeof(double));
  cudaMalloc(&second_signalR_ncc, signal_size*sizeof(double));
  cudaMalloc(&second_signalG_ncc, signal_size*sizeof(double));
  cudaMalloc(&second_signalB_ncc, signal_size*sizeof(double));

  cudaMalloc(&dev_first_signal, signal_size*sizeof(Color));
  cudaMalloc(&signal_correlationB_thread, signal_size*sizeof(Color));

  //CUDA Copying
  cudaMemcpy(&dev_first_signal, first_signal, sizeof(signal_size), cudaMemcpyHostToDevice);
  cudaMemcpy(&dev_second_signal, second_signal, sizeof(signal_size), cudaMemcpyHostToDevice);

  //Calculate block and thread size
  double block_num = round(sqrt(signal_size));
  double thread_num = signal_size/block_num;

  double correlation[1]; 
  int first_signal_sum[3];
  int second_signal_sum[3];
  calculate_correlation_CUDA<<<block_num,thread_num>>>(signal_size, dev_first_signal, dev_second_signal, signal_correlationR_thread, signal_correlationG_thread, signal_correlationB_thread, first_signalR_ncc, first_signalG_ncc, first_signalB_ncc, second_signalR_ncc, second_signalG_ncc, second_signalB_ncc);
  cudaMemcpy(&correlation, dev_correlation, sizeof(double), cudaMemcpyDeviceToHost);
  cudaMemcpy(&first_signal_sum, dev_first_signal_sum, 3*sizeof(double), cudaMemcpyDeviceToHost);
  cudaMemcpy(&second_signal_sum, dev_second_signal_sum, 3*sizeof(double), cudaMemcpyDeviceToHost);

  double correlation_scalar = 0;
  double total_sum1 = (first_signal_sum[0] + first_signal_sum[1] + first_signal_sum[2]) / 3; 
  double total_sum2 = (second_signal_sum[0] + second_signal_sum[1] + second_signal_sum[2]) / 3; 
  correlation_scalar = sqrt(total_sum1*total_sum2);

  return (double)correlation[0]/correlation_scalar;

}

double calcNCC(vector<PixelLoc> *interior, double * current, Image *myimg, Image *myimgOther)
{
  int signal_size = interior->size();
  Color black(0,0,0);
  Color white(255,255,255);
  Color signal1[signal_size], signal2[signal_size];
  double point[2];
  for(unsigned int i=0; i<signal_size; ++i){
    homography(interior[0][i].x + 0.5 , interior[0][i].y + 0.5, current, point);
                Coord mycoord(point[0], point[1]);
                if(inImage(myimg,mycoord)){
                    signal1[i] = asInterpolatedColor(mycoord, myimg);
                    signal2[i] = myimgOther->getPixel(interior[0][i]);
                } else {
                    signal1[i] = black;
                    signal2[i] = white;
                }
         }
         return calculate_normalized_correlation_CUDA(signal_size, signal1, signal2);
}

#endif