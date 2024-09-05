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

  friend std::istream& operator>>(std::istream& is, ServerData& obj);
};

struct ClasterData {
  std::vector<ServerData> servers;
};

struct GroupedServerCores {
  std::vector<std::vector<ServerData>> cores;

  GroupedServerCores(int16_t number_of_replicas);
  GroupedServerCores(const GroupedServerCores& other);
  GroupedServerCores(GroupedServerCores&& other) noexcept;
  ~GroupedServerCores() = default;

  GroupedServerCores& operator=(const GroupedServerCores& other);
  GroupedServerCores& operator=(GroupedServerCores&& other) noexcept;
};

class JsonServersParser {
 public:
  static ClasterData Parse(const std::string& path_to_file = "./input.json");

 private:
  static int64_t ParseSize(const std::string& s_size);
};

class TerminalServersParser {
 public:
  static ClasterData Parse();
};

class GreedyGrouper {
 public:
  static GroupedServerCores Group(const ClasterData& data,
                                  const int16_t& number_of_replicas);
};

}  // namespace server_infrastructure_utils