#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <random>
#include <string>
#include <vector>
#include <regex>

#include "search_config.h"
#include "bayan.h"

TEST(GlobRegexTest, test_star) {
  auto regex = make_regex("*.txt");
  std::string matching_names[] = {".txt", "file.txt", "file.txt.txt", "src.cpp.txt"};
  std::string mismatching_names[] = {"txt", "filetxt", "file.txttxt", "file.txt.tmp", "file_txt"};
  for (const auto &filename: matching_names)
    EXPECT_TRUE(std::regex_match(filename, regex));
  for (const auto &filename: mismatching_names)
    EXPECT_FALSE(std::regex_match(filename, regex));
}

TEST(GlobRegexTest, test_question_mask) {
  auto regex = make_regex("application_???.log");
  std::string matching_names[] = {"application_000.log", "application_AAA.log", "application_tmp.log"};
  std::string mismatching_names[] = {"application000.log", "application.000.log", "application_00.log",
                                     "aplication_000.log", "my_application_000.log", "app_000.log",
                                     "application_000.log.txt"};
  for (const auto &filename: matching_names)
    EXPECT_TRUE(std::regex_match(filename, regex));
  for (const auto &filename: mismatching_names)
    EXPECT_FALSE(std::regex_match(filename, regex));
}

TEST(GlobRegexTest, test_dot) {
  auto regex = make_regex("plaintext.log");
  std::string matching_names[] = {"plaintext.log"};
  std::string mismatching_names[] = {"whatever", "plaintext_log"};
  for (const auto &filename: matching_names)
    EXPECT_TRUE(std::regex_match(filename, regex));
  for (const auto &filename: mismatching_names)
    EXPECT_FALSE(std::regex_match(filename, regex));
}

namespace fs = boost::filesystem;

// Создаёт следующие файлы (одна строка = одинаковое содержимое):
// ./A.txt ./B.txt ./src/a.py ./src/b.py
// ./.gitignore ./docs/README.md
// ./C.txt
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
            1, 0,
            {{make_regex("*")}},
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

TEST_F(FilesystemTest, test_exclude_dirs) {
  auto config = DefaultConfig();
  auto all_files = collect_files(config);
  EXPECT_THAT(all_files,
              testing::UnorderedElementsAre(_a_txt_path, _b_txt_path, _c_txt_path, _gitignore_path, _a_py_path,
                                            _b_py_path, _readme_md_path));
  config.exclude_dirs = {{_src_dir}};
  auto excl_files = collect_files(config);
  EXPECT_THAT(excl_files,
              testing::UnorderedElementsAre(_a_txt_path, _b_txt_path, _c_txt_path, _gitignore_path, _readme_md_path));
}

TEST_F(FilesystemTest, test_masks) {
  auto config = DefaultConfig();
  auto all_files = collect_files(config);
  EXPECT_THAT(all_files,
              testing::UnorderedElementsAre(_a_txt_path, _b_txt_path, _c_txt_path, _gitignore_path, _a_py_path,
                                            _b_py_path, _readme_md_path));
  config.filename_masks = {make_regex("*.txt"), make_regex("*.md")};
  auto txt_or_md_files = collect_files(config);
  EXPECT_THAT(txt_or_md_files,
              testing::UnorderedElementsAre(_a_txt_path, _b_txt_path, _c_txt_path, _readme_md_path));
}

TEST_F(FilesystemTest, test_duplicates) {
  auto config = DefaultConfig();
  std::vector<fs::path> files = {_a_txt_path, _b_txt_path, _c_txt_path, _gitignore_path, _a_py_path,
                                 _b_py_path, _readme_md_path};
  auto hash = [config](const std::string &text) { return hashing::hash(config.hash, text); };
  auto duplicates = find_duplicates(files, config.block_size, hash);
  EXPECT_THAT(
          duplicates,
          testing::UnorderedElementsAre(
                  testing::UnorderedElementsAre(_c_txt_path),
                  testing::UnorderedElementsAre(_gitignore_path, _readme_md_path),
                  testing::UnorderedElementsAre(_a_txt_path, _b_txt_path, _a_py_path, _b_py_path)
          )
  );
}