#' K Nearest Neighbors Algo
#' 
#' An R function that wraps C++ code
#'
#' @param X.mat numeric input feature matrix [n x p]
#' @param y.vec numeric train label vector [n], either all 0/1 for binary class., or other real numbers for regression (multi-class classification not supported)
#' @param testX.vec numeric test feature vector [p]
#' @param max.neighbors scalar integer, max number of neighbors
#'
#' @return numeric vector of size max.neighbors, predictions from 1 to max
#' @export
#'
#' @examples
knn <- function(X.mat, y.vec, testX.vec, max.neighbors){
  result.list <- .C("knn_interface", as.double(X.mat), as.double(y.vec), as.double(testX.vec), as.integer(nrow(X.mat)), 
                    as.integer(ncol(X.mat)), as.integer(max.neighbors), predictions=double(max.neighbors), 
                    PACKAGE=nearestNeighbors)
  result.list$predictions
}

#' Learning algorithm using cross-validation to select the number of neighbors.
#'
#' @param X.mat A training data set
#' @param y.vec A training data set
#' @param max.neighbors scalar integer, max number of neighbors
#' @param fold.vec vector of fold ID numbers
#' @param n.folds number of folds to break
#'
#' @return a list of training data from X.mat and y.vec, 
#' validation.loss.mat (matrices of loss values for each fold and number of neighbors),
#' validation.loss.vec (vectors with max.neighbors elements: mean loss over all folds),
#' selected.neighbors (number of neighbors selected by minimizing the mean validation loss),
#' predict(testX.mat), a function that takes a matrix of inputs/features and returns a vector of predictions.
#' @export
#'
#' @examples
NNLearnCV <- function(X.mat, y.vec, max.neighbors=30, fold.vec=NULL, n.folds=5) {
  # If fold.vec is NULL, randomly assign fold IDs from 1 to n.folds 
  
  if(fold.vec == is.null) {
    fold.vec <- sample(rep(1:n.folds, l=nrow(X.mat)))
  }
    
  # make sure that fold.vec is the same size as y.vec, which is the 
  # same as the number of rows in X.mat). Use stop() with an informative error message
  
  if(nrow(X.mat) != length(y.vec)) {
    stop("ERROR: The number of rows in X.mat is not the same as the size of y.vec")
  }
  
  if(length(fold.vec) != length(y.vec)) {
    stop("ERROR: the size of fold.vec IS NOT equivalent to the size of y.vec")
  }
  
  # then perform cross-validation to compute two matrices of mean loss values, 
  # (max.neighbors rows x n.folds columns), one for the train sets (train.loss.mat), 
  # one for the validation sets (validation.loss.mat).
  
  # TODO
  
  # If the labels (y.vec) are all in {0,1} then the loss function should 
  # be the 01-loss (binary classification), otherwise use the square loss (regression).
  
  # TODO
  
  for(fold.i in seq_along(unique.folds)){
    for(prediction.set.name in c("train", "validation")){
      pred.mat <- NN1toKmaxPredict(
        train.features, train.labels,
        prediction.set.features, max.neighbors)
      loss.mat <- if(labels.all.01){
        ifelse(pred.mat>0.5, 1, 0) != y.vec #zero-one loss for binary classification.
      }else{
        (pred.mat-y.vec)^2 #square loss for regression.
      }
      train.or.validation.loss.mat[, fold.i] <- colMeans(loss.mat)
    }
  }
  
  # return the list from the documentation above
}
