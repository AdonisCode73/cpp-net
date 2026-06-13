#pragma once 

#include <unistd.h>
#include <netdb.h>

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
        socket tcp_listener(addrinfo hints, int level, int optname, int optval);
        socket accept(const socket& listener);
}
