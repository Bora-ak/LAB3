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
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(9000);

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind");
        return 1;
    }

    char buffer[1024];
    sockaddr_in cliaddr{};
    socklen_t len = sizeof(cliaddr);

    std::cout << "Server listening on port 9000...\n";

    int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cliaddr, &len);

    CarStatus status;
    if (status.ParseFromArray(buffer, n)) {
        std::cout << "Received: speed=" << status.speed()
                  << " rpm=" << status.rpm()
                  << " gear=" << status.gear() << std::endl;
    }

    std::string reply = "Ack: got your car status!";
    sendto(sockfd, reply.c_str(), reply.size(), 0, (struct sockaddr*)&cliaddr, len);

    close(sockfd);
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
