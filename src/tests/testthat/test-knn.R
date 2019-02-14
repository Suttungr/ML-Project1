library(nearestNeighbors)
library(testthat)
context("knn")

test_that("knn computes same answer as R", {
  data(zip.train, package="ElemStatLearn")
  i01 <- which(zip.train[,1] %in% c(0,1))
  train.i <- i01[1:100]
  test.i <- i01[6]
  X <- zip.train[train.i, -1]
  y <- zip.train[train.i, 1]
  testX <- zip.train[test.i, -1]
  max.neighbors <- 4
  pred.vec <- knn(X, y, testX, 3)
  dist.vec <- t(X) - testX
  dist.vec <- sqrt(colSums(dist.mat * dist.vec))
  sorted.index.vec <- order(dist.vec)
  closest.indices <- sorted.index.vec[1:max.neighbors]
  expected.prediction <- cumsum(y[closest.indices]) / (1:max.neighbors)
  expected_equal(pred.vec, expected.prediction)
})