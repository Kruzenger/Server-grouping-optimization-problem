#include "./server_infrastructure_utils.h"

int main() {
  server_infrastructure_utils::FastGrouper(2).Group(
      *server_infrastructure_utils::TerminalParser().Parse().get());
}