#include <vector>
#include <algorithm>
using namespace std;

#include <stdio.h>

#include "reader.h"
#include "writer.h"

#define MAX_UNIT_SIZE 8 // (bytes)
#define MEM_SIZE 32 // (bytes)
#define VECTOR_SIZE 32

class Unit {
public:
	Unit(const char* b = NULL, const int& s = 0): _buf(b), _size(s) {}
	bool operator < (const Unit& u){
		int min = (_size < u._size) ? _size : u._size;
		for(int i=0; i<min; ++i)
			if(_buf[i] != u._buf[i]) return (_buf[i] < u._buf[i]);
		return (_size < u._size);
	}
	void set(const char* b, const int s){
		//printf("s=%d\n",s);
		_buf = b;
		_size = s;
	}
	void show() const {
		for(int i=0; i<_size; ++i) printf("%c", _buf[i]);
	}
	const char* get_buf() const { return _buf; }
	int get_size() const { return _size; }
private:
	const char* _buf;
	int _size;
};


class IdIssuer {
public:
	IdIssuer(){ 
		for(int i=0; i<3; ++i) _id[i] = 'a';
		_id[3] = '\0';
	}
	IdIssuer& operator ++(){
		int num = 1;
		for(int i=0; i<3; ++i){
			num += (_id[i] - 'a');
			_id[i] = num%26 + 'a';
			num /= 26;
		}
		return (*this);
	}
	/*IdIssuer& operator --(){
		for(int i=0; i<3; ++i){
			if(_id[i]=='a') _id[i] = 'z';
			else {
				--_id[i];
				break;
			}
		}
		return (*this);
	}*/
	IdIssuer& operator = (const IdIssuer& other) {
		for(int i=0; i<3; ++i) _id[i] = other._id[i];
		return (*this);
	}
	operator char* (){ return _id; }
private:
	char _id[3+1];
};

class Parser {
public:
	Parser(char* const m): size(0), len(0), mem(m), buf(m), ptr(m) {}
	int contiRead(const Reader& reader, const int& mem_size){
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
		return 0;
	}
	void next(){
		while(size-- && ptr[len++]!='\n');
	}
	//bool isEnd() const { printf("%d\n", size); return size<0; }
	bool isEnd() const { return size<0; }
	const char* get_ptr(){ return ptr; }
	int get_len() { return len; }
	void reset_ptr() {
		ptr += (len);
		len = 0;
	}
private:
	int size;
	int len;
	char* const mem;
	char* buf;
	const char* ptr;
};

int appendRest(Parser& parser, const Writer& writer){
	while(!parser.isEnd()){
		parser.next();
		if(!parser.isEnd()){
			if(writer.writeFile(parser.get_ptr(), parser.get_len()) < 0){
				fprintf(stderr, "Writer.writeFile fail\n");
				return -1;
			}
			parser.reset_ptr();
		}
	}
	return 0;
}


int main(int argc, char* argv[]){
	if(argc < 3){
		fprintf(stderr, "Usage: %s < input file > < output file >\n", argv[0]);
		return -1;
	}

	char mem[MEM_SIZE];
	vector<Unit> v(VECTOR_SIZE);
	Reader reader(argv[1]);
	IdIssuer id;
	IdIssuer id_start = id;
	int count = 0;

	Parser parser(mem);
	while(parser.contiRead(reader, MEM_SIZE) == 0){
		int v_size = 0;
		while(!parser.isEnd()){
			parser.next();
			if(!parser.isEnd()){
				v[v_size++].set(parser.get_ptr(), parser.get_len());
				parser.reset_ptr();
			}
		}
		//for(int i=0; i<v_size; ++i) v[i].show();
		sort(v.begin(), v.begin()+v_size);
		//for(int i=0; i<v_size; ++i) v[i].show();
		{
			Writer writer(id);
			for(int i=0; i<v_size; ++i) 
				if(writer.writeFile(v[i].get_buf(), v[i].get_size()) < 0){
					fprintf(stderr, "Writer.writeFile fail\n");
					return -1;
				}
		}
		/*
		if(remove(id) != 0){
			fprintf(stderr, "remove file fail\n");
			return -1;
		}*/
		++id;
		++count;
	}
	while(count>1){
		for(int kk=1; kk<count; kk=kk+2) {
			Reader reader1(id_start);
			++id_start;
			Reader reader2(id_start);
			++id_start;
			Writer writer(id);
			++id;

			const int mem_size = MEM_SIZE/2;
			Parser parser1(mem);
			Parser parser2(mem + mem_size);

			int turn = 0;
			while(1){
				if(turn==1 || turn==0) if(parser1.contiRead(reader1, mem_size) < 0) break;
				if(turn==2 || turn==0) if(parser2.contiRead(reader2, mem_size) < 0) break;
				while(!( parser1.isEnd() || parser2.isEnd() )){
					if(turn==0) {
						parser2.next();
						if(!parser2.isEnd()) v[2].set(parser2.get_ptr(), parser2.get_len());
						turn=1;
					}
					if(turn==1){
						parser1.next();
						if(!parser1.isEnd()) v[1].set(parser1.get_ptr(), parser1.get_len());
					}
					else{
						parser2.next();
						if(!parser2.isEnd()) v[2].set(parser2.get_ptr(), parser2.get_len());
					}
					if( (turn==1) ? !parser1.isEnd() : !parser2.isEnd() ){
						turn = (v[1] < v[2]) ? 1 : 2; 
						if(writer.writeFile(v[turn].get_buf(), v[turn].get_size()) < 0){
							fprintf(stderr, "Writer.writeFile fail\n");
							return -1;
						}
						v[turn].show();
						if(turn==1) parser1.reset_ptr();
						else parser2.reset_ptr();
					}
					//printf("turn=%d\n",turn);
				}
			}
			if(!parser1.isEnd()) {
				v[1].show();
				if(writer.writeFile(v[1].get_buf(), v[1].get_size()) < 0){
					fprintf(stderr, "Writer.writeFile fail\n");
					return -1;
				}
				parser1.reset_ptr();
				do{ appendRest(parser1, writer);
				}while(parser1.contiRead(reader1, mem_size) == 0);
			}
			else {
				v[2].show();
				if(writer.writeFile(v[2].get_buf(), v[2].get_size()) < 0){
					fprintf(stderr, "Writer.writeFile fail\n");
					return -1;
				}
				parser2.reset_ptr();
				do{ appendRest(parser2, writer);
				}while(parser2.contiRead(reader2, mem_size) == 0);
			}
		}
		count = count/2 + count%2;
	}
	return 0;
}
