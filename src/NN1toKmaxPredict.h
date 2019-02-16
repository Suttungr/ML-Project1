

int NN1toKmaxPredict(
    const double *train_inputs_ptr, //n_train_observations x n_features (Matrix)
    const double *test_inputs_ptr, //n_test_features
    const double *train_labels_ptr,  //n_observations (vector)
    const int n_train_observations, const int n_test_observations, const int n_features, const int max_neighbors, 
    double *test_predictions_ptr //n_test_observations x max_neighbors
);