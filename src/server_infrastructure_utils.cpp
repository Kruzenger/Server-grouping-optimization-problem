#include "./server_infrastructure_utils.h"

#include <fstream>
#include <map>
#include <nlohmann/json.hpp>

namespace server_infrastructure_utils {

int64_t JsonServersParser::ParseSize(const std::string& s_size) {
  int64_t size = 0;
  for (int i = 0; i < s_size.size() - 3; ++i) {
    size *= 10;
    size += s_size[i] - '0';
  }
  return size;
}

ClasterData JsonServersParser::Parse(const std::string& path_to_file) {
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
}

ClasterData TerminalServersParser::Parse() {
  ClasterData parsed_data = ClasterData();

  int16_t num_of_servers = 0;
  std::cin >> num_of_servers;
  for (int i = 0; i < num_of_servers; ++i) {
    ServerData server_data;
    std::cin >> server_data;
    parsed_data.servers.emplace(parsed_data.servers.end(), server_data);
  }

  return parsed_data;
}

GroupedServerCores GreedyGrouper::Group(const ClasterData& data,
                                        const int16_t& number_of_replicas) {
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
}

}  // namespace server_infrastructure_utils