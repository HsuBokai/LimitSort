#include <stdio.h>
#include <fcntl.h> // open
#include <unistd.h> // close

#include "rand.h"

Rand::Rand():_fd(-1) {
	_fd = open("/dev/urandom", O_RDONLY);
	if(_fd < 0) fprintf(stderr, "open /dev/urandom file fail\n");
}

Rand::~Rand(){
	if(check_fd() < 0) return ;
	_fd = close(_fd);
	if(_fd < 0) fprintf(stderr, "close/dev/urandom file fail\n");
}

int Rand::get(unsigned char buf[]) const {
	if(check_fd() < 0) return -1;
	if(read(_fd, buf, 1)<=0){
		fprintf(stderr, "read /dev/urandom file fail\n");
		return -1;
	}
	return 0;
}

int Rand::check_fd() const {
	if(_fd < 0) {
		fprintf(stderr, "/dev/urandom file descriptor < 0\n");
		return -1;
	}
	return 0;
}
