import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib import animation
import argparse

def lims(xview, yview, winwidth = None):
	xmax = xview.max().max()
	xmin = xview.min().min()

	ymax = yview.max().max()
	ymin = yview.min().min()

	d = winwidth or np.max([xmax-xmin, ymax - ymin])

	xlim = [(xmax + xmin)/2 - 1.1 * d/2, (xmax + xmin)/2 + 1.1 * d/2]
	ylim = [(ymax + ymin)/2 - 1.1 * d/2, (ymax + ymin)/2 + 1.1 * d/2]

	return xlim, ylim

def main():
	parser = argparse.ArgumentParser(description='Visualizes output of simulation with pandas and matplotlib.')
	parser.add_argument('--animstep', type=int, default = 1, action = 'store', dest= "animstep", help='Step per frame of animation')
	parser.add_argument('--winwidth', type=float, default = None, action = 'store', dest= "winwidth", help='Window Width')
	parser.add_argument('--outfile', default = None, action = 'store', dest="outfile")
	parser.add_argument("csvfile")
	args = parser.parse_args()

	df = pd.read_csv(args.csvfile, index_col=[0,1])

	xview = df['x']
	yview = df['y']

	xlim, ylim = lims(xview,yview, args.winwidth)

	fig = plt.figure(figsize=(6,6))

	def animate(nframe):
		n = nframe * int(args.animstep)
		x = xview.ix[n]
		y = yview.ix[n]
	
		plt.cla()
		ax = plt.gca()
		ax.patch.set_facecolor('black')

		plt.xlim(xlim[0],xlim[1])
		plt.ylim(ylim[0],ylim[1])
		plt.scatter(x,y, s= 1, color='yellow')
		plt.title('Frame %d' % nframe)
	


	nframes = len(df.index.levels[0])

	anim = animation.FuncAnimation(fig, animate, frames=nframes/args.animstep)

	if args.outfile:
		anim.save('demoanimation.gif', writer='imagemagick', fps=4);

	plt.show()

if __name__ == '__main__':
	main()