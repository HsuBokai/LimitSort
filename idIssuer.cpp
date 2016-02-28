#include "idIssuer.h"

IdIssuer::IdIssuer(){ 
	for(int i=0; i<4; ++i) _id[i] = 'a';
	_id[4] = '\0';
}

IdIssuer& IdIssuer::operator ++(){
	for(int i=0; i<4; ++i){
		if(_id[i]=='z') _id[i] = 'a';
		else {
			++_id[i];
			break;
		}
	}
	return (*this);
}
/*
IdIssuer& IdIssuer::operator --(){
	for(int i=0; i<4; ++i){
		if(_id[i]=='a') _id[i] = 'z';
		else {
			--_id[i];
			break;
		}
	}
	return (*this);
}
*/
IdIssuer& IdIssuer::operator = (const IdIssuer& other) {
	for(int i=0; i<4; ++i) _id[i] = other._id[i];
	return (*this);
}

IdIssuer::operator const char* () const { return _id; }

