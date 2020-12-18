// Copyright 2020, Verizon Media
// Licensed under the terms of MIT. See LICENSE file in project root for terms.

#ifndef MODEL_TAG_H
#define MODEL_TAG_H

#include <QString>
#include <QVariant>
#include <QDataStream>

namespace model {
class Tag {
 public:
  Tag() = default;
  ~Tag() = default;
  Tag(const Tag &) = default;
  Tag& operator=(const Tag&) = default;

  Tag(qint64 id, qint64 tagId, QString name) : Tag(tagId, name) { this->id = id; }
  Tag(qint64 tagId, QString name) {
    this->serverTagId = tagId;
    this->tagName = name;
  }

 public:
  friend QDataStream& operator<<(QDataStream& out, const model::Tag& v) {
    out << v.tagName << v.id << v.serverTagId;
    return out;
  }

  friend QDataStream& operator>>(QDataStream& in, model::Tag& v) {
    in >> v.tagName;
    in >> v.id;
    in >> v.serverTagId;
    return in;
  }

 public:
  qint64 id;
  qint64 serverTagId;
  QString tagName;
};
}  // namespace model

Q_DECLARE_METATYPE(model::Tag);
Q_DECLARE_METATYPE(std::vector<model::Tag>);
#endif  // MODEL_TAG_H
