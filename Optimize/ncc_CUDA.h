#ifdef CUDA
#include "homography.h"


__global__ void calculate_correlation_CUDA(int signal_size, const Color * first_signal, const Color * second_signal, long * signal_correlationR_thread, long * signal_correlationG_thread, long * signal_correlationB_thread, long * first_signalR_ncc, long * first_signalG_ncc, long * first_signalB_ncc, long * second_signalR_ncc, long * second_signalG_ncc, long * second_signalB_ncc)
{
  unsigned int tid = threadIdx.x + blockIdx.x * blockDim.x;

  signal_correlationR_thread[tid] = first_signal[tid].r*second_signal[tid].r;
  signal_correlationG_thread[tid] = first_signal[tid].g*second_signal[tid].g;
  signal_correlationB_thread[tid] = first_signal[tid].b*second_signal[tid].b;

  first_signalR_ncc[tid] = first_signal[tid].r * first_signal[tid].r;
  first_signalG_ncc[tid] = first_signal[tid].g * first_signal[tid].g;
  first_signalB_ncc[tid] = first_signal[tid].b * first_signal[tid].b;

  second_signalR_ncc[tid] = second_signal[tid].r * second_signal[tid].r;   
  second_signalG_ncc[tid] = second_signal[tid].g * second_signal[tid].g;    
  second_signalB_ncc[tid] = second_signal[tid].b * second_signal[tid].b;
}

double calculate_normalized_correlation_CUDA(int signal_size, const Color * first_signal, const Color * second_signal)
{  
  //CPU Variables
  long signal_correlationR_thread[signal_size];
  long signal_correlationG_thread[signal_size];
  long signal_correlationB_thread[signal_size];

  long first_signalR_ncc[signal_size];
  long first_signalG_ncc[signal_size];
  long first_signalB_ncc[signal_size];
  long second_signalR_ncc[signal_size];
  long second_signalG_ncc[signal_size];
  long second_signalB_ncc[signal_size];

  long first_signal_sum[3];
  long second_signal_sum[3];

  //CUDA Variables
  long * dev_signal_correlationR_thread;
  long * dev_signal_correlationG_thread;
  long * dev_signal_correlationB_thread;

  long * dev_first_signalR_ncc;
  long * dev_first_signalG_ncc;
  long * dev_first_signalB_ncc;
  long * dev_second_signalR_ncc;
  long * dev_second_signalG_ncc;
  long * dev_second_signalB_ncc;

  Color * dev_first_signal;
  Color * dev_second_signal;

  //CUDA Allocation
  cudaMalloc(&dev_signal_correlationR_thread, signal_size*sizeof(long));
  cudaMalloc(&dev_signal_correlationG_thread, signal_size*sizeof(long));
  cudaMalloc(&dev_signal_correlationB_thread, signal_size*sizeof(long));

  cudaMalloc(&dev_first_signalR_ncc, signal_size*sizeof(long));
  cudaMalloc(&dev_first_signalG_ncc, signal_size*sizeof(long));
  cudaMalloc(&dev_first_signalB_ncc, signal_size*sizeof(long));
  cudaMalloc(&dev_second_signalR_ncc, signal_size*sizeof(long));
  cudaMalloc(&dev_second_signalG_ncc, signal_size*sizeof(long));
  cudaMalloc(&dev_second_signalB_ncc, signal_size*sizeof(long));

  cudaMalloc(&dev_first_signal, signal_size*sizeof(Color));
  cudaMalloc(&dev_second_signal, signal_size*sizeof(Color));

  //CUDA Copying
  cudaMemcpy(dev_first_signal, first_signal, signal_size*sizeof(Color), cudaMemcpyHostToDevice);
  cudaMemcpy(dev_second_signal, second_signal, signal_size*sizeof(Color), cudaMemcpyHostToDevice);

  //Calculate block and thread size
  double block_num = round(sqrt(signal_size));
  double thread_num = signal_size/block_num;

  calculate_correlation_CUDA<<<block_num,thread_num>>>(signal_size, dev_first_signal, dev_second_signal, dev_signal_correlationR_thread, dev_signal_correlationG_thread, dev_signal_correlationB_thread, dev_first_signalR_ncc, dev_first_signalG_ncc, dev_first_signalB_ncc, dev_second_signalR_ncc, dev_second_signalG_ncc, dev_second_signalB_ncc);

  cudaMemcpy(signal_correlationR_thread, dev_signal_correlationR_thread, signal_size*sizeof(long), cudaMemcpyDeviceToHost);
  cudaMemcpy(signal_correlationG_thread, dev_signal_correlationG_thread, signal_size*sizeof(long), cudaMemcpyDeviceToHost);
  cudaMemcpy(signal_correlationB_thread, dev_signal_correlationB_thread, signal_size*sizeof(long), cudaMemcpyDeviceToHost);

  cudaMemcpy(first_signalR_ncc, dev_first_signalR_ncc, signal_size*sizeof(long), cudaMemcpyDeviceToHost);
  cudaMemcpy(first_signalG_ncc, dev_first_signalG_ncc, signal_size*sizeof(long), cudaMemcpyDeviceToHost);
  cudaMemcpy(first_signalB_ncc, dev_first_signalB_ncc, signal_size*sizeof(long), cudaMemcpyDeviceToHost);
  cudaMemcpy(second_signalR_ncc, dev_second_signalR_ncc, signal_size*sizeof(long), cudaMemcpyDeviceToHost);
  cudaMemcpy(second_signalG_ncc, dev_second_signalG_ncc, signal_size*sizeof(long), cudaMemcpyDeviceToHost);
  cudaMemcpy(second_signalB_ncc, dev_second_signalB_ncc, signal_size*sizeof(long), cudaMemcpyDeviceToHost);

  long signal_correlationR = 0;
  long signal_correlationG = 0;
  long signal_correlationB = 0;
  long correlation = 0;

  for (int i = 0; i < signal_size; i++){
    signal_correlationR += signal_correlationR_thread[i];
    signal_correlationG += signal_correlationG_thread[i];
    signal_correlationB += signal_correlationB_thread[i];

    first_signal_sum[0] += first_signalR_ncc[i];
    first_signal_sum[1] += first_signalG_ncc[i];
    first_signal_sum[2] += first_signalB_ncc[i];

    second_signal_sum[0] += second_signalR_ncc[i];
    second_signal_sum[1] += second_signalG_ncc[i];
    second_signal_sum[2] += second_signalB_ncc[i];
  }
  correlation = (signal_correlationR+signal_correlationG+signal_correlationB)/3;


  long correlation_scalar = 0;
  long total_sum1 = (first_signal_sum[0] + first_signal_sum[1] + first_signal_sum[2]) / 3; 
  long total_sum2 = (second_signal_sum[0] + second_signal_sum[1] + second_signal_sum[2]) / 3; 
  correlation_scalar = sqrt(total_sum1*total_sum2); 
  
  for (int i = 0 ; i < 3; i++){ //If this is not included the values remain for an unknown reason
    first_signal_sum[0] = first_signal_sum[1] = first_signal_sum[2] = 0;
    second_signal_sum[0] = second_signal_sum[1] = second_signal_sum[2] = 0;
   }
  if(correlation_scalar == 0){
    return -1;
  }else{
    return (double)correlation/correlation_scalar;
  }
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

double calcNCCInv(vector<PixelLoc> *interior, double * current, Image *myimg, Image *myimgOther)
{
    int signal_size = interior->size();
    Color black(0,0,0);
    Color white(255,255,255);
    Color signal1[signal_size], signal2[signal_size];
    double point[2];
    double inverse[9];
    inverse_homography(inverse, current);
    for(unsigned int i=0; i<signal_size; ++i){
        homography(interior[0][i].x + 0.5 , interior[0][i].y + 0.5, inverse, point);
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