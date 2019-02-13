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
