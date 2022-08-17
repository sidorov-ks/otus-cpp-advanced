#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "sample.h"
#include "model.h"

int main(int argc, char **argv) {
  try {
    if (argc != 3) {
      std::cerr << "Usage: fashio_mnist <test sample> <dir with w1.txt && w2.txt>\n";
      return 1;
    }
    std::string sample_file{argv[1]}, model_dir{argv[2]};
    Model model = load_model(model_dir);
    std::vector<SampleRecord> test_sample = load_sample(sample_file);
    std::vector<Label> test_labels;
    test_labels.reserve(test_sample.size());
    std::transform(test_sample.begin(), test_sample.end(), std::back_inserter(test_labels),
                   [](SampleRecord &rec) { return rec.label; });
    std::vector<FeatureVector> feature_matrix;
    feature_matrix.reserve(test_sample.size());
    std::transform(test_sample.begin(), test_sample.end(), std::back_inserter(feature_matrix),
                   [](SampleRecord &rec) { return rec.features; });
    std::vector<Label> model_labels = model(feature_matrix);
    double model_acc = accuracy(model_labels, test_labels);
    std::cout << model_acc << std::endl;
    return 0;
  }
  catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  return 0;
}
