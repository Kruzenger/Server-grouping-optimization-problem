#include "./server_infrastructure_utils.h"

#include <fstream>
#include <map>
#include <nlohmann/json.hpp>

namespace server_infrastructure_utils {

std::istream& operator>>(std::istream& is, ServerData& obj) {
  try {
    std::cin >> obj.name;
    std::cin >> obj.size;
  } catch (...) {
    throw;
  }
  return is;
}

GroupedServerCores::GroupedServerCores(int16_t number_of_replicas) {
  try {
    cores = std::vector<std::vector<ServerData>>(number_of_replicas,
                                                 std::vector<ServerData>());
  } catch (...) {
    throw;
  }
}

GroupedServerCores::GroupedServerCores(const GroupedServerCores& other) {
  try {
    cores = std::vector<std::vector<ServerData>>(other.cores);
  } catch (...) {
    throw;
  }
}

GroupedServerCores::GroupedServerCores(GroupedServerCores&& other) noexcept {
  this->cores = std::vector<std::vector<ServerData>>(std::move(other.cores));
}

GroupedServerCores& GroupedServerCores::operator=(
    const GroupedServerCores& other) {
  try {
    this->cores = other.cores;
    return *this;
  } catch (...) {
    throw;
  }
}

GroupedServerCores& GroupedServerCores::operator=(
    GroupedServerCores&& other) noexcept {
  this->cores = std::move(other.cores);
  return *this;
}

int64_t JsonServersParser::ParseSize(const std::string& s_size) {
  try {
    int64_t size = 0;
    for (int i = 0; i < s_size.size() - 3; ++i) {
      size *= 10;
      size += s_size[i] - '0';
    }
    return size;
  } catch (...) {
    throw;
  }
}

ClasterData JsonServersParser::Parse(const std::string& path_to_file) {
  try {
    ClasterData parsed_data = ClasterData();
    std::ifstream file("../" + path_to_file);
    nlohmann::json data = nlohmann::json::parse(file);

    bool end_of_servers = false;
    int i = 0;
    while (!end_of_servers) {
      ServerData server_data = ServerData();
      try {
        server_data.name = data["Servers"].at(i)["name"];
        server_data.size = ParseSize(data["Servers"].at(i)["size"]);

        parsed_data.servers.push_back(server_data);
        ++i;
      } catch (...) {
        end_of_servers = true;
      }
    }
    return parsed_data;
  } catch (...) {
    throw;
  }
}

ClasterData TerminalServersParser::Parse() {
  try {
    ClasterData parsed_data = ClasterData();

    int16_t num_of_servers = 0;
    std::cin >> num_of_servers;
    for (int i = 0; i < num_of_servers; ++i) {
      ServerData server_data;
      std::cin >> server_data;
      parsed_data.servers.emplace(parsed_data.servers.end(), server_data);
    }

    return parsed_data;
  } catch (...) {
    throw;
  }
}

GroupedServerCores GreedyGrouper::Group(const ClasterData& data,
                                        const int16_t& number_of_replicas) {
  try {
    GroupedServerCores result = GroupedServerCores(number_of_replicas);

    std::vector<ServerData> servers(data.servers.begin(), data.servers.end());
    std::sort(servers.begin(), servers.end(), std::greater<ServerData>());

    std::multimap<int64_t, int16_t> groups;
    for (int i = 0; i < number_of_replicas; ++i) {
      groups.insert({0, i});
    }

    // Here starts Greedy Algorithm
    for (ServerData server : servers) {
      auto minimum_iter = groups.begin();
      result.cores[minimum_iter->second].push_back(server);
      std::pair<int64_t, int16_t> changed_map_element = {
          minimum_iter->first + server.size, minimum_iter->second};
      groups.extract(minimum_iter);
      groups.insert(changed_map_element);
    }
    return result;
  } catch (...) {
    throw;
  }
}

}  // namespace server_infrastructure_utils