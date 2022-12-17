## MLPERF

***this guide is under development***

### What is MLperf ?

MLPerf™ is a consortium of AI leaders from academia, research labs, and industry whose mission is to “build fair and useful benchmarks” that provide 
unbiased evaluations of training and inference performance for hardware, software, 
and services—all conducted under prescribed conditions. To stay on the cutting edge of industry trends, MLPerf continues to evolve, 
holding new tests at regular intervals and adding new workloads that represent the state of the art in AI.



### MLPerf Submission Categories

MLPerf Training v2.1 is the seventh instantiation for training and consists of eight different workloads covering a broad diversity of use cases,
including vision, language, recommenders, and reinforcement learning. 

MLPerf Inference v2.1 is the sixth instantiation for inference and tested seven different use cases across 
seven different kinds of neural networks. Three of these use cases were for computer vision, one for recommender systems, two for language processing, and one for medical imaging.

MLPerf HPC v2.0 is the third iteration for HPC and tested three different scientific computing use cases,
including climate atmospheric river identification, cosmology parameter prediction, and quantum molecular modeling.


You will work only on one section of the MLperf benchmarks: the image recnognition benchmark using resnet50 network.
The g

You will find the details at this link:

https://github.com/mlcommons/inference/tree/master/vision/classification_and_detection

This is the DELL optimized implementation of the benchmark.

https://github.com/mlcommons/inference_results_v2.1/blob/master/closed/Dell/code/resnet50/tensorrt/README.md


Steps to do:
 - install the standard/reference benchmark and familiarize with it 
 - perform some benchmarkl measures 
 - install the DELL optimized version, run it and report difference

