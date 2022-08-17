#pragma once

#include <array>

const std::size_t WIDTH = 28, HEIGHT = 28;
const std::size_t N_DIM = WIDTH * HEIGHT;
const std::size_t N_CLASSES = 10;
const short MAX_PIXEL_VALUE = 255;

using Label = short;
using FeatureVector = std::array<double, N_DIM>;

struct SampleRecord {
  Label label;
  FeatureVector features;
};