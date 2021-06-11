#include <iostream>
    using std::cout;
    using std::endl;
#include <NTL/ZZ.h>
    using NTL::ZZ;
    using NTL::conv;
#include <string>
    using std::string;

ZZ naiveExponentiation(ZZ a, ZZ p, ZZ n);
ZZ mod(ZZ a, ZZ b);

int main(){
    ZZ a = conv <ZZ>("17612114");

    ZZ p = conv<ZZ>("31501436");
    ZZ n = conv<ZZ>("28211478");
    cout<< naiveExponentiation(a,p,n);
    return 0;
}

ZZ naiveExponentiation(ZZ a, ZZ p, ZZ n){
    ZZ r (1);
    for(ZZ i (0); i < p; i++)
        r = mod((r*a),n);
    return r;
}
ZZ mod(ZZ a, ZZ b){
    ZZ q= a/b;
	ZZ	r= a- (q*b);
	if(a<ZZ(0)){
		ZZ ar=r;
	    r= b+ar;
	}
	return r;
}
//12699
