#include <iostream>
#include <vector>
#include <NTL/ZZ.h>
using namespace std;
using namespace NTL;

ZZ mod(ZZ a, ZZ b){
	ZZ r= a%b;
	if(a<0){
		ZZ ar=r;
	    r= b+ar;
	}
	return r;
}

ZZ Binary_Exponentiation( ZZ a, ZZ e, ZZ n ) {
    ZZ result (1);
    while( e != ZZ(0)) {
        if( mod(e,ZZ(2)) == ZZ(1))
            result  = mod(result*a,n);
        e >>= 1;
        a = mod(a*a,n);
    }
    return result;
}
int main() {
    ZZ a = conv <ZZ>("188");

    ZZ p = conv<ZZ>("2");
    ZZ n = conv<ZZ>("221");

    ZZ bb = Binary_Exponentiation(a,p,n);

cout << bb<<endl;
}
