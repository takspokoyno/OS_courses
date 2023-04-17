#include <iostream>
#include <iomanip>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc != 4) {
        return -1;
    }
    std::string pid{argv[1]};
    auto start = std::stoull(argv[2], nullptr, 16);
    auto size = std::stoull(argv[3]);
    auto filename = "/proc/" + pid + "/mem";
    std::cout << "Filename: " << filename << "\nAddr is: " << (void *) start << "\nSize: " << size << "\n";
    while(true){
	int fd = open(filename.c_str(), O_RDONLY);
	if(fd == -1) {
	    std::cout << "Not opened\n";
	    return -2;
    	}

	off_t res = lseek(fd, (off_t)start, SEEK_SET);
    	if(res == (off_t)-1) {
	    std::cout << "Not lseeked\n";
            return -3;
    	}

        std::string output(size, 0);
        auto read_res = read(fd, const_cast<char*>(output.data()), output.size());
        std::cout << "Data is: " << std::quoted(output) <<"\n";
	sleep(3);
    }
    return 0;
}
