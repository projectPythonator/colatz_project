/* title  :: colatz cycle finding
 * author :: agis daniels
 * date   :: September 25 2016
 * my docs
 * this program is for running through numbers in the colatz conjecture
 * it checks for cycles that are greater then there n in starting rangedefines the implementation of class MyClass<T>
 * it uses an avl tree 
 * we use the number 2billion to minimize the amount of inserts we do
 * in truth we are more worried about throwing our selves into a cycle
 * asap then how fast we relize we are in a cycle  other then the 
 * trival 4,2,1,4,2,1...
 * my method is n=3*n+1 then right shift n till its odd again
 * works in cunjution with the library gmp 6.1.1
 * get from here https://gmplib.org/
 * compilied with clang++ -O2 -lgmp -lm -std=gnu++11 colbit.cpp -ocolatz
 */

#include <algorithm>
#include <chrono>
#include <fstream>
#include <vector>
#include <functional>
#include <iostream>
#include <stdio.h>
#include <string>
#include <utility>
#include <gmp.h>

using namespace std;

typedef const int CstInt;

typedef chrono::high_resolution_clock Clock;

typedef pair<string, string> sPair;

typedef vector<sPair> pVec;

typedef vector<string> sVec;

pVec three_seen(1, make_pair("3", "3"));

mpz_t TWO_POW, ONE, THREE, SIX, tmpA1, tmpA2, tmpB1, tmpB2, tmpC1, tmpC2, tmpD1, tmpD2, tmpD3, tmpE1, tmpE2;

CstInt STRBASE=62;

bool myComp(const sPair &a, const sPair &b){
    mpz_set_str(tmpA1, a.second.c_str(), STRBASE);
    mpz_set_str(tmpA2, b.first.c_str() , STRBASE);
    return (mpz_cmp(tmpA1, tmpA2)<0);
}

void fixThrees(){
    int indA=1, indB=0;
    CstInt lim=three_seen.size();
    sPair a;
    a.first =three_seen[0].first ;
    a.second=three_seen[0].second;
    pVec tVec(lim, a);
    while(true){
        if(indA>=lim){
            a.second=three_seen[lim-1].second;
            tVec[indB]=a;
            ++indB;
            break;
        }else{
            if(three_seen[indA].first==a.second){
                a.second=three_seen[indA].second;
            }else{
                mpz_set_str(tmpB1, three_seen[indA].first.c_str(), STRBASE);
                mpz_set_str(tmpB2, a.second.c_str()             , STRBASE);
                mpz_sub    (tmpB1, tmpB1, SIX);
                if(0==mpz_cmp(tmpB1, tmpB2)){
                    a.second=three_seen[indA].second;
                }else{
                    tVec[indB]=a;
                    ++indB;
                    a.first =three_seen[indA].first ;
                    a.second=three_seen[indA].second;
                }
            }
            ++indA;
        }
    }
    for(int i=lim-indB; i>0; --i){
        tVec.pop_back();
    }
    swap(tVec, three_seen);
}

void moveToCorrect(CstInt a){
    for(int i=three_seen.size()-1; i!=a; --i){
        swap(three_seen[i], three_seen[i-1]);
    }
}

CstInt inRangePair(mpz_t a, mpz_t b, mpz_t c){
    mpz_add(tmpC1, c, SIX);
    mpz_sub(tmpC2, c, SIX);
    if((mpz_cmp(a, tmpC1)>0)||(mpz_cmp(b, tmpC2)<0)){
        return 0;
    }else if((mpz_cmp(a, c)<=0)&&(mpz_cmp(b, c)>=0)){
        return 3;
    }else if(mpz_cmp(b, tmpC2)==0){
        return 1;
    }else{
        return 2;
    }
}

void addTo(const sVec &in){
    sPair tmpP;
    for(auto it=in.begin(); it!=in.end(); ++it){
        tmpP.first =*it;
        tmpP.second=*it;
        auto up=upper_bound(three_seen.begin(), three_seen.end(), tmpP, myComp);
        CstInt a=up-three_seen.begin()-1;
        mpz_set_str(tmpD1, three_seen[a].first.c_str(),  STRBASE);
        mpz_set_str(tmpD2, three_seen[a].second.c_str(), STRBASE);
        mpz_set_str(tmpD3, it->c_str(), STRBASE);
        CstInt b=inRangePair(tmpD1, tmpD2, tmpD3);
        if(0==b){
            three_seen.push_back(tmpP);
            moveToCorrect(a+1);
            fixThrees();
        }else if(1==b){
            three_seen[a].second=*it;
            if((a+1)<three_seen.size()){
                mpz_set_str(tmpD2, three_seen[a].second.c_str() , STRBASE);
                mpz_set_str(tmpD3, three_seen[a+1].first.c_str(), STRBASE);
                if(inRangePair(tmpD1, tmpD2, tmpD3)){
                    fixThrees();
                }
            }
        }else if(2==b){
            three_seen[a].first=*it;
            if(0<a){
                mpz_set_str(tmpD1, three_seen[a].first.c_str(), STRBASE);
                mpz_set_str(tmpD3, three_seen[a-1].second.c_str(), STRBASE);
                if(inRangePair(tmpD1, tmpD2, tmpD3)){
                    fixThrees();
                }
            }
        }
    }
}

sVec getVecSet(mpz_t in){
    int indA=0, indB=0;
    int c=mpz_fdiv_ui(in, 3);
    sVec a(400, " ");
    sVec b(200, " ");
    if(2==c){
        mpz_add   (tmpE1, in, in);
        mpz_sub   (tmpE2, tmpE1, ONE);
        mpz_cdiv_q(tmpE2, tmpE2, THREE);
        for(int i=0; i<500; ++i){
            c=mpz_fdiv_ui(tmpE2, 3);
            const char* d=mpz_get_str(NULL, STRBASE, tmpE2);
            string umar(d);
            delete d;
            if(c==0){
                b[indB]=umar;
                ++indB;
            }else{
                a[indA]=umar;
                ++indA;
            }
            mpz_mul_2exp(tmpE1, tmpE1, 2);
            mpz_sub     (tmpE2, tmpE1, ONE);
            mpz_cdiv_q  (tmpE2, tmpE2, THREE);
            if(mpz_cmp(tmpE1, TWO_POW)>=0){
                break;

            }
        }
    }else{
        mpz_mul_2exp(tmpE1, in, 2);
        mpz_sub     (tmpE2, tmpE1, ONE);
        mpz_cdiv_q  (tmpE2, tmpE2, THREE);
        for(int i=0; i<500; ++i){
            c=mpz_fdiv_ui(tmpE2, 3);
            const char* d=mpz_get_str(NULL, STRBASE, tmpE2);
            string umar(d);
            delete d;
            if(c==0){
                b[indB]=umar;
                ++indB;
            }else{
                a[indA]=umar;
                ++indA;
            }
            mpz_mul_2exp(tmpE1, tmpE1, 2);
            mpz_sub     (tmpE2, tmpE1, ONE);
            mpz_cdiv_q  (tmpE2, tmpE2, THREE);
            if(mpz_cmp(tmpE1, TWO_POW)>=0){
                break;
            }
        }

    }
    for(int i=400-indA; i>0; --i){
        a.pop_back();
    }
    for(int i=200-indB; i>0; --i){
        b.pop_back();
    }
    addTo(b);
    return a;
}


bool recFunc(mpz_t in){
    mpz_t tmp;
    mpz_init_set_si(tmp, 1);
    mpz_add(tmp, in, in);
    if(mpz_cmp(in, ONE)<=0){
        return true;
    }else if(mpz_cmp(tmp, TWO_POW)>=0){
        return true;
    }else{
        sVec a=getVecSet(in);
        for(auto it=a.begin(); it!=a.end(); ++it){
            mpz_set_str(tmp, it->c_str(), STRBASE);
            cout<<three_seen.size()<<endl;
            return recFunc(tmp);
        }
    }
}

int main() {
    mpz_init_set_si (TWO_POW, 1);
    mpz_init_set_si (ONE, 1);
    mpz_init_set_si (THREE, 3);
    mpz_init_set_si (SIX, 6);
    mpz_init_set_si (tmpA1, 1);
    mpz_init_set_si (tmpA2, 1);
    mpz_init_set_si (tmpB1, 1);
    mpz_init_set_si (tmpB2, 1);
    mpz_init_set_si (tmpC1, 1);
    mpz_init_set_si (tmpC2, 1);
    mpz_init_set_si (tmpD1, 1);
    mpz_init_set_si (tmpD2, 1);
    mpz_init_set_si (tmpD3, 1);
    mpz_init_set_si (tmpE1, 1);
    mpz_init_set_si (tmpE2, 1);
    mpz_mul_2exp    (TWO_POW, TWO_POW, 1000);
    mpz_t start_num;
    mpz_init_set_si (start_num, 5);
    //i like to keep time
    auto t1 = Clock::now();
    recFunc(start_num);
    cout<<three_seen.size()<<endl;
    auto t2 = Clock::now();
    cout << "Delta t2-t1: " 
         << chrono::duration_cast<chrono::microseconds>(t2 - t1).count()
         << " micro seconds."
         << endl;
    return 0;
}
