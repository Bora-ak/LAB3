#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include "carstatus.pb.h"

int main() {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) { perror("socket"); return 1; }

    sockaddr_in servaddr{};
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9000);
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);

    CarStatus status;
    status.set_speed(100);
    status.set_rpm(3000);
    status.set_gear("D");

    std::string out;
    status.SerializeToString(&out);

    sendto(sockfd, out.data(), out.size(), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));

    char buffer[1024];
    socklen_t len = sizeof(servaddr);
    int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&servaddr, &len);

    std::cout << "Server says: " << std::string(buffer, n) << std::endl;

    close(sockfd);
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
