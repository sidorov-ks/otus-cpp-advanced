#include "document_view.h"

#include <iostream>

void DocumentView::render() const {
  std::cout << "Document " << _document->current_filename().value_or("*Untitled*") << "\n";
  for (const auto &it: *_document) {
    it->render();
    std::cout << "\n";
  }
}
