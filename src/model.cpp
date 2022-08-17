#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>

#include <iostream>

#include "model.h"

std::vector<SampleRecord> load_sample(const std::string &sample_path) {
  std::ifstream fin(sample_path);
  std::string line;
  std::vector<SampleRecord> records;
  while (std::getline(fin, line)) {
    SampleRecord record;
    std::istringstream iss(line);
    std::string label_str;
    std::getline(iss, label_str, ',');
    record.label = std::stoi(label_str);
    std::string pixel_str;
    std::size_t ix = 0;
    while (std::getline(iss, pixel_str, ',')) {
      assert(ix < N_DIM);
      assert(std::stoi(pixel_str) >= 0 && std::stoi(pixel_str) <= MAX_PIXEL_VALUE);
      record.features[ix] = 1.0 * std::stoi(pixel_str) / MAX_PIXEL_VALUE;
      ix++;
    }
    assert(ix == N_DIM);
    records.push_back(record);
  }
  return records;
}

template<Eigen::Index Rows, Eigen::Index Cols>
Eigen::MatrixXd eigen_from_file(const std::string &path) {
  Eigen::MatrixXd res(Rows, Cols);
  std::ifstream fin(path);
  for (Eigen::Index row_ix = 0; row_ix < Rows; row_ix++) {
    for (Eigen::Index col_ix = 0; col_ix < Cols; col_ix++) {
      fin >> res(row_ix, col_ix);
    }
  }
  return res;
}

Model load_model(const std::string &model_path) {
  Model model{
          eigen_from_file<N_DIM, Model::HIDDEN_DIM>(model_path + "/w1.txt"),
          eigen_from_file<Model::HIDDEN_DIM, N_CLASSES>(model_path + "/w2.txt")
  };
  return model;
}

std::vector<Label> Model::operator()(const std::vector<FeatureVector> &sample) const {
  Eigen::Matrix<double, Eigen::Dynamic, N_DIM> feature_matrix(sample.size(), N_DIM);
  for (std::size_t row_ix = 0; row_ix < sample.size(); ++row_ix) {
    for (std::size_t col_ix = 0; col_ix < N_DIM; ++col_ix) {
      feature_matrix(row_ix, col_ix) = sample[row_ix][col_ix];
    }
  }
  auto hidden_output = (feature_matrix * layer_1).unaryExpr([](const double z) { return 1.0 / (1.0 + exp(-z)); });
  auto model_output = (hidden_output * layer_2).eval();
  std::vector<Label> labels(sample.size());
  for (std::size_t row_ix = 0; row_ix < sample.size(); ++row_ix) {
    std::size_t max_index = -1;
    double max_value = -std::numeric_limits<double>::infinity();
    auto record_model_output = model_output.row(row_ix);
    for (std::size_t col_ix = 0; col_ix < N_CLASSES; ++col_ix) {
      if (record_model_output(col_ix) > max_value) {
        max_index = col_ix;
        max_value = record_model_output(col_ix);
      }
    }
    labels[row_ix] = max_index;
  }
  return labels;
}

double accuracy(std::vector<Label> &pred_labels, std::vector<Label> &true_labels) {
  assert(pred_labels.size() == true_labels.size());
  std::size_t n_correct = 0, n_total = pred_labels.size();
  for (std::size_t ix = 0; ix < n_total; ++ix) {
    if (pred_labels[ix] == true_labels[ix])
      n_correct++;
  }
  return 1.0 * n_correct / n_total;
}