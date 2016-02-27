#ifndef _WRITER_H_
#define _WRITER_H_

class Writer {
public:
	Writer(const char file[]);
	virtual ~Writer();
	int writeFile(const char* ptr, int total_len) const;
private:
	int _fd;
	int check_fd() const;
};

#endif
