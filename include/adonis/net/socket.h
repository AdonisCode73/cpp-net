#pragma once 

#include <unistd.h>

namespace adonis::net {
    class Socket {
    public:
        explicit Socket(int fd); 
        ~Socket(); 
        Socket(const Socket&) = delete;
        Socket& operator=(const Socket&) = delete;

        Socket(Socket&& other) noexcept;
        Socket& operator=(Socket&& other) noexcept;

        int get_fd() const { return m_fd; }
        
        Socket socket();

    private:
        int m_fd = -1;
    };
}
