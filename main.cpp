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
	void set(const char* b, const int& s){
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
	int contiRead(const Reader& reader){
		buf = mem;
		while(len--) *(buf++) = *(ptr++);
		ptr = mem;
		len = 0;
		size = reader.readFile(buf, mem + MEM_SIZE - buf);
		if(size==0) return -2;
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
	bool isEnd() const {return size<0; }
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
	while(parser.contiRead(reader) == 0){
		int v_size = 0;
		while(!parser.isEnd()){
			parser.next();
			v[v_size++].set(parser.get_ptr(), parser.get_len());
			parser.reset_ptr();
		}
		/*
		sort(v.begin(), v.begin()+v_size);
		{
			Writer writer(id);
			for(int i=0; i<v_size; ++i) 
				if(writer.writeFile(v[i].get_buf(), v[i].get_size()) < 0){
					fprintf(stderr, "Writer.writeFile fail\n");
					return -1;
				}
		}
		if(remove(id) != 0){
			fprintf(stderr, "remove file fail\n");
			return -1;
		}*/
		++id;
		++count;
	}
	/*
	count = 3;
	count /= 2;
	while(count--){
		Reader reader1(id_start);
		++id_start;
		Reader reader2(id_start);
		++id_start;
		Writer writer(id);
		++id;
		char *mem1 = mem;
		const int mem_size = MEM_SIZE/2;
		char *mem2 = mem + mem_size;

		int turn = 0, size1 = 0, size2 = 0;

		char *buf1, *ptr1;
		int len1 = 0;
		char *buf2, *ptr2;
		int len2 = 0;
		while(1){
			if(turn==1 || turn==0){
				buf1 = mem1;
				while(len1--) *(buf1++) = *(ptr1++);
				ptr1 = mem1;
				len1 = 0;
				size1 = reader1.readFile(buf1, mem1 + mem_size - buf1);
				if(size1==0) break;
				if(size1<0){
					fprintf(stderr, "Reader.readFile fail\n");
					return -1;
				}
				size1 += (buf1 - mem1);
				printf("\nsize1=%d\n", size1);
			}
			if(turn==2 || turn==0){
				buf2 = mem2;
				while(len2--) *(buf2++) = *(ptr2++);
				ptr2 = mem2;
				len2 = 0;
				size2 = reader2.readFile(buf2, mem2 + mem_size - buf2);
				if(size2==0) break;
				if(size2<0){
					fprintf(stderr, "Reader.readFile fail\n");
					return -1;
				}
				size2 += (buf2 - mem2);
				printf("\nsize2=%d\n", size2);
			}
			while(size1 >= 0 && size2 >= 0){
				if(turn==0) {
					while(size2-- && ptr2[len2++]!='\n');
					if(size2 >= 0) v[2].set(ptr2, len2);
					turn=1;
				}
				if(turn==1){
					while(size1-- && ptr1[len1++]!='\n');
					if(size1 >= 0){
						v[1].set(ptr1, len1);
						if(v[1] < v[2]){
							turn = 1;
							v[1].show();
							//printf("len1=%d\n", len1);
							ptr1 += (len1);
							len1 = 0;
						}
						else{
							turn = 2;
							v[2].show();
							//printf("len2=%d\n", len2);
							ptr2 += (len2);
							len2 = 0;
						}
					}
				}
				else{
					while(size2-- && ptr2[len2++]!='\n');
					if(size2 >= 0){
						v[2].set(ptr2, len2);
						if(v[1] < v[2]){
							turn = 1;
							//v[1].show();
							//printf("len1=%d\n", len1);
							ptr1 += (len1);
							len1 = 0;
						}
						else{
							turn = 2;
							//v[2].show();
							//printf("len2=%d\n", len2);
							ptr2 += (len2);
							len2 = 0;
						}
					}
				}
				//printf("turn=%d\n",turn);
				//printf("size1=%d\n", size1);
				//printf("size2=%d\n", size2);
			}
		}
		
		printf("\n");
	}
	*/
	return 0;
}
