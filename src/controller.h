#pragma once

#include "primitive/point.h"
#include "primitive/primitive.h"
#include "document.h"
#include "document_view.h"

#include <memory>
#include <optional>

enum CursorType {
  LineCursor, CircleCursor
};

class Controller {
public:
  Controller() : _document(std::make_shared<Document>()), _view(std::make_shared<DocumentView>(_document)),
                 _cursor_loc() {
    _document->attach_view(_view);
  }

  ~Controller() { _document->detach_view(); }

  void open(const std::string &path);

  void save();

  void save_as(const std::string &path);

  void set_cursor(CursorType type);

  std::optional<Primitive *> click_at(const Point &loc);

  void drag_to(Point loc);

  int render();

private:
  std::shared_ptr<Document> _document;
  std::shared_ptr<DocumentView> _view;
  CursorType _cursor = LineCursor;
  std::optional<Point> _cursor_loc;

  static void render_help();

  void render_cursor();
};