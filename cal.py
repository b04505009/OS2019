import sys
import subprocess

program = sys.argv[1]
input = sys.argv[2]

subprocess.Popen(program + ' < '+ input)
sunprocess.Popen('dmesg > dmesg.txt')

content = []
with open('dmesg.txt', 'r+') as f:
	for line in f:
		sp = line.split()
		content.append(float(sp[4]-sp[3]))

print(content)
