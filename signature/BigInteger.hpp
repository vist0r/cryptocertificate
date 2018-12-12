#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cmath>
using namespace std;
 
const int maxn = 5000;

#define PI acos(-1.0)

class complex{
  public :
      double real,image;
      complex(double r=0.0,double i=0.0)
      {
        this->real=r;
        this->image=i;
      }
      friend complex operator +(const complex a, const complex b)
      {
        return complex(a.real+b.real,a.image+b.image);
      }
      friend complex operator -(const complex a, const complex b)
      {
        return complex(a.real-b.real,a.image-b.image);
      }
      friend complex operator *(const complex a, const complex b)
      {
        return complex(a.real*b.real-a.image*b.image,a.real*b.image+b.real*a.image);

      }
};
class bign{
    public:
        int d[maxn], len, isNegative;
    	void clean() { while(len > 1 && !d[len-1]) len--; }
        bign() 			{ memset(d, 0, sizeof(d)); len = 1; }
        bign(int num) 	{ *this = num; } 
    	bign(char* num) { *this = num; }
        bign operator = (const char* num){
            memset(d, 0, sizeof(d)); len = strlen(num);
            int start = (num[0] == '-');
            isNegative = (num[0] == '-'); 
            for(int i = start; i < len; i++) d[i - start] = num[len-1-i + start] - '0';
            clean();
    		return *this;
        }

        void print(){
            for (int i = 0; i < len; i++) cout << d[i] ;
            cout << endl;
        }
        bool is_even() const {
            return !(d[0] & 1);
        }

        bign operator = (int num){
            char s[20]; sprintf(s, "%d", num);
            *this = s;
    		return *this;
        }

        bign padd(bign a,bign b,int flag)const{
            int i; a.isNegative = flag;
            for (i = 0; i < b.len; i++){
                a.d[i] += b.d[i];
                if (a.d[i] > 9) a.d[i]%=10, a.d[i+1]++;
            }
            while (a.d[i] > 9) a.d[i++]%=10, a.d[i]++;
            a.len = max(len, b.len);
            if (a.d[i] && a.len <= i) a.len = i+1;
            return a;
        }

        bign SwitchAdd(bign a, bign b)const{
             if ((!a.isNegative) && (!b.isNegative)) return padd(a,b,0);
             if (a.isNegative && (!b.isNegative)) {
                return SwitchSub(b, a * bign(-1));
             }
             if ((!a.isNegative) && b.isNegative){
                return SwitchSub(a,b * bign(-1));
             }
             return SwitchSub(b  * bign(-1),a);
        }
     
        bign operator + (const bign& b){
            bign c = *this;  
            return SwitchAdd(c,b);;
        }

        bign psub(bign a, bign b,int isN)const{
            a.isNegative = isN; int i;
            for (i = 0; i < b.len; i++){
                a.d[i] -= b.d[i];
                if (a.d[i] < 0) a.d[i]+=10, a.d[i+1]--;
            }
            while (a.d[i] < 0) a.d[i++]+=10, a.d[i]--;
            a.clean();
            return a;
        }

        bign SwitchSub(bign a, bign b)const{
            if (a.isNegative && b.isNegative)
            {
                return SwitchSub(b * bign(-1),a * bign(-1));
            }
            if (!a.isNegative && b.isNegative){
                 return b * bign(-1) + a;
            }
            if (a.isNegative && !b.isNegative){
                return padd(a * bign(-1),b,1);
            }
            if (a < b){
                return psub(b,a,1);
            }
            return psub(a,b,0);
        }
        bign operator - (const bign& b){
            bign c = *this; int i;
    		return SwitchSub(c,b);;
        }
        bign operator * (const bign& b) const {
            bign c = *this; c.len = len + b.len + 1;
            if (c.isNegative && b.isNegative) c.isNegative = 0;
            else if ((!c.isNegative) &&  (!b.isNegative)) c.isNegative = 0;
            else c.isNegative = 1;
            int l = 1;
            int l1 = b.len;
            int l2 = len;
            while (l < l1 * 2|| l < l2 * 2) l *= 2;  
            complex x1[maxn],x2[maxn],x[maxn];
            for (int i = 0; i < l1; i++)
            {
                x1[i].real = b.d[i];
                x1[i].image = 0.0;
            }
            for (int i = l1;i < l; i++) x1[i].real = x1[i].image = 0.0;
            for (int i = 0;i < l2; i++)
            {
                x2[i].real = d[i];
                x2[i].image = 0.0;
            }
            for (int i = l2; i < l; i++) x2[i].real = x2[i].image=0.0;
            fft(x1,l,1);
            fft(x2,l,1);
            for (int i = 0;i < l; i++) x1[i] = x1[i] * x2[i];
            fft(x1,l,-1);   
            for (int i = 0; i < l; i++) c.d[i] = x1[i].real + 0.5;
            for (int i = 0; i < l; i++)
            {
               c.d[i + 1] +=c.d[i] / 10;
               c.d[i] %= 10;
            }
            c.clean();
    		return c;
        }
        bign operator / (const bign& b){
        	int i, j,flag;
    		bign c = *this, a = 0;          
            if (c.isNegative && b.isNegative) flag = 0;
            else if ((!c.isNegative) &&  (!b.isNegative)) flag = 0;
            else flag = 1;
            for (i = len - 1; i >= 0; i--)
            {
                a = a * 10 + d[i];
                for (j = 0; j < 10; j++) if (a < b * (j + 1)) break;
                c.d[i] = j;
                a = a - b * j;
            }
            c.isNegative = flag;
        	c.clean();
        	return c;
        }
        bign operator % (const bign& b){
        	int i, j;
    		bign a = 0;
        	for (i = len - 1; i >= 0; i--)
        	{
        		a = a*10 + d[i];
        		for (j = 0; j < 10; j++) if (a < b*(j+1)) break;
        		a = a - b*j;
        	}
            int flag = 1;
            if (isNegative) a.isNegative = 1;
            else a.isNegative = 0;
        	return a;
        }
    	bign operator += (const bign& b){
            *this = *this + b;
            return *this;
        }
     
        bool operator <(const bign& b) const{
            if(len != b.len) return len < b.len;
            for(int i = len-1; i >= 0; i--)
                if(d[i] != b.d[i]) return d[i] < b.d[i];
            return false;
        }
        bool operator >(const bign& b) const{return b < *this;}
        bool operator<=(const bign& b) const{return !(b < *this);}
        bool operator>=(const bign& b) const{return !(*this < b);}
        bool operator!=(const bign& b) const{return b < *this || *this < b;}
        bool operator==(const bign& b) const{return !(b < *this) && !(b > *this);}
     
        string str() const{
            char s[maxn]={};
            for(int i = 0; i < len; i++) s[len-1-i] = d[i]+'0';
            return s;
        }
    private :
        void change(complex *A,int len) const 
        {
          int j=len/2;
          for (int i=1;i<len-1;i++)
          {
            if (i<j) swap(A[i],A[j]);
            int k=len/2;
            while (j>=k)
            {
              j-=k;
              k/=2;
            }
            if (j<k) j+=k;
          }
        }
        void fft(complex *A,int len,int on) const 
        {
          change(A,len);
          for (int i=2;i<=len;i=i*2)
          {
            complex wn(cos(on*2*PI/i),sin(-on*2*PI/i));
            for (int j=0;j<len;j+=i)
            {
              complex w(1,0);
              for (int k=j;k<j+i/2;k++)
              {
                complex u=A[k];
                complex v=w*A[k+i/2];
                A[k]=u+v;
                A[k+i/2]=u-v;
                w=w*wn;
              }
            }
          }
          if(on==-1) for (int i=0;i<len;i++) A[i].real/=len;
        }
};
istream& operator >> (istream& in, bign& x)
{
    string s;
    in >> s;
    x = s.c_str();
    return in;
}
 
ostream& operator << (ostream& out, const bign& x)
{
    string flag;
    flag.clear();
    if (x.isNegative) flag = "-";
    out << flag + x.str();
    return out;
}
namespace bigInteger{
    bign Zero(0); bign One(1);
    bign Two(2); bign Ten(10);
    bign pow_n(bign a, bign b){
        bign ans(1);
        while (b != Zero){
            if (!b.is_even()) ans = ans * a;
            a = a * a;
            b = b / Two;
        }
        return ans;
    }

    bign gcd_n(bign a, bign b){
        if (b  != Zero){
            return gcd_n(b, a % b);
        }
        return a;
    }

    void exgcd(bign a, bign b, bign &x, bign &y){
        if  (b != Zero) {
            x = One;
            y = Zero;
        }
        exgcd(b,a % b, y, x);
        x = x  -  (a / b) * y;
    }
    bign getInv_n(bign a,bign p){
        if (gcd_n(a,p) != bign(1)){
            return -1;    
        }

    }
}