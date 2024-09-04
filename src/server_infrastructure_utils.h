#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

namespace server_infrastructure_utils {

struct ServerData {
  std::string name;
  int16_t size;

  friend inline bool operator<(const ServerData& lhs, const ServerData& rhs) {
    return lhs.size < rhs.size;
  }
  friend inline bool operator>(const ServerData& lhs, const ServerData& rhs) {
    return rhs < lhs;
  }
  friend inline bool operator<=(const ServerData& lhs, const ServerData& rhs) {
    return !(lhs > rhs);
  }
  friend inline bool operator>=(const ServerData& lhs, const ServerData& rhs) {
    return !(lhs < rhs);
  }
  friend inline bool operator==(const ServerData& lhs, const ServerData& rhs) {
    return lhs.size == rhs.size;
  }
  friend inline bool operator!=(const ServerData& lhs, const ServerData& rhs) {
    return !(lhs == rhs);
  }

  friend std::istream& operator>>(std::istream& is, ServerData& obj) {
    std::cin >> obj.name;
    std::cin >> obj.size;
    return is;
  }
};

struct ServerClasterData {
  std::vector<ServerData> servers;
};

struct GroupedServerClasterData {
  std::vector<std::vector<ServerData>> shells;

  GroupedServerClasterData(int16_t number_of_replicas) {
    shells = std::vector<std::vector<ServerData>>(number_of_replicas,
                                                  std::vector<ServerData>());
  }
};

class IInputParser {
 public:
  virtual std::unique_ptr<ServerClasterData> Parse() = 0;
};

class JsonParser : IInputParser {
 public:
  virtual std::unique_ptr<ServerClasterData> Parse() override;
};

class TerminalParser : IInputParser {
 public:
  virtual std::unique_ptr<ServerClasterData> Parse() override;
};

class IGrouper {
 public:
  virtual std::unique_ptr<GroupedServerClasterData> Group(
      const ServerClasterData& data) = 0;

 private:
};

class FastGrouper : IGrouper {
 public:
  FastGrouper(int16_t number_of_replicas)
      : _number_of_replicas(number_of_replicas) {}

  std::unique_ptr<GroupedServerClasterData> Group(
      const ServerClasterData& data) override;

 private:
  int16_t _number_of_replicas;
};

}  // namespace server_infrastructure_utils