import sys
import os
import argparse
#from natsort import natsorted
import csv
import itertools
import numpy as np
from collections import defaultdict
from decimal import Decimal, ROUND_HALF_UP
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import seaborn as sns

################################################################################
################################################################################

CLI = argparse.ArgumentParser()

CLI.add_argument(
        '-s', 
        '--scorer',
        nargs=1,
        type=int,
        default=[1])
args = CLI.parse_args()


################################################################################

# Choose the number of particles to plot (order unchanged)
#nPart = 11

# Choose scorer (1, 2, or 3) and set the corresponding depth (or plot titles)
scorer = int(args.scorer[0])
if scorer == 1:
    depth = '15 cm'
elif scorer == 2:
    depth = '7.5 cm'
elif scorer == 3:
    depth = '1.5 cm'


#data_files = glob.glob(path + '/' + 'Clustered*.csv')

# Start empty lists to be filled by the input cross-section data
data = []

#particle = newParticle
#particleName = newParticleName

# Choose which electron histogram to open
hist = 12*(scorer - 1) + 9

path = '/home/clund/geant4/work/Data/xRaySpectra/'
scorerFile = path+'xRay_h1_'+str(hist)+'.csv'


# Read in data files entered during function call
with open(scorerFile, 'r') as f: 
    # Split the single iterable output of csv.reader into multiple iterables
    rows, columns = itertools.tee(
        csv.reader(f, delimiter=',', skipinitialspace=True))

    # Make sure you're starting from the beginning of the file
    f.seek(0)

    # Skip the 7 headers and the first bin
    for i in range(8):
        next(f)

    # Create ntuple containing (Ek, elastic, capture, total cs) info
    for line in f:
        thisData = line.split(',')
        data.append(thisData)

    # Make sure itertools doesn't hold rows in memory
    del rows


# Try to convert the tuples into lists of numbers
try:
    entries, weightedEntries, weightSqEntries, mean, meanSq = zip(*data)

    entries = [int(i) for i in entries]

except:
    raise ValueError('This file contains non-numerical data and/or blank spaces')

total = sum(entries)
maxNum = max(entries)
if total > 0:
    entries = [i/maxNum for i in entries]

data = []

# Set up edges of each histogram
vMin   = 1
vMax   = 1e+6
nBins  = 502  # actual bins is n+2, where n is the chosen number

dlog = (np.log10(vMax) - np.log10(vMin))/nBins
dv = 10**dlog

edges = []
edge = vMin
while len(edges) <= nBins:
    edges.append(edge)
    edge *= dv

# The first bin value is ignored
del edges[0]

# Record bin values and output an ASCII-file histogram (for track generation) for each particle
binValues = []

binValues.append(0)

for i in range(len(edges) -1):
    binWidth = edges[i+1]-edges[i]
    spectraValue = entries[i] / (binWidth * 1e+6)
    binValue = entries[i]
    binValues.append(binValue)

#particleProb = []
#for i in relProp:
#    part = newParticle[relProp.index(i)][0]
#    particleList = []
#    for j in part:
#        particleList.append(i * j)
#    particleProb.append(particleList)
#
#probTotal = [sum(i) for i in particleProb]
#totalProb = sum(probTotal)
#
#relProp = [round(i, 3) for i in relProp]

# Energy histograms require units of MeV - switch to this from eV. Note that now we need the full
# length of the edges list
edgesForFile = [i*1e-6 for i in edges]
#edgesForFile = [round(i, 5) for i in edgesForFile]

# Geant4 requires the command '/gps/hist/point' before each energy histogram energy. Create these
# lists
command = ['/gps/hist/point']*len(edgesForFile)

#binTotal = dict()
#for i in binValues:
#    binTotal[i] = sum(binValues[i])
#
#binWeights = defaultdict(list)
#for i in binValues:
#    bins = binValues[i]
#    if binTotal[i] > 0:
#        for j in bins:
#            binWeights[i].append(j/binTotal[i])
#    else:
#        for j in bins:
#            binWeights[i].append(j)


histoDirName = path+'Histos/'
os.makedirs(histoDirName, exist_ok=True)

histo = np.vstack((command, edgesForFile, binValues)).T
np.savetxt(histoDirName+'xRay_scorer'+str(scorer)+'_energy_histogram.dat',
        histo, delimiter='\t', fmt='%s')

#....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

binValues = [i/1e+6 for i in binValues]

fig = plt.figure(figsize=(9,6))

#ax = fig.add_subplot(111)

# Set a colormap that samples over some range as many times as there are energies
# colors = cm.rainbow(np.linspace(0, 1, 3))

#sns.set() # Use seaborn defaults rather than matplotlib

# Choose one of 5 themes (darkgrid (default), whitegrid, dark, white, ticks)
# A parameter dictionary can also be passed as a second argument
#sns.set_style('white', {'xtick.bottom': True, 'ytick.left': True}) 

# Scales size/aspect ratio of plot. Paper, notebook (default), talk or poster are the options
#sns.set_context('notebook', rc={'axes.labelsize': 14, 'xtick.labelsize': 12, 'ytick.labelsize': 12, 'legend.fontsize': 12}) 

#sns.set_palette('husl', 1)
#sns.set_palette('cubehelix', len(energy))
#sns.set_palette('deep', len(energy))

#edgesForPlot = [i*1e-3 for i in edges[:-1]]
edgesForPlot = []
for i in range(len(edges)-1):
    midway = np.sqrt(edges[i]*edges[i+1]) * 1e-6
    edgesForPlot.append(midway)

plt.semilogx(edgesForPlot, entries, marker='o', markersize=1, linewidth=2.5)

sns.despine() # Removes the top and right borders (if using dark, white, or ticks)

#plt.rc('text', usetex=True)
font = {'family':'serif', 'serif': ['computer modern roman']}
plt.rc('font', **font)
#plt.title('Secondary electron spectra produced by 250 kV x-rays at '+depth+' depth in an ICRU sphere')
plt.xlabel('Energy (MeV)', fontsize=20)
#plt.ylabel('Relative probability [1/keV]')#, fontsize=16)
plt.ylabel('Normalized energy spectrum', fontsize=20)# '+r'$\left( \frac{N(E)}{\sum_E N(E)} \right)$', fontsize=18)
#lgd = plt.legend(bbox_to_anchor=(1.04,1), borderaxespad=0)
#plt.legend(loc='upper right', bbox_to_anchor=(0.95, 1.)) # Scorer 1, 2019-01-21
#plt.legend(loc='center left', bbox_to_anchor=(0., 0.415)) # Scorer 2, 2019-01-21
#plt.legend()

plt.xlim(7e-7,1.5)
plt.ylim(0,1.05)

# ax = plt.axes()

# ratio = 1.5
# xvals,yvals = ax.axes.get_xlim(),ax.axes.get_ylim()
# xrange = np.log(xvals[1])-np.log(xvals[0])
# yrange = np.log(yvals[1])-np.log(yvals[0])
# ax.set_aspect(ratio*(xrange/yrange), adjustable='box')

plt.minorticks_on()
plt.tick_params(axis='both', which='major', labelsize=18)

plt.tight_layout()
#plt.show()
# #
#paths = path.rsplit('/',maxsplit=2)
#root = paths[0]
#date = paths[1]
#dirName = root+'/Plots/'+date+'/'
##print(dirName)
#os.makedirs(dirName, exist_ok=True)
fileName = 'xRay_scorer'+str(scorer)
plt.savefig(fileName+'.eps', 
        format='eps', 
        dpi=800,
        #bbox_inches='tight',
        transparent=False)
#        #pad_inches=0)
plt.savefig(fileName+'.png',
        format='png',
        dpi=600,
        bbox_inches='tight'
        #bbox_extra_artists=(lgd,),
        #bbox_inches='tight'
        #pad_inches=1
        )
