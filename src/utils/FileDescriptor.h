#ifndef FILE_DESCRIPTOR_H
#define FILE_DESCRIPTOR_H // Macros for include guard

#include <unistd.h> // for close()
#include <utility> // for std::move

namespace utils{

class FileDescriptor {
public:
    explicit FileDescriptor(int fd)
        : fd_(fd) {}
        ~FileDescriptor() {
        if (fd_ >= 0) {
            close(fd_);
        }
    }

    // Delete copy constructor and assignment operator to prevent copying
    FileDescriptor(const FileDescriptor&) = delete;
    FileDescriptor& operator=(const FileDescriptor&) = delete;

    // Allow move semantics
    FileDescriptor(FileDescriptor&& other) noexcept // && means rvalue reference, used for move semantics
         : fd_(-1) {
                fd_ = other.fd_;
                other.fd_ = -1; // Invalidate the moved-from object
        }

    FileDescriptor& operator=(FileDescriptor&& other) noexcept {
        if (this != &other) {
            if (fd_ >= 0) {
                close(fd_);
            }
            fd_ = other.fd_;
            other.fd_ = -1; // Invalidate the moved-from object
        }
        return *this;
    }


    int get() const {
        return fd_;
    }

    bool valid() const { // const means this function does not modify the state of the object
        return fd_ >= 0;
    }



private:
    int fd_;
        
};

}

#endif // FILE_DESCRIPTOR_H 