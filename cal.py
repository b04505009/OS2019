content = []

unit_t = 0.0019799022452 
unit_t = 0.002040561485290527

with open("cal.txt",'r+') as f:
	for line in f:
		if line.strip() == '':
			continue
		arr = line.strip().split(' ')
		print(arr)
		pass_t = float(arr[3])-float(arr[2])
		arr.append(' '+str(pass_t)+' ')
		arr.append(' '+str( int(pass_t// unit_t))+' ')
		

		content.append(("".join([ ('|'+i) for i in arr])).strip())

# print("| name | pid  | start time           | end time             | pass time     | pass unit time |")
# print("| ---- | ---- | -------------------- | -------------------- | ------------- | -------------- |")
for i in content:
	print(i)
