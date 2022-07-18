#include "controller.h"
#include "primitive/line.h"
#include "primitive/circle.h"

#include <iostream>
#include <string>

int Controller::render() {
  render_help();
  while (true) {
    render_cursor();
    std::cout << "Enter command: ";
    std::string cmd;
    std::cin >> cmd;
    if (cmd == "quit") return 0;
    else if (cmd == "help") render_help();
    else if (cmd == "open") {
      std::string filename;
      std::cin >> filename;
      _document = std::make_shared<Document>(filename);
      _view = std::make_shared<DocumentView>(_document);
      _document->attach_view(_view);
    } else if (cmd == "save") _document->save();
    else if (cmd == "export") {
      std::string filename;
      std::cin >> filename;
      _document->save_as(filename);
    } else if (cmd == "circle") set_cursor(CircleCursor);
    else if (cmd == "line") set_cursor(LineCursor);
    else if (cmd == "click") {
      double x, y;
      std::cin >> x >> y;
      auto primitive = click_at({x, y});
      if (primitive) {
        std::cout << "Selected primitive:\n";
        primitive.value()->render();
        std::cout << "\n";
      }
    } else if (cmd == "drag") {
      double x, y;
      std::cin >> x >> y;
      drag_to({x, y});
    } else if (cmd == "delete") {
      if (_cursor_loc) {
        auto prim = _document->find_at(_cursor_loc.value());
        if (prim) {
          _document->remove_primitive(*prim.value());
        }
      }
    }
  }
}

void Controller::render_cursor() {
  std::cout << "Selected tool " << _cursor << "\n";
  if (_cursor_loc)
    std::cout << "Cursor is located at " << _cursor_loc->x << ", " << _cursor_loc->y;
  else
    std::cout << "No location selected";
  std::cout << "\n";
}

void Controller::render_help() {
  std::cout << "Commands\n"
            << "\thelp: print help message\n"
            << "\topen <filename>: open document from disk\n"
            << "\tsave: save current document to disk\n"
            << "\texport <filename>: save current document to a new file\n"
            << "\tcircle: use circle cursor\n"
            << "\tline: use line cursor\n"
            << "\tclick x y: click to the point (x, y)\n"
            << "\tdrag x y: drag to the point (x, y)\n"
            << "\tdelete: remove primitive at cursor\n"
            << "\tquit: quit the editor\n";
}

void Controller::set_cursor(CursorType type) {
  _cursor = type;
}

std::optional<Primitive *> Controller::click_at(const Point &loc) {
  _cursor_loc = {loc};
  return _document->find_at(loc);
}

void Controller::drag_to(Point loc) {
  if (_cursor_loc) {
    auto cursor_loc = _cursor_loc.value();
    switch (_cursor) {
      case LineCursor:
        _document->add_primitive(std::make_unique<Line>(cursor_loc, loc));
        break;
      case CircleCursor:
        double dist = distance(cursor_loc, loc);
        _document->add_primitive(std::make_unique<Circle>(cursor_loc, dist));
        break;
    }
    _cursor_loc = std::nullopt;
  }
}

void Controller::open(const std::string &path) {
  _document->save();
  _document->detach_view();
  _document = std::make_shared<Document>(path);
  _view = std::make_shared<DocumentView>(_document);
  _document->attach_view(_view);
}

void Controller::save() {
  _document->save();
}

void Controller::save_as(const std::string &path) {
  _document->save_as(path);
}
