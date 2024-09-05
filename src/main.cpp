#include "./server_infrastructure_utils.h"

int main() {
  server_infrastructure_utils::GreedyGrouper::Group(
      server_infrastructure_utils::JsonServersParser::Parse(), 2);
}