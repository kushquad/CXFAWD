class OBJreader:
    def __init__(self, filename):
        self.vertices = []
        self.faces = []
        self.filename = filename
        self.parseOBJ()
        
    def parseOBJ(self):
        file = open(self.filename,"r")
        for line in file:
            if line[0]=='v' and line[1]==' ':
                comps = line.strip().split(' ')
                self.vertices.append([float(comps[1]),float(comps[2]),float(comps[3])])
            elif line[0]=='f':
                comps = line.strip().split(' ')
                vertinds = [int(faceatt.split('/')[0]) for faceatt in comps[1:]]
                l = len(vertinds)
                for i in xrange(l-2):
                    self.faces.append([vertinds[i+1]-1,vertinds[i+2]-1,vertinds[0]-1])
        file.close()
