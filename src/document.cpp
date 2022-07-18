#include <algorithm>
#include "document.h"

std::optional<std::string> Document::current_filename() {
  return _path;
}

bool Document::save_as(const std::string &filename) {
  _path = {filename};
  return save();
}

bool Document::save() {
  notify();
  return true;
}

void Document::add_primitive(std::unique_ptr<Primitive> &&primitive) {
  _primitives.push_back(std::move(primitive));
  notify();
}

bool Document::remove_primitive(const Primitive &primitive) {
  auto it = std::find_if(_primitives.begin(), _primitives.end(), [&](std::unique_ptr<Primitive> const &_other) {
    return *_other == primitive;
  });
  bool will_delete = it != _primitives.end();
  if (will_delete) _primitives.erase(it);
  notify();
  return will_delete;
}

void Document::attach_view(const std::shared_ptr<DocumentView> &view) {
  _view = {view};
  notify();
}

void Document::detach_view() {
  notify();
  _view = std::nullopt;
}

void Document::notify() const {
  if (auto view = _view.value()) {
    view->render();
  }
}

std::optional<Primitive *> Document::find_at(const Point &point) const {
  for (const auto &prim: _primitives) {
    if (prim->contains(point)) return {prim.get()};
  }
  return std::nullopt;
}
