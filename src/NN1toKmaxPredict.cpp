#include <Eigen/Dense>
#include <iostream>
#include "NN1toKmaxPredict.h"
#include "ErrorCodes.h"

int NN1toKmaxPredict(
    const double *train_inputs_ptr, // n_train_observations x n_features (Matrix)
    const double *test_inputs_ptr, // n_test_observations x n_features(Matrix)
    const double *train_labels_ptr, //n_observations (vector)
    const int n_train_observations, const int n_test_observations, const int n_features, const int max_neighbors, 
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
  
  //Eigen::Map<Eigen::VectorXd> test_inputs_vec(
   // (double*) test_inputs_ptr, n_features);
  
  Eigen::Map< Eigen::MatrixXd > test_inputs_mat(
      (double*) test_inputs_ptr, n_test_observations, n_features);
  
  Eigen::Map<Eigen::VectorXd> train_labels_vec(
    (double*) train_labels_ptr, n_train_observations);
  
  Eigen::MatrixXd dist_mat(n_train_observations, n_test_observations);
  
  Eigen::VectorXd diff_vec(n_features);
  
  Eigen::VectorXi sorted_index_vec(n_train_observations * n_test_observations);
  
  // Finding the distance
  // Find a 2D matrix for each test set
  for(int j = 0; j < n_test_observations; j++){
    
    // For each test vector, the distance is found between each training vector and that test vector
    for(int i = 0; i < n_train_observations; i++){
      
      // Distance for L1 Manhattan
      diff_vec = test_inputs_mat.row(j) - train_inputs_mat.row(i);
      dist_mat.row(i)[j] = diff_vec.sum();
      
      // Finds the appropriate horizontal index to use a 1D vector in 2D
      int row_index = i * n_train_observations;
      
      sorted_index_vec(row_index + j) = row_index + j;
    }
  }
  
  
  // Sorting while keeping track of indexes
  // This might break with distances being a matrix
  std::sort(
    sorted_index_vec.data(),
    sorted_index_vec.data()+sorted_index_vec.size(),
    [&dist_mat](int left, int right){
      return dist_mat(left) < dist_mat(right);
    });
  
  
  // For classification return the mode of the labels, for regression return the mean
  // For each set of test data, add a row to the matrix.  For each of those rows, for each k, compute the mean||mode
    for(int i = 0; i < n_test_observations; i++){
      
    // Finds the appropriate horizontal index to use a 1D vector in 2D
    int row_index = i * n_test_observations;
    
    double total_labels = 0.0; // Reset the total for each row (each testing set)
    
    // For each set of test data
    for(int k = 0; k < max_neighbors; k++){
      
      int neighbors = k + 1;
      total_labels += dist_mat.row(i)[k];
      test_predictions_ptr[row_index + k] = total_labels/neighbors;
    }
  }

  return 0;
}