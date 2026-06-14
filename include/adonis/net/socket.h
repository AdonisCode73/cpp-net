#pragma once 

#include <system_error>
#include <unistd.h>
#include <netdb.h>
#include <string>
#include <expected>

namespace adonis::net {
    class socket {
    public:
        explicit socket(int fd); 
        ~socket(); 
        socket(const socket&) = delete;
        socket& operator=(const socket&) = delete;

        socket(socket&& other) noexcept;
        socket& operator=(socket&& other) noexcept;

        int get_fd() const { return m_fd; }  

    private:
        int m_fd = -1;
    };
        socket ipv4_tcp_listener(std::string name, std::string port, int optval);
        std::expected<socket, std::error_code> accept(const socket& listener);
}
