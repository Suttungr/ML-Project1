#include <R.h>
#include <R_ext/Rdynload.h>
#include "NN1toKmaxPredict.h"


// interface functions always return void
void NN1toKmaxPredict_interface(
    const double *train_inputs_ptr, //n_observations x n_features
    const double *test_inputs_ptr, // n_test_observations x n_features(Matrix)
    const double *train_labels_ptr,  //n_observations
    const int *n_train_observations_ptr,
    const int *n_test_observations_ptr,
    const int *n_features_ptr,
    const int *max_neighbors_ptr,
    double *test_predictions_ptr //n_test_observations x max_neighbors
){
  int status = NN1toKmaxPredict(
    train_inputs_ptr,
    test_inputs_ptr,
    train_labels_ptr,
    *n_train_observations_ptr,
    *n_test_observations_ptr,
    *n_features_ptr,
    *max_neighbors_ptr,
    test_predictions_ptr
  );
  
  if (status != 0){
    error("non-zero exit status from NN1toKmaxPredict");
  }
}

R_CMethodDef cMethods[] = {
  {"NN1toKmaxPredict_interface", (DL_FUNC) &NN1toKmaxPredict_interface, 7},
  {NULL, NULL, 0}
};

extern "C" {
  void R_init_nearestNeighbors(DllInfo *info){
    R_registerRoutines(info, cMethods, NULL, NULL, NULL);
    R_useDynamicSymbols(info, FALSE);
  }
}
