/**
 *   UCR-ECCI
 *   CI-0123 Proyecto integrador de redes y sistemas operativos
 *
 *   Intermediate server with SSL for PI. Project
 *
 **/
 
#include <iostream>
#include <string.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>
#include <vector>

#include <sys/socket.h>

#ifndef METHODSCOMMON_H
#define METHODSCOMMON_H

/// @brief 
/// @return 
std::string getIPAddress() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return "";
    }

    const char* googleDNS = "8.8.8.8";
    int port = 9;
    struct sockaddr_in serverAddress;
    std::memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    if (inet_pton(AF_INET, googleDNS, &(serverAddress.sin_addr)) <= 0) {
        std::cerr << "Invalid address" << std::endl;
        close(sock);
        return "";
    }

    if (connect(sock, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) < 0) {
        std::cerr << "Error connecting to server" << std::endl;
        close(sock);
        return "";
    }

    struct sockaddr_in localAddress;
    socklen_t addressLength = sizeof(localAddress);
    if (getsockname(sock, reinterpret_cast<struct sockaddr*>(&localAddress), &addressLength) < 0) {
        std::cerr << "Error getting local address" << std::endl;
        close(sock);
        return "";
    }

    close(sock);

    char ipAddress[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &(localAddress.sin_addr), ipAddress, INET_ADDRSTRLEN) == nullptr) {
        std::cerr << "Error converting address to string" << std::endl;
        return "";
    }

    return std::string(ipAddress);
}

/// @brief 
/// @param buffer 
/// @return 
std::vector<std::string> splitDiscover(std::string buffer){
    std::vector<std::string> discoverInfo;
    std::istringstream iss(buffer);
    std::string info;
    while (std::getline(iss, info, char(29))) {
        std::istringstream iss2(info);
        std::string subinfo;
        while (std::getline(iss2, subinfo, ':')) {
            discoverInfo.push_back(subinfo);
        }
    }
    return discoverInfo;
}

/// @brief 
/// @param buffer 
/// @return 
std::vector<std::string> splitPresent(std::string buffer){
    std::vector<std::string> presentInfo;
    std::istringstream iss(buffer);
    std::string info;
    while (std::getline(iss, info, char(29))) {
        std::istringstream iss2(info);
        std::string subinfo;
        while (std::getline(iss2, subinfo, ':')) {
            presentInfo.push_back(subinfo);
        }
    }
    return presentInfo;
}

#endif