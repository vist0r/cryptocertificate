#include "BigInteger.hpp"

class Curve{
	public :
		bign p,a,b,G,n,h;
		Curve(bign p = 0,bign a = 0,bign b = 0,bign G = 0,bign n = 0,bign h = 0){
			p = p;
			a = a;
			b = b;
			G = G;
			n = n;
			h = h;
		}

};

class point_c : Curve{
public:
	bign _x,_y;
	
	point_c(bign _x = 0, bign _y = 0, bign p = 0){
		this->_x = _x;
		this->_y = _y;
		this->p = p;
	}

	bool equal_c(const point_c &a ,const point_c &b) const{
		return (a.p == b.p) && (a.a == b.a)  && (a.b == b.b) && (a.G == b.G) && (a.n == b.n) && (a.h == b.h); 
	}
	bool equal_p(const point_c &a ,const point_c &b) const{
		return (a.p == b.p) && (a.a == b.a)  && (a.b == b.b) && (a.G == b.G) && (a.n == b.n) && (a.h == b.h) && (a._x == b._x) && (a._y == b._y);
	}
	point_c operator + (const point_c &b) const {
		point_c c = *this;
		point_c ans = *this;
		assert(equal_c(b,c));
		bign k;
		if (!equal_p(b,c)){
			bign zi = (c._y - b._y);
			bign mu = (c._x - b._x);
			bign g = BigInteger::gcd_n(zi,mu);
			zi = zi / g;
			mu = mu / g;
			bign inv = BigInteger::getInv_n((mu % p+ p) % p,p);
			k = zi * inv % p;
			ans._x = ((k * k % p - c._x - b._x) % p + p) % p ;
			ans._y = ((k * (c._x - ans._x) - c._y) % p + p) % p;  
		}else {
			bign zi = (bign(3) * (c._x * c._y) % p + p + a) % p;
			bign mu = (bign(2) * c._y % p + p) % p;
			bign g = BigInteger::gcd_n(zi,mu);
			zi = zi / g;
			mu = mu / g;
			bign inv = BigInteger::getInv_n(mu % p + p,p);
			k = zi * inv % p;
			ans._x = ((k * k % p - c._x - b._x) % p + p) % p ;
			ans._y = ((k * (c._x - ans._x) - c._y) % p + p) % p; 
		}
		return ans;
	}

};