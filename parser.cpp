#include "stdio.h"
#include "writer.h"
#include "reader.h"
#include "parser.h"

Parser::Parser(char* const m): size(0), len(0), mem(m), buf(m), ptr(m) {}

int Parser::contiRead(const Reader& reader, const int& mem_size){
	buf = mem;
	while(len--) *(buf++) = *(ptr++);
	ptr = mem;
	len = 0;
	size = reader.readFile(buf, mem + mem_size - buf);
	if(size==0) {
		size = -1;
		return -2;
	}
	if(size<0){
		fprintf(stderr, "Reader.readFile fail\n");
		return -1;
	}
	size += (buf - mem);
	next();
	return 0;
}

void Parser::next(){
	while(size-- && ptr[len++]!='\n');
}

bool Parser::isEnd() const { return size<0; }

const char* Parser::get_ptr() const { return ptr; }

const int& Parser::get_len() const { return len; }

void Parser::reset_ptr() {
	ptr += (len);
	len = 0;
}

