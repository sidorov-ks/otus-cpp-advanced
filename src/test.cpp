#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <random>
#include <string>
#include <vector>
#include <regex>

#include "search_config.h"
#include "bayan.h"

namespace fs = boost::filesystem;

// Создаёт следующие файлы (одна строка = одинаковое содержимое):
// ./A.txt ./B.txt ./C.txt ./src/a.py ./src/b.py
// ./.gitignore
// ./docs/README.md
// Все файлы (даже различающиеся) совпадают в первых 2048 символах
class FilesystemTest : public ::testing::Test {
protected:
  FilesystemTest() : _root_dir(fs::temp_directory_path() / fs::unique_path()),
                     _src_dir(_root_dir / "src"),
                     _docs_dir(_root_dir / "docs"),
                     _a_txt_path(_root_dir / "A.txt"),
                     _b_txt_path(_root_dir / "B.txt"),
                     _c_txt_path(_root_dir / "C.txt"),
                     _gitignore_path(_root_dir / ".gitignore"),
                     _a_py_path(_src_dir / "a.py"),
                     _b_py_path(_src_dir / "b.py"),
                     _readme_md_path(_docs_dir / "README.md") {}

  void SetUp() override {
    std::cerr << _root_dir << "\n";
    MakeTree();
    std::string prefix = RandomText(PREFIX_LENGTH);
    std::string text_1 = prefix + RandomText(SUFFIX_LENGTH);
    std::string text_2 = prefix + RandomText(SUFFIX_LENGTH);
    std::string text_3 = prefix + RandomText(SUFFIX_LENGTH);
    _a_txt << text_1;
    _b_txt << text_1;
    _a_py << text_1;
    _b_py << text_1;
    _c_txt << text_2;
    _gitignore << text_3;
    _readme_md << text_3;
  }

  void TearDown() override {
    fs::remove_all(_root_dir);
  }

  SearchConfiguration DefaultConfig() {
    return {
            {{_root_dir}}, {},
            0, 0,
            {{std::regex{".*"}}},
            128, hashing::HASH_MD5
    };
  }

  fs::path _root_dir, _src_dir, _docs_dir;
  fs::path _a_txt_path, _b_txt_path, _c_txt_path, _gitignore_path, _a_py_path, _b_py_path, _readme_md_path;
  fs::ofstream _a_txt, _b_txt, _c_txt, _gitignore, _a_py, _b_py, _readme_md;

private:
  std::string RandomText(std::size_t length) {
    std::string text(length, '\0');
    for (auto &dis: text)
      dis = distribution(generator);
    return text;
  }

  void MakeTree() {
    fs::create_directories(_root_dir);
    fs::create_directories(_src_dir);
    fs::create_directories(_docs_dir);
    _a_txt.open(_a_txt_path);
    _b_txt.open(_b_txt_path);
    _c_txt.open(_c_txt_path);
    _gitignore.open(_gitignore_path);
    _a_py.open(_a_py_path);
    _b_py.open(_b_py_path);
    _readme_md.open(_readme_md_path);
  }

  const std::size_t PREFIX_LENGTH = 2048;
  const std::size_t SUFFIX_LENGTH = 2048;
  std::mt19937 generator{std::random_device{}()};
  std::uniform_int_distribution<int> distribution{'a', 'z'};
};

TEST_F(FilesystemTest, test_depth) {
  auto config = DefaultConfig();
  config.depth = 0;
  auto depth_0_files = collect_files(config);
  EXPECT_THAT(depth_0_files, testing::UnorderedElementsAre(_a_txt_path, _b_txt_path, _c_txt_path, _gitignore_path));
  config.depth = 1;
  auto depth_1_files = collect_files(config);
  EXPECT_THAT(depth_1_files,
              testing::UnorderedElementsAre(_a_txt_path, _b_txt_path, _c_txt_path, _gitignore_path, _a_py_path,
                                            _b_py_path, _readme_md_path));
}