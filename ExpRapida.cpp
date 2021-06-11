#include <iostream>
#include <vector>
#include <NTL/ZZ.h>
using namespace std;
using namespace NTL;
ZZ expModularNTL(ZZ a, ZZ p, ZZ n);
ZZ mod(ZZ a, ZZ b);
int main(){
    ZZ a =conv<ZZ>("176120583692541215223756019569734453133");
    ZZ p =conv<ZZ>("315087067920665239075112866970441951150");
    ZZ n =conv<ZZ>("282920299886359916964241039137997310129");
    cout<<expModularNTL(a,p,n);
}

ZZ expModularNTL(ZZ a, ZZ p, ZZ n)
{

   ZZ t;
   if (p == ZZ(0)) return ZZ(1);
   if (mod(p,ZZ(2)) == ZZ(0))
   {
     t = expModularNTL(a, p/2, n);
     return mod(t*t,n);
   }
   t = expModularNTL(a, (p-1)/2, n);

   return mod(a*mod(t*t,n),n);
}

ZZ mod(ZZ a, ZZ b){
    ZZ q= a/b;
    ZZ r= a-(q*b);

    if (a < ZZ(0)){
        ZZ aux = r;
        r = b+aux;
    }

    return r;

}
