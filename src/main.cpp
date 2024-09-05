#include "./server_infrastructure_utils.h"

int main() {
  int16_t number_of_replicas = 2;
  server_infrastructure_utils::GreedyGrouper::Group(
      server_infrastructure_utils::JsonServersParser::Parse(),
      number_of_replicas);
}