#ifndef _READER_H_
#define _READER_H_

class Reader {
public:
	Reader(const char file[]);
	virtual ~Reader();
	int readFile(char buf[], const int& total_len) const;
private:
	int check_fd() const;
	
	int _fd;
};

#endif
