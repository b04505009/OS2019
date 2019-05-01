l = []

with open("unit.txt",'r+')as f:
	for line in f:
		if line.strip() == '':
			continue
		sp = line.split()
		l.append(float(sp[4])-float(sp[3]))


unit_t = sum(l)/len(l)/5000
print(unit_t)
