#pragma once

class DocumentView;

#include "document_view.h"
#include "primitive/primitive.h"

#include <string>
#include <vector>
#include <optional>
#include <memory>

class Document {
public:
  Document() {}

  Document(const std::string &filename) : _path(filename) {}

  std::optional<std::string> current_filename();

  bool save_as(const std::string &filename);

  bool save();

  void add_primitive(std::unique_ptr<Primitive> &&primitive);

  bool remove_primitive(const Primitive &primitive);

  std::optional<Primitive *> find_at(const Point &point) const;

  std::vector<std::unique_ptr<Primitive>>::const_iterator begin() { return _primitives.cbegin(); }

  std::vector<std::unique_ptr<Primitive>>::const_iterator end() { return _primitives.cend(); }

  void attach_view(const std::shared_ptr<DocumentView> &view);

  void detach_view();

private:
  std::vector<std::unique_ptr<Primitive>> _primitives;
  std::optional<std::string> _path;
  std::optional<std::shared_ptr<DocumentView>> _view;

  void notify() const;
};