#include <stdio.h>

#include "writer.h"
#include "rand.h"

int genLine(const Rand& rand, const Writer& writer, const int& maxColumn){
	unsigned char len;
	if(rand.get(&len)<0){
		fprintf(stderr, "Rand.get fail\n");
		return -1;
	}
	len = (len % maxColumn)+1;

	char* buf = new char[len+1];
	for(int i=0; i<len; ++i){
		unsigned char c;
		if(rand.get(&c)<0){
			fprintf(stderr, "Rand.get fail\n");
			delete [] buf;
			return -1;
		}
		buf[i] = c%26 + 'a';
	}
	buf[len] = '\n';
	
	if(writer.writeFile(buf, len+1)<0) {
		fprintf(stderr, "Writer.writeFile fail\n");
		delete [] buf;
		return -1;
	}
	delete [] buf;
	return 0;
}


int main(int argc, char* argv[]){
	if(argc < 4){
		fprintf(stderr, "Usage: %s < line # > < max column # > < output file >\n", argv[0]);
		return -1;
	}
	int ret;
	int line;
	ret = sscanf(argv[1], "%d", &line);
	if(ret != 1 || ret == EOF) {
		fprintf(stderr, "parse %s for < line # > fail\n", argv[1]);
		return -1;
	}
	int maxColumn;
	ret = sscanf(argv[2], "%d", &maxColumn);
	if(ret != 1 || ret == EOF) {
		fprintf(stderr, "parse %s for < max column # > fail\n", argv[2]);
		return -1;
	}

	fprintf(stdout, "line=%d, maxColumn=%d\n", line, maxColumn);
	
	Writer writer(argv[3]);
	Rand rand;


	for(int i=0; i<line; ++i) 
		if(genLine(rand, writer, maxColumn) < 0) {
			fprintf(stderr, "getLine fail\n");
			return -1;
		}
	return 0;
}
