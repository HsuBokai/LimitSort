#include <stdio.h>
#include <fcntl.h> // open
#include <unistd.h> // close

#include "writer.h"

Writer::Writer(const char file[]):_fd(-1) {
	_fd = open(file, O_WRONLY | O_CREAT | O_EXCL | O_FSYNC);
	if(_fd < 0) fprintf(stderr, "Writer create %s file fail\n", file);
}

Writer::~Writer(){
	if(check_fd() < 0) return ;
	_fd = close(_fd);
	if(_fd < 0) fprintf(stderr, "Writer close file fail\n");
}

int Writer::writeFile(const char* ptr, int total_len) const {
	if(check_fd() < 0) return -1;
	while(total_len > 0){
		int len = write(_fd, ptr, total_len);
		if(len<0){
			fprintf(stderr, "Writer write file descriptor %d fail\n", _fd);
			return -1;
		}
		total_len -= len;
		ptr += len;
	}
	return 0;
}

int Writer::check_fd() const {
	if(_fd < 0) {
		fprintf(stderr, "Writer file descriptor < 0\n");
		return -1;
	}
	return 0;
}


