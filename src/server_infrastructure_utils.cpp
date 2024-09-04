#include "./server_infrastructure_utils.h"

#include <map>
#include <nlohmann/json.hpp>

namespace server_infrastructure_utils {

std::unique_ptr<ServerClasterData> JsonParser::Parse() {}

std::unique_ptr<ServerClasterData> TerminalParser::Parse() {
  std::unique_ptr<ServerClasterData> parsed_data =
      std::unique_ptr<ServerClasterData>(new ServerClasterData());
  int16_t num_of_servers = 0;
  std::cin >> num_of_servers;
  for (int i = 0; i < num_of_servers; ++i) {
    ServerData server_data;
    std::cin >> server_data;
    parsed_data->servers.emplace(parsed_data->servers.end(), server_data);
  }
  return parsed_data;
}

std::unique_ptr<GroupedServerClasterData> FastGrouper::Group(
    const ServerClasterData& data) {
  std::unique_ptr<GroupedServerClasterData> result =
      std::unique_ptr<GroupedServerClasterData>(
          new GroupedServerClasterData(_number_of_replicas));
  std::vector<ServerData> servers(data.servers.begin(), data.servers.end());
  std::sort(servers.begin(), servers.end(), std::greater<ServerData>());
  std::multimap<int64_t, int16_t> groups;
  for (int i = 0; i < _number_of_replicas; ++i) {
    groups.insert({0, i});
  }
  for (ServerData server : servers) {
    auto minimum_iter = groups.begin();
    result->shells[minimum_iter->second].push_back(server);
    std::pair<int64_t, int16_t> changed_map_element = {
        minimum_iter->first + server.size, minimum_iter->second};
    groups.extract(minimum_iter);
    groups.insert(changed_map_element);
  }
  return result;
}

}  // namespace server_infrastructure_utils