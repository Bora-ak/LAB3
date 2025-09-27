#include <iostream>
#include <vector>
#include "udp.hpp"
#include "simple_message.pb.h"

int main() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  UdpSocket sock;

  lab3::SimpleMessage msg;
  msg.set_sender_name("client-bora");

  std::string out;
  msg.SerializeToString(&out);
  std::vector<uint8_t> bytes(out.begin(), out.end());

  UdpEndpoint server{"127.0.0.1", 9000};
  if (!sock.send_to(server, bytes)) {
    std::cerr << "send_to failed\n"; return 1;
  }

  std::vector<uint8_t> buf(2048);
  UdpEndpoint from;
  int n = sock.recv_from(buf, from);
  if (n <= 0) { std::cerr << "recv failed\n"; return 1; }

  lab3::SimpleMessage reply;
  if (!reply.ParseFromArray(buf.data(), n)) {
    std::cerr << "reply parse failed\n"; return 1;
  }
  std::cout << "Server replied: \"" << reply.sender_name() << "\"\n";

  google::protobuf::ShutdownProtobufLibrary();
  return 0;
}
