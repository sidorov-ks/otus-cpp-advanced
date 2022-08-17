#pragma once

#include <Eigen/Dense>

#include <vector>
#include <string>

#include "sample.h"

struct Model {
  static const std::size_t HIDDEN_DIM = 128;
  Eigen::MatrixXd layer_1;
  Eigen::MatrixXd layer_2;

  std::vector<Label> operator()(const std::vector<FeatureVector> &sample) const;
};

std::vector<SampleRecord> load_sample(const std::string &sample_path);

Model load_model(const std::string &model_path);

double accuracy(std::vector<Label> &pred_labels, std::vector<Label> &true_labels);