import sys
import csv
import argparse

def main():
	parser = argparse.ArgumentParser(description='Converts Princeton file format to csv.')
	parser.add_argument("princetonfile")
	args = parser.parse_args()

	meters_per_au = 149597871000
	kg_per_earthmass = 5.972e24
	seconds_per_day = 60*60*24

	with open(args.princetonfile, "r") as fd:
		contents = fd.read()

	contents = contents.split()

	nbodies = int(contents[0])

	print nbodies
	print 0.0

	for i in xrange(nbodies):
		x = float(contents[2 + i  * 8]) / meters_per_au
		y = float(contents[3 + i  * 8]) / meters_per_au
		vx = float(contents[4 + i  * 8]) * seconds_per_day / meters_per_au 
		vy = float(contents[5 + i  * 8]) * seconds_per_day / meters_per_au 
		m = float(contents[6 + i  * 8]) / kg_per_earthmass

		print x,y,vx,vy,m

if __name__ == '__main__':
	main()