#include <iostream>
#include <vector>
#include "udp.hpp"
#include "simple_message.pb.h"

int main() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  UdpSocket sock;
  if (!sock.bind(9000)) return 1;
  std::cout << "Server listening on UDP :9000\n";

  std::vector<uint8_t> buf(2048);
  UdpEndpoint from;

  int n = sock.recv_from(buf, from);
  if (n <= 0) { std::cerr << "recv failed\n"; return 1; }

  lab3::SimpleMessage msg;
  if (!msg.ParseFromArray(buf.data(), n)) {
    std::cerr << "protobuf parse failed\n"; return 1;
  }
  std::cout << "Received from " << from.host << ":" << from.port
            << " -> sender_name=\"" << msg.sender_name() << "\"\n";

  lab3::SimpleMessage reply;
  reply.set_sender_name("server-ht08");

  std::string out;
  reply.SerializeToString(&out);
  std::vector<uint8_t> bytes(out.begin(), out.end());
  sock.send_to(from, bytes);

  google::protobuf::ShutdownProtobufLibrary();
  return 0;
}
