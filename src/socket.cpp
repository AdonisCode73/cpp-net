#include "adonis/net/socket.h"
#include <filesystem>
#include <unistd.h>
#include <utility>

#include <asm-generic/socket.h>
#include <cerrno>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <system_error>
#include <memory>
#include <expected>

namespace adonis::net {
    socket::socket(int fd) : m_fd(fd) {}
    socket::~socket() {
        if (m_fd >= 0) {
            ::close(m_fd);
        }
    }

    socket::socket(socket&& other) noexcept : m_fd(std::exchange(other.m_fd, -1)) {};
    socket& socket::operator=(socket&& other) noexcept {
        if (this != &other) {
            if (m_fd >= 0) {
                ::close(m_fd);
            }
            m_fd = std::exchange(other.m_fd, -1);
        }

        return *this;
    }

    socket ipv4_tcp_listener(std::string name, std::string port, int optval) {

        addrinfo hints{};
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        if (name.empty()) {
            hints.ai_flags = AI_PASSIVE;
        }
        
        addrinfo* res = nullptr;
        if (int status = getaddrinfo(
        name.empty() ? NULL : name.c_str(),
    port.empty() ? NULL : port.c_str(),
        &hints, &res); status != 0) {
            throw std::runtime_error(std::string{"getaddrinfo: "} + gai_strerror(status));
        }
    
        std::unique_ptr<addrinfo, decltype(&freeaddrinfo)> res_guard{res, freeaddrinfo};

        for (addrinfo* p = res; p != NULL; p = p->ai_next) {
            int fd = ::socket(p->ai_family, p->ai_socktype, p->ai_protocol);
            if (fd < 0) continue;

            socket sock{fd};
            
            if (optval) {
                if (setsockopt(sock.get_fd(), SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
                    throw std::system_error(errno, std::system_category(), "setsockopt(REUSEADDR)");
                }
            }

            if (bind(sock.get_fd(), p->ai_addr, p->ai_addrlen) == 0) {
                return sock;
            }
        }

        throw std::runtime_error("could not bind to any address");
    }

    std::expected<socket, std::error_code> accept(const socket& listener) {
        sockaddr_storage theirAddr;
        socklen_t addrSize = sizeof(theirAddr);

        int fd = ::accept(listener.get_fd(), reinterpret_cast<sockaddr *>(&theirAddr),& addrSize);

        if (fd < 0) {
            return std::unexpected(std::error_code(errno, std::system_category()));
        }
        
        return socket{fd};
    }
}
