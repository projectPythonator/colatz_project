
seen=[]
seen.append([3,3])
pow2=2**1000


def bin_search(a):
    l=0
    u=len(seen)
    while l<u:
        b=l+(u-l)//2
        v=seen[b][1]
        if a==v:
            return b 
        elif a>v:
            if l==b:
                break
            l=b
        elif a<v:
            u=b
    return l+1

def fix_seen():
    global seen
    lim=len(seen)
    b=[seen[0][0],seen[0][1]]
    a=[b for i in range(lim)]
    t1=0
    t2=0
    ind=1
    indB=0
    while(True):
        if ind>=lim:
            b[1]=seen[lim-1][1]
            a[indB]=b
            indB+=1
            break
        else:
            c=seen[ind][0]
            d=seen[ind][1]
            if c==b[1]:
                b[1]=d
            else:
                iA=c-6
                if iA==b[1]:
                    b[1]=d
                else:
                    a[indB]=b
                    indB+=1
                    b[0]=c
                    b[1]=d
            ind+=1
    seen=a

def in_range_pair(a, b, c):
    if a>(c+6) or b<(c-6):
        return 0
    elif a<=c and b>=c:
        return 3
    elif b==(c-6):
        return 1
    else:
        return 2

def add_to(a):
    global seen
    iA=1
    iB=1
    iC=1
    b=[9,9]
    for i in a:
        b[0]=i
        b[1]=i
        pos=bin_search(i)-1
        iA=seen[pos][0]
        iB=seen[pos][1]
        iC=i
        c=in_range_pair(iA, iB, iC)
        if 0==c:
            seen.insert(pos+1, b)
            fix_seen()
        elif 1==c:
            seen[pos][1]=i
            if((pos+1)<len(seen)):
                iB=seen[pos][1]
                iC=seen[pos+1][0]
                if 0!=in_range_pair(iA, iB, iC):
                    fix_seen()
        elif 2==b:
            seen[pos][0]=i
            if pos>0:
                iA=seen[pos][0]
                iC=seen[pos-1][1]
                if 0!=in_range_pair(iA, iB, iC):
                    fix_seen()

def make_set(a):
    iA=0
    iB=0
    rA=[0 for i in range(400)]
    rB=[0 for i in range(200)]
    if 2==(a%3):
        iC=a+a
        iD=(iC-1)//3
        for i in range(500):
            if 0==(iD%3):
                rB[iB]=iD
                iB+=1
            else:
                rA[iA]=iD
                iA+=1
            iC+=iC+iC+iC
            iD=(iC-1)//3
            if iC>=pow2:
                break
    else:
        iC=a+a+a+a
        iD=(iC-1)//3
        for i in range(500):
            if 0==(iD%3):
                rB[iB]=iD
                iB+=1
            else:
                rA[iA]=iD
                iA+=1
            iC+=iC+iC+iC
            iD=(iC-1)//3
            if iC>pow2:
                break
    while iB<200:
        rB.pop()
        iB+=1
    while iA<400:
        rA.pop()
        iA+=1
    add_to(rB)
    rA.reverse()
    return rA


def rev_Func(a):
    if 1>=a:
        return True
    elif (a+a)>=pow2:
        return True
    elif 0==(a%3):
        b=[a]
        add_to(b)
        return True
    else:
        b=make_set(a)
        while len(b)>0:
            c=b.pop()
            print(c)
            rev_Func(c)
        return True

def main():
    lim=2**62
    i=4
    while i<pow2:
        print(i)
        s=i-1
        if 0==(s%3):
            s//=3
            rev_Func(s)
        i+=i
    print(len(seen))

if __name__=='__main__':
    main()
