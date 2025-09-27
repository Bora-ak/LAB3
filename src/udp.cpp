#include "udp.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

UdpSocket::UdpSocket() {
  fd_ = ::socket(AF_INET, SOCK_DGRAM, 0);
  if (fd_ < 0) perror("socket");
}

UdpSocket::~UdpSocket() {
  if (fd_ >= 0) ::close(fd_);
}

bool UdpSocket::bind(uint16_t port) {
  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(port);

  int enable = 1;
  ::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));

  if (::bind(fd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
    perror("bind");
    return false;
  }
  return true;
}

bool UdpSocket::send_to(const UdpEndpoint& to, const std::vector<uint8_t>& bytes) const {
  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(to.port);
  if (::inet_pton(AF_INET, to.host.c_str(), &addr.sin_addr) != 1) {
    std::cerr << "inet_pton failed for host " << to.host << "\n";
    return false;
  }
  ssize_t n = ::sendto(fd_, bytes.data(), bytes.size(), 0,
                       reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
  return n == static_cast<ssize_t>(bytes.size());
}

int UdpSocket::recv_from(std::vector<uint8_t>& buffer, UdpEndpoint& from) const {
  sockaddr_in addr{};
  socklen_t alen = sizeof(addr);
  int n = ::recvfrom(fd_, buffer.data(), buffer.size(), 0,
                     reinterpret_cast<sockaddr*>(&addr), &alen);
  char ip[INET_ADDRSTRLEN];
  ::inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));
  from.host = ip;
  from.port = ntohs(addr.sin_port);
  return n;
}
