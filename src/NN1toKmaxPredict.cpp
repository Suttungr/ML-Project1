#include <Eigen/Dense>

int NN1toKmaxPredict(
    const int n_train_observations, const int n_test_observations, const int n_features, const int max_neighbors, 
    const double *train_inputs_ptr, //n_train_observations x n_features (Matrix)
    // const double *test_inputs_ptr, //n_test_observations x n_features(Matrix)
    const double *test_inputs_ptr, //n_test_features
    const double *train_labels_ptr,  //n_observations (vector)
    double *test_predictions_ptr //n_test_observations x max_neighbors
) {
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
  
  for(int i = 0; i < n_train_observations; i++){
    diff_vec = train_inputs_mat.row(i).transpose() - test_inputs_vec;
    dist_vec(i) = diff_vec.norm();
    sorted_index_vec(i)=i;
  }
  
  std::sort(
    sorted_index_vec.data(),
    sorted_index_vec.data()+sorted_index_vec.size(),
    [&dist_vec](int left, int right){
      return dist_vec(left) < dist_vec(right);
    });
  

  //std::cout << sorted_index_vec << std::endl<<std::end1;

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