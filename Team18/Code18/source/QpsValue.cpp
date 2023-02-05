#include "QpsValue.h"

#include <string>

spa::QpsValue::QpsValue(std::string val) {
  this->type = QpsValueType::STRING;
  this->strValue = val;
}

spa::QpsValue::QpsValue(int val) {
  this->type = QpsValueType::INTEGER;
  this->numValue = val;
}

spa::QpsValue::QpsValue(const QpsValue& other) {
  this->type = other.type;
  if (type == QpsValueType::INTEGER) {
    this->numValue = other.getInteger();
  } else {
    this->strValue = other.getString();
  }
}

spa::QpsValue& spa::QpsValue::operator=(const QpsValue& other) {
  this->type = other.type;
  if (type == QpsValueType::INTEGER) {
    this->numValue = other.getInteger();
  } else {
    this->strValue = other.getString();
  }
  return *this;
}

spa::QpsValueType spa::QpsValue::getType() const {
  return type;
}

const std::string& spa::QpsValue::getString() const {
  return strValue.value();
}

const int& spa::QpsValue::getInteger() const {
  return numValue.value();
}

const std::string spa::QpsValue::toString() const {
  if (strValue) {
    return getString();
  }

  return std::to_string(getInteger());
}

bool spa::operator==(const QpsValue& v1, const QpsValue& v2) {
  if (v1.type != v2.type) {
    return false;
  }
  if (v1.type == QpsValueType::STRING) {
    return v1.strValue.value() == v2.strValue.value();
  }
  return v1.numValue.value() == v2.numValue.value();
}

bool spa::operator!=(const QpsValue& v1, const QpsValue& v2) {
  return !(v1 == v2);
}

size_t spa::QpsValueHash::operator()(const QpsValue& val) const {
  if (val.getType() == QpsValueType::STRING) {
    return std::hash<std::string>()(val.getString());
  }
  return std::hash<int>()(val.getInteger());
}

bool spa::QpsValueEquality::operator()(const QpsValue& val1, const QpsValue& val2) const {
  return val1 == val2;
}
