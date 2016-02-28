#include <stdio.h>
#include <fcntl.h> // open
#include <unistd.h> // close

#include "reader.h"

Reader::Reader(const char file[]):_fd(-1) {
	_fd = open(file, O_RDONLY);
	if(_fd < 0) fprintf(stderr, "Reader open %s file fail\n", file);
}

Reader::~Reader(){
	if(check_fd() < 0) return ;
	_fd = close(_fd);
	if(_fd < 0) fprintf(stderr, "Reader close file fail\n");
}

int Reader::readFile(char buf[], const int& total_len) const {
	if(check_fd() < 0) return -1;
	int len = read(_fd, buf, total_len);
	if(len<0) fprintf(stderr, "Reader read file descriptor %d fail\n", _fd);
	return len;
}

int Reader::check_fd() const {
	if(_fd < 0) {
		fprintf(stderr, "Reader file descriptor < 0\n");
		return -1;
	}
	return 0;
}
