import os
files = os.listdir('data')
NODE = []
EDGE = []

for f in files:
    fp = open('data\\'+f,'r')
    d = fp.readlines()
    for l in d:
        if('TCP' in l or 'ESTABLISHED' in l):
            line = l.split()
            src = line[1].split(':')
            dst = line[2].split(':')
            if(len(src) == 2 and len(dst) == 2):
                if(src[0] not in ('0.0.0.0','127.0.0.1') and dst[0] not in ('0.0.0.0','127.0.0.1')):
                    src = src[0]
                    proto = dst[1]
                    dst = dst[0]
                    internal = False
                    if((dst.split('.')[0] == '10') or ((dst.split('.')[0] == '192') and (dst.split('.')[1] == '168')) or ((dst.split('.')[0] == '172') and (int(dst.split('.')[1]) >= 16) and (int(dst.split('.')[1]) <= 32))):
                         internal = True
                    print src,dst,proto,internal
                    if(src not in NODE):
                        NODE.append(src)
                    
                    if(dst not in NODE):
                        NODE.append(dst)
                    EDGE.append([src,dst,proto,internal])
fp = open('NODE.csv','w')
fp.write('Id,Label,Internal\r\n')
for N in NODE:
    try:
        internal = False
        
        if((N.split('.')[0] == '10') or ((N.split('.')[0] == '192') and (N.split('.')[1] == '168')) or ((N.split('.')[0] == '172') and (int(N.split('.')[1]) >= 16) and (int(N.split('.')[1]) <= 32))):
            internal = True
        fp.write(N+','+N+','+str(internal)+'\r\n')
    except:
        pass
fp.close()
fp = open('EDGE.csv','w')
fp.write('Source,Target,Protocol,Internal,Label,Type,Weight\r\n')
for E in EDGE:
    fp.write(E[0]+','+E[1]+','+E[2]+','+str(E[3])+','+E[2]+',Direct,1.0\r\n')
fp.close()
