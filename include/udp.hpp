#pragma once
#include <string>
#include <vector>
#include <cstdint>

struct UdpEndpoint {
  std::string host;
  uint16_t port;
};

class UdpSocket {
public:
  UdpSocket();
  ~UdpSocket();

  // bind local port on INADDR_ANY (server)
  bool bind(uint16_t port);

  // send to IPv4 host:port
  bool send_to(const UdpEndpoint& to, const std::vector<uint8_t>& bytes) const;

  // blocking receive, returns number of bytes, fills 'from'
  int recv_from(std::vector<uint8_t>& buffer, UdpEndpoint& from) const;

private:
  int fd_{-1};
};
