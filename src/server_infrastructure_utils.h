#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

namespace server_infrastructure_utils {

struct ServerData {
  std::string name;
  int64_t size;

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

class JsonServersParser {
 public:
  static ServerClasterData Parse();

 private:
  static int64_t ParseSize(const std::string& s_size);
};

class TerminalServersParser {
 public:
  static ServerClasterData Parse();
};

class GreedyGrouper {
 public:
  static GroupedServerClasterData Group(const ServerClasterData& data,
                                        const int16_t& number_of_replicas);
};

}  // namespace server_infrastructure_utils