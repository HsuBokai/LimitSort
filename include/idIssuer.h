#ifndef _ID_ISSUER_H_
#define _ID_ISSUER_H_

class IdIssuer {
public:
	IdIssuer();
	IdIssuer& operator ++();
	//IdIssuer& operator --();
	IdIssuer& operator = (const IdIssuer& other);
	operator const char* () const ;
private:
	char _id[4+1];
};

#endif
