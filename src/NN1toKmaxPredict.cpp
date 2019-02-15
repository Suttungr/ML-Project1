#include <Eigen/Dense>
#include<iostream>
#include "ErrorCodes.h"

int NN1toKmaxPredict(
    const int n_train_observations, const int n_test_observations, const int n_features, const int max_neighbors, 
    const double *train_inputs_ptr, //n_train_observations x n_features (Matrix)
    // const double *test_inputs_ptr, //n_test_observations x n_features(Matrix)
    const double *test_inputs_ptr, //n_test_features
    const double *train_labels_ptr,  //n_observations (vector)
    double *test_predictions_ptr //n_test_observations x max_neighbors
) {
  
  if(max_neighbors <= 0){
    
    std::cout << K_NOT_POSITIVE_ERROR << std::endl;
    return EXIT_FAILURE;
  }
  
  if(n_train_observations <= 0 ||
     n_test_observations <= 0 ||
     n_features <= 0){
    
    std:: cout << DIMENSIONS_NOT_POSITIVE_ERROR << std::endl;
    return EXIT_FAILURE;
  }
  
  Eigen::Map< Eigen::MatrixXd > train_inputs_mat(
      (double*)train_inputs_ptr, n_train_observations, n_features);
  
  Eigen::Map<Eigen::VectorXd> test_inputs_vec(
    (double*) test_inputs_ptr, n_features);
  
  //Eigen::Map< Eigen::MatrixXd > test_inputs_mat(
    //  (double*) test_inputs_ptr, n_test_observations, n_features);
  
  Eigen::Map<Eigen::VectorXd> train_labels_vec(
    (double*) train_labels_ptr, n_train_observations);
  
  Eigen::VectorXd dist_vec(n_train_observations);
  
  Eigen::VectorXd diff_vec(n_features);
  
  Eigen::VectorXi sorted_index_vec(n_train_observations);
  
  // Finding the distance
  for(int i = 0; i < n_train_observations; i++){
    
    // Distance for L1 Manhattan
    diff_vec = test_inputs_vec - train_inputs_mat.row(i).transpose();
    dist_vec(i) = diff_vec.sum();
    sorted_index_vec(i) = i;
    
    /*// Distance for L2 Norm
     * diff_vec = train_inputs_mat.row(i).transpose() - test_inputs_vec;
     * dist_vec(i) = diff_vec.norm();
     * sorted_index_vec(i)=i;
     */
  }
  
  // Sorting while keeping track of indexes
  std::sort(
    sorted_index_vec.data(),
    sorted_index_vec.data()+sorted_index_vec.size(),
    [&dist_vec](int left, int right){
      return dist_vec(left) < dist_vec(right);
    });
  

  //std::cout << sorted_index_vec << std::endl<<std::end1;

  // Grabbing the first nearest neighbors and find their mean (regression)
  // For classification return the mode of the labels
  double total_labels = 0.0;
  for(int k = 0; k < max_neighbors; k++)
  {
    int row = sorted_index_vec(k);
    int neighbors = k + 1;
    total_labels += train_labels_ptr[k];
    test_predictions_ptr[k] = total_labels/neighbors;
  }

  return 0;
}