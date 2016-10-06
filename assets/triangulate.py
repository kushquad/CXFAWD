import sys

filename = sys.argv[1]
file = open(filename)
file2 = open(filename[:-4]+"_new"+".obj","w")

for line in file:
    if not line.startswith('f'):
        file2.write(line)
    else:
        facecoords = line.strip().split(' ')[1:]
        vertinds = [int(faceatt.split('/')[0]) for faceatt in facecoords]
        l = len(vertinds)
        for i in xrange(l-2):
            file2.write('f '+" ".join([str(vertinds[i+1]),str(vertinds[i+2]),str(vertinds[0])])+'\n')
file.close()
file2.close()
