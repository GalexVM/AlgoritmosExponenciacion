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

ZZ modular_exponentationmm( ZZ a, ZZ n, ZZ m ) {
    ZZ result (1);

    while( n != ZZ(0)) {

        if( mod(n,ZZ(2)) == ZZ(1))
            result  = mod(result*a,m);
        n >>= 2;

        a = mod(a*a,m);
    }
    return result;
}
int main() {
    ZZ a = conv <ZZ>("176120583692541215223756019569734453133");

    ZZ p = conv<ZZ>("315087067920665239075112866970441951150");
    ZZ n = conv<ZZ>("282920299886359916964241039137997310129");

    ZZ bb = modular_exponentationmm(a,p,n);

cout << bb<<endl;
}
