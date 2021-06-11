#include <NTL/ZZ.h>
#include <vector>
#include <iostream>
#include <bitset>
using namespace std;
using namespace NTL;

ZZ mod(ZZ a, ZZ b){
	ZZ q= a/b;
	ZZ	r= a- (q*b);
	if(a<ZZ(0)){
		ZZ ar=r;
	    r= b+ar;
	}
	return r;
}

ZZ d(0),k(0),l(0);
vector <ZZ>  eu_extendido(ZZ a, ZZ b){
  vector<ZZ> res = {d,k, l};
  if (b==ZZ(0))  return res = {a,ZZ(1),ZZ(0)};
  ZZ q = mod(a,b);
  ZZ r = (a-q)/b;
  res = eu_extendido(b, q);
  d=res[0];
  k=res[1];
  l=res[2];
  return res = {d,l,k-l*r};
}

ZZ eu_binario(ZZ a, ZZ b ){
    ZZ g(1) ,x=abs(a),y=abs(b), t;
    while(mod(x,ZZ(2))==0 and mod(y,ZZ(2))==0){
	    x=x/2;
	    y=y/2;
	    g=2*g;
    }
    while(x!=ZZ(0)){
        if( mod(x,ZZ(2))==ZZ(0)) x=x/2;
        else if (mod(y,ZZ(2))==0 ) y=y/2;
        else {
            t=abs(x-y)/2;
            if (x>=y) x=t;
            else y=t;
        }
    }
    return (g*y);
}

ZZ expModularNTL(ZZ a, ZZ p, ZZ n){
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

bool test_primalidad100(ZZ n){
    if (n==ZZ(1)or n==ZZ(0) ) return 0;
	ZZ lista[4]={ZZ(2),ZZ(3),ZZ(5),ZZ(7)};
	for (ZZ *i=lista; i<lista+4; i++){
	    if (mod(n,*i )==ZZ(0) and n!=*i ) {return 0;	}
	}
	return 1;
}

bool prime(ZZ n){
	ZZ n2= SqrRoot (n);
	for (ZZ i=ZZ(0); i<n2;i++ ){
		if (i<=ZZ(100) and test_primalidad100(i)) {
            if (mod(n,i)==ZZ(0)) return 0;
        }
        else if(i>ZZ(100) and prime(i) and mod(n,i)==ZZ(0)) return 0;
	}
	return 1;
}
class expon{
    public: ZZ base; int ex;
    expon(ZZ b){
       base= b; ex=0;
    }
    void sum(){
        ex++;
    }
};
vector <expon> prop3(ZZ n ){
    vector <expon> divisor;
    ZZ copy_n=n;
    for (ZZ i(2); i<copy_n; i++){
        if (  (i<=ZZ(100) and test_primalidad100(i)) or (i>ZZ(100) and prime(i))  ){
            ZZ aux= mod(n,i);
            if (aux ==(0)) {
                expon E1(i);
                while (  aux ==(0)  ){
                    E1.sum();
                    n= n/i;
                    aux= mod(n,i);
                }
                divisor.push_back(E1);
            }
        }
        if  (n==ZZ(1)) break;
    }
    return divisor;
}
ZZ prop4(expon n){ //p*(e) -  p*(e-1)
    return ( power(n.base, n.ex ) - power(n.base , n.ex-1 ));
}
ZZ fi_euler(ZZ n){
    if (n==ZZ(1)) return ZZ(0);
    else if ( (n<=ZZ(100) and test_primalidad100(n)) or (n>ZZ(100) and prime(n)) ) return ZZ(n-1);
    else{//buscamos sus multiplos primos
        vector <expon> aux= prop3(n);
        ZZ count (0), rpta(1);
        //comprobar si los multiplos son unicos o estan elevados a algun num
        for (int i=0; i< aux.size(); i++) if (aux[i].ex == 1 ) count++;
        //todos son multiplos unicos. llamamos prop 2 o 1
        if (count == aux.size()){
            for (int i=0; i< aux.size(); i++)rpta*=fi_euler(aux[i].base);
        }
        //multiplos repetidos osea a**n. llamamos pop 4.
        else{
            for (int i=0; i< aux.size(); i++) rpta*= prop4(aux[i]);
        }
        return rpta;
    }
}

ZZ exponenciacion_euler(ZZ base, ZZ ex, ZZ m ){
    ZZ phi= fi_euler(m);
    if (phi==ex) return ZZ(1);
    else return expModularNTL(base,ex-phi,m);
}
ZZ inversa_euler(ZZ base, ZZ m){
    return expModularNTL(base,fi_euler(m)-1, m);
}
// (= es congruente) en la forma: ax + b = c (mod n)
ZZ ecuacion_mod_k0(ZZ a, ZZ b, ZZ c, ZZ n){
    if (eu_binario(a,n)==ZZ(1)){
        c= mod(c-b,n);
        c= mod(c*inversa_euler(a,n),n);
        return c ;
	}
}
bool solucion( vector<ZZ> p){
    for (int i=0; i< p.size()-1; i++){
        if (eu_binario( p[i],p[i+1])!= ZZ(1)) return 0;
    }
    return 1;
}

vector<ZZ> resto_chino(vector<ZZ> a, vector<ZZ> p){
	vector<ZZ> vect;
	//ver si tiene solucion
    if (solucion(p) ){
    	ZZ Pn=ZZ(1), x0=ZZ(0);
    	vector<ZZ> P,q;
    	//hallamos Pn "Zn"
    	for (int j=0; j<p.size();j++){  Pn *= p[j];  }

    	for (int j=0; j<p.size();j++){
    	//llenamos P con Pi
    	    P.push_back(Pn/p[j]);
		//llenamos qi
		    q.push_back(ecuacion_mod_k0 ( P[j],ZZ(0),ZZ(1),p[j] ) );
		    x0 +=  (mod(a[j]*P[j]*q[j] , Pn));
		    if (x0>=Pn) x0=mod(x0,Pn);
		}
		vect.push_back(x0);
		vect.push_back(Pn);
	    return vect;
	     //devuelve vect= x0,Pn para x= x0 + (Pn*k) para todo k
	}
}


int main(){
    //resto chino euler
    /*ZZ p1 = conv<ZZ>("20922061633289321786662463851237792159969511713677949291457953131831940867053995697993299791874274572430543539318666952578424854733297116388801744370775406705838560524197895233981992541457630408151063336841783428390698041867453768161173566332713308022930488832464744450626914348851749489131804939991360333226653032538154041958398624164730129728515077824248988990834822017731359434220814947626710709796293754001615013753722891674017240852617513932338863528103544426760649772293394697063289782824592953978290083541268998261805533197865496230075385598820942153912161846986005157975391390128968249179630059093522405068301");
    ZZ p2 = conv<ZZ>("28064516982937666554730774250912841038391456248329886095601299563792400118236115651225951285389613103984830644940142901104451585035059221169808630828486522965787543732459607425572758009215104179771342134080834171835920264601912106553443973145208904328988106789889218345982246299966088587901951028872746315469118592632142866963216373637221914598309248734776141416091772424708268266239123387244614470910996152865626854080226881102471498429111717507765463765779509653273095283784066110001112271651977339818793799842625924305191874358139742183692346521760807927926991318874563909786921912111561914047641344508051594742500");
    ZZ p3 = conv<ZZ>("23049699217264565666512031579791167494289174875643782639076646115872690487769557999161972696680122252634067541424289194486923780219918933629959120503306414751309949325422370849486163557358229045818231930391222554388897050280323736092926960495461700622418338450758725249322132116410556059292735555518037751467106712666387984523042859530424251304349996520813319107672615644432894935948857252728325219260142576210450121384296085096185143813450597596415766132120262985749834645697007556501384444120184470771786793402387514804348722262409804035157597201998516022191298697769603245972832839166436555960720111910220397102051");

    ZZ a1 = conv<ZZ>("16945039801157226162015673644514057699187743833414520765362580259144789665801788470601756664409373332447074379985097425932754398804048802355917602891755025588362844557532946185091361591120811321833155725130213366434617043414116940973048270542758838990699526146428315859695743618154884002039491926900547030784530079719990261322087325115429638545221911684963734152357897767949094939647644027885454027621264044360277942961204955954869782344348170633657836349318893159257294908618486115042579620416812666280574124312605928118605744268009610091107281810245881387879473874890080539550096952388422141212291068923641998023979");
    ZZ a2 = conv<ZZ>("28064516982937666554730774250912841038391456248329886095601299563792400118236115651225951285389613103984830644940142901104451585035059221169808630828486522965787543732459607425572758009215104179771342134080834171835920264601912106553443973145208904328988106789889218345982246299966088587901951028872746315469118592632142866963216373637221914598309248734776141416091772424708268266239123387244614470910996152865626854080226881102471498429111717507765463765779509653273095283784066110001112271651977339818793799842625924305191874358139742183692346521760807927926991318874563909786921912111561914047641344508051594742500");
    ZZ a3 = conv<ZZ>("23049699217264565666512031579791167494289174875643782639076646115872690487769557999161972696680122252634067541424289194486923780219918933629959120503306414751309949325422370849486163557358229045818231930391222554388897050280323736092926960495461700622418338450758725249322132116410556059292735555518037751467106712666387984523042859530424251304349996520813319107672615644432894935948857252728325219260142576210450121384296085096185143813450597596415766132120262985749834645697007556501384444120184470771786793402387514804348722262409804035157597201998516022191298697769603245972832839166436555960720111910220397102051");

    vector<ZZ> p{p1,p2,p3};
    vector<ZZ> a{a1,a2,a3};
    vector<ZZ> rpta=resto_chino(a,p);
    cout << rpta[0]<<" " <<rpta[1]<<endl;
    */

    vector<ZZ> p{ZZ(3),ZZ(5),ZZ(7)};
    vector<ZZ> a{ZZ(2),ZZ(3),ZZ(2)};
    vector<ZZ> rpta=resto_chino(a,p);
    cout << rpta[0]<<" " <<rpta[1]<<endl;

}
