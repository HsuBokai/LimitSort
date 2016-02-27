#ifndef _RAND_H_
#define _RAND_H_

class Rand {
public:
	Rand();
	virtual ~Rand();
	int get(unsigned char buf[]) const;
private:
	int _fd;
	int check_fd() const;
};

#endif
