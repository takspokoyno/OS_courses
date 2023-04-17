#include <iostream>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/input-event-codes.h>
#include <errno.h>

static const std::string evval[3] = {
    "RELEASED",
    "PRESSED ",
    "REPEATED"
};

int main() {
    const char *dev = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
    struct input_event ev;
    ssize_t n;
    int fd;

    fd = open(dev, O_RDONLY);
    if (fd == -1) {
        std::cerr << "Cannot open " << dev << ": " << strerror(errno) << ".\n";
        return EXIT_FAILURE;
    }

    while (true) {
        n = read(fd, &ev, sizeof ev);
        if (n == (ssize_t) -1) {
            if (errno == EINTR)
                continue;
            else
                break;
        } else if (n != sizeof ev) {
            errno = EIO;
            break;
        }
        if (ev.type == EV_KEY && ev.value >= 0 && ev.value <= 2)
            std::cout << evval[ev.value] << " 0x" << std::hex << ev.code  << " (" << std::dec << ev.code << ")\n";
    }

    std::cout.flush();
    std::cerr << strerror(errno) << ".\n";
    return EXIT_FAILURE;
}
