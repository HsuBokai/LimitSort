#ifndef _PARSER_H_
#define _PARSER_H_

class Parser {
public:
	Parser(char* const m);
	int contiRead(const Reader& reader, const int& mem_size) ;
	void next() ;
	bool isEnd() const ;
	const char* get_ptr() const ;
	const int& get_len() const ;
	void reset_ptr();
private:
	int size;
	int len;
	char* const mem;
	char* buf;
	const char* ptr;
};


#endif
