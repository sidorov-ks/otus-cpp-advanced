#pragma once

class Document;

#include "document.h"

#include <memory>

class DocumentView {
public:
  DocumentView(const std::shared_ptr<Document> document) : _document(document) {}

  void render() const;

private:
  std::shared_ptr<Document> _document;
};
