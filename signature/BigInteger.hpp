#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cmath>
using namespace std;
 
const int maxn = 1000;

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
        int d[maxn], len;
    	void clean() { while(len > 1 && !d[len-1]) len--; }
        bign() 			{ memset(d, 0, sizeof(d)); len = 1; }
        bign(int num) 	{ *this = num; } 
    	bign(char* num) { *this = num; }
        bign operator = (const char* num){
            memset(d, 0, sizeof(d)); len = strlen(num);
            for(int i = 0; i < len; i++) d[i] = num[len-1-i] - '0';
            clean();
    		return *this;
        }
        bign operator = (int num){
            char s[20]; sprintf(s, "%d", num);
            *this = s;
    		return *this;
        }
     
        bign operator + (const bign& b){
            bign c = *this; int i;
            for (i = 0; i < b.len; i++){
            	c.d[i] += b.d[i];
            	if (c.d[i] > 9) c.d[i]%=10, c.d[i+1]++;
    		}
    		while (c.d[i] > 9) c.d[i++]%=10, c.d[i]++;
    		c.len = max(len, b.len);
    		if (c.d[i] && c.len <= i) c.len = i+1;
            return c;
        }
        bign operator - (const bign& b){
            bign c = *this; int i;
            for (i = 0; i < b.len; i++){
            	c.d[i] -= b.d[i];
            	if (c.d[i] < 0) c.d[i]+=10, c.d[i+1]--;
    		}
    		while (c.d[i] < 0) c.d[i++]+=10, c.d[i]--;
    		c.clean();
    		return c;
        }
        bign operator * (const bign& b) const {
            bign c; c.len = len + b.len + 1;
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
        	int i, j;
    		bign c = *this, a = 0;
        	for (i = len - 1; i >= 0; i--)
        	{
        		a = a*10 + d[i];
        		for (j = 0; j < 10; j++) if (a < b * (j + 1)) break;
        		c.d[i] = j;
        		a = a - b * j;
        	}
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
    out << x.str();
    return out;
}
