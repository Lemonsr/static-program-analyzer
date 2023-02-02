#pragma once

#include <optional>
#include <string>

namespace spa {
enum class QpsValueType {
  STRING,
  INTEGER
};

class QpsValue {
 private:
  QpsValueType type;
  std::optional<std::string> strValue;
  std::optional<int> numValue;
 public:
  QpsValue() = default;
  explicit QpsValue(std::string val);
  explicit QpsValue(int val);
  QpsValue(const QpsValue& other);
  QpsValue& operator=(const QpsValue& other);
  QpsValueType getType() const;
  const std::string& getString() const;
  const int& getInteger() const;

  friend bool operator==(const QpsValue& v1, const QpsValue& v2);
  friend bool operator!=(const QpsValue& v1, const QpsValue& v2);
};

struct QpsValueHash {
  size_t operator()(const QpsValue& val) const;
};

struct QpsValueEquality {
  bool operator()(const QpsValue& val1, const QpsValue& val2) const;
};
}  // namespace spa
