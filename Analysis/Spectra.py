import sys
import os
import argparse
from natsort import natsorted
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
CLI.add_argument(
        '-e',
        '--energy',
        nargs=1,
        type=str,
        default=["1MeV"]
        )
CLI.add_argument(
        '-p',
        '--particles',
        nargs="*",
        type=str,
        default=['protons', 'deuterons', 'tritons', 'alphas', 'beryllium nuclei', 'boron nuclei', 'carbon nuclei', 'nitrogen nuclei', 'oxygen nuclei', 'electrons', 'positrons']
        )
CLI.add_argument(
        '-path',
        '--path',
        nargs=1,
        type=str,
        default='./'
        )
#CLI.add_argument(
#        '-t',
#        '--title',
#        nargs=1,
#        type=str,
#        default='Spectra_Comparison_Plot'
#        )
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

protons   = []
deuterons = []
tritons   = []
alphas    = []
beryls    = []
borons    = []
carbons   = []
nitrogens = []
oxygens   = []
electrons = []
positrons = []
gammas    = []
particle = [protons, deuterons, tritons, alphas, beryls, borons, carbons, nitrogens, oxygens, electrons, positrons, gammas]
particleName = ['protons',  'deuterons', 'tritons', 'alphas', 'beryllium nuclei', 'boron nuclei', 'carbon nuclei', 'nitrogen nuclei', 'oxygen nuclei', 'electrons',  'positrons', 'gammas']
particleNamesForFile = ['protons', 'deuterons', 'tritons', 'alphas', 'berylliumns', 'borons', 'carbons', 'nitrogens', 'oxygens', 'electrons', 'positrons']

partIDs = []
for i in args.particles:
    partID = particleName.index(i)
    partIDs.append(partID)

newParticle = []
newParticleName = []
for i in partIDs:
    newParticle.append(particle[i])
    newParticleName.append(particleName[i])

#particle = newParticle
#particleName = newParticleName


#Choose which series to loop through based on the scorer id
histMult = 12*(scorer - 1)

energy = args.energy[0]
scorer_files = []
for i in partIDs:
    hist = histMult + i
    scorer_files.append('_'+energy+'_h1_'+str(hist)+'.csv')

# Conver list to a tuple so it can be used with .endswith
scorer_files = tuple(scorer_files)

# Search the given directory for the csv files corresponding to the chosen scorer
path = args.path[0]
files = [f for f in os.listdir(path) if f.endswith(scorer_files)]
# Sort them by histogram id
files = natsorted(files)


#particle.pop(nPart)
#particle.pop(4) # no alphas at 1 MeV
#particleName.pop(nPart)
#particleName.pop(4)
#particleNamesForFile.pop(4)
particleNumber = []
maxNum = []

#files.pop(4)

# Read in data files entered during function call
for file in files:
    with open(path+file, 'r') as f:

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
    maxNum.append(max(entries))
    particleNumber.append(total)
    #if total > 0:
    #    entries = [i/total for i in entries]

    index = files.index(file)
    newParticle[index].append(entries)

    data = []

# Set up edges of each histogram
vMin   = 1
vMax   = 1e+7
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
#binValues = defaultdict(list)

#if ('electrons' in newParticleName):
#    elIndex = newParticleName.index('electrons')
#    noElPartNames = newParticleName[:elIndex] + newParticleName[elIndex+1:]
#
#    for i in noElPartNames:
#        binValues[i].append(0)
#        index = newParticleName.index(i)
#
#        for j in range(len(edges) -1):
#            binWidth = edges[j+1]-edges[j]
#            spectraValue = newParticle[index][0][j] / (binWidth * 1e+3)
#            binValue = newParticle[index][0][j]
#            binValues[i].append(binValue)
#
#    binValues['electrons'].append(0)
#    under1M = [k0 for k0 in edges if k0 < 1e+6]
#    for k1 in range(len(under1M) - 1):
#        binWidth = edges[k1+1]-edges[k1]
#        #spectraValue = newParticle[elIndex][0][k1] / (binWidth * 1e+3 )
#        #spectraValues[elIndex].append(spectraValue)
#        binValue = newParticle[elIndex][0][k1]
#        binValues['electrons'].append(binValue)
#
#else:
#    for i in partIDs:
#        binValues[i].append(0)
#
#        for j in range(len(edges) -1):
#            binWidth = edges[j+1]-edges[j]
#            #spectraValue = newParticle[i][0][j] / (binWidth * 1e+3)
#            binValue = newParticle[i][0][j]
#            binValues[i].append(binValue)
#for i in partIDs:
#    binValues[i].append(0)
#
#    for j in range(len(edges) -1):
#        binWidth = edges[j+1]-edges[j]
#        #spectraValue = newParticle[i][0][j] / (binWidth * 1e+3)
#        binValue = newParticle[i][0][j]
#        binValues[i].append(binValue)


# Determine the relative proportion of each particle
totalParticles = sum(particleNumber)
#relProp = [i/totalParticles for i in particleNumber]


#for i in range(len(particle)):
#
#    # Fill in the rest of the values. Note that there was 1 more edge than value because of the left
#    # and right boundaries.
#    for j in range(len(edges)-1):
#        binWidth = edges[j+1]-edges[j]
#        binValue = particle[i][0][j] * (binWidth / 1e+3) # spectra in 1/keV
#        #binValue = particle[i][0][j]
#        binValues[i].append(binValue)

# Values are assigned to the rightmost edge of each bin, so need to remove the first edge
#del edges[0]

#particleProbd = defaultdict(list)
#for i in relProp:
#    part = binValues[relProp.index(i)]
#    particleList = []
#    for j in part:
#        particleList.append(i * j)
#    particleProbd[relProp.index(i)].append(particleList)
#
maxPart = max(maxNum)
#relProp = [i/maxPart for i in particleNumber]
particleProb = []
#for i in relProp:
#    part = newParticle[relProp.index(i)][0]
#    particleList = []
#    for j in part:
#        particleList.append(i * j)
    #particleProb.append(particleList)

for i in newParticle:
    part = i[0]
    particleList = []
    for j in part:
        particleList.append(j / maxPart)
    particleProb.append(particleList)
#
#probTotal = [sum(i) for i in particleProb]
#totalProb = sum(probTotal)
#
#relProp = [round(i, 3) for i in relProp]

# Energy histograms require units of MeV - switch to this from eV. Note that now we need the full
# length of the edges list
#edgesForFile = [i*1e-6 for i in edges]
#edgesForeFile = [i*1e-6 for i in edges[:len(under1M)]]
#edgesForFile = [round(i, 5) for i in edgesForFile]

# Geant4 requires the command '/gps/hist/point' before each energy histogram energy. Create these
# lists
#command = ['/gps/hist/point']*len(edgesForFile)
#ecommand = ['/gps/hist/point']*len(edgesForeFile)

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


#sepPath = path.rsplit('/',maxsplit=2)
#date = sepPath[1]
#histoDirName = sepPath[0]+'/Histos/'+date+'/'
#os.makedirs(histoDirName, exist_ok=True)
#
#for i in binWeights:
#    if i != 'electrons':
#        histo = np.vstack((command, edgesForFile, binWeights[i])).T
#        np.savetxt(histoDirName+particleNamesForFile[particleName.index(i)]+'_scorer'+str(scorer)+'_energy_histogram_'+energy.replace('_', '')+'.dat',
#                histo, delimiter='\t', fmt='%s')
#    else:
#        histo = np.vstack((ecommand, edgesForeFile, binWeights[i])).T
#        np.savetxt(histoDirName+particleNamesForFile[particleName.index(i)]+'_scorer'+str(scorer)+'_energy_histogram_'+energy.replace('_', '')+'.dat',
#                histo, delimiter='\t', fmt='%s')
#....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#for i in binValues:
#    binValues[i] = [j/1e+3 for j in binValues[i]]

fig = plt.figure(figsize=(9,6))

#edgesForPlot = [i*1e-3 for i in edges[:-1]]
edgesForPlot = []
for i in range(len(edges)-1):
    midway = np.sqrt(edges[i]*edges[i+1]) * 1e-6
    edgesForPlot.append(midway)

for i in range(len(newParticle)):
    #plt.semilogx(edgesForPlot, particleProbd[i][0], label=particleName[i]+' ('+str(relProp[i])+')', marker='+', markersize=1, linewidth=1)
    #plt.semilogx(edgesForPlot, particleProb[i], label=newParticleName[i]+' ('+str(relProp[i])+')', marker='+', markersize=1, linewidth=1)
    plt.semilogx(edgesForPlot, particleProb[i], label=newParticleName[i], marker='+', markersize=1,
            linewidth=1.5)

sns.despine() # Removes the top and right borders (if using dark, white, or ticks)

font = {'family':'serif', 'serif': ['computer modern roman']}
plt.rc('font', **font)
#plt.title('Secondary charged particle spectra produced by '+energy.replace('_', ' ')+' neutrons at '+depth+' depth in an ICRU sphere')
plt.xlabel('Energy (MeV)', fontsize=20)#, fontsize=16)
#plt.ylabel('Relative probability [1/keV]')#, fontsize=16)
plt.ylabel('Normalized energy spectrum', fontsize=20)#, fontsize=16)
plt.xlim(7e-7,15)
plt.ylim(0,1)
#lgd = plt.legend(bbox_to_anchor=(1.04,1), borderaxespad=0)
#plt.legend(loc='upper right', bbox_to_anchor=(0.95, 1.)) # Scorer 1, 2019-01-21
#plt.legend(loc='center left', bbox_to_anchor=(0., 0.415)) # Scorer 2, 2019-01-21
#plt.legend(loc='upper center', bbox_to_anchor=(0.66,1.), fontsize=18)

plt.minorticks_on()
plt.tick_params(axis='both', which='major', labelsize=18)
# ax = plt.axes()

# ratio = 1.5
# xvals,yvals = ax.axes.get_xlim(),ax.axes.get_ylim()
# xrange = np.log(xvals[1])-np.log(xvals[0])
# yrange = np.log(yvals[1])-np.log(yvals[0])
# ax.set_aspect(ratio*(xrange/yrange), adjustable='box')

#plt.minorticks_on()
#plt.tick_params(axis='both', which='major', labelsize=14)

#plt.tight_layout()
#plt.show()
# #
paths = path.rsplit('/',maxsplit=2)
root = paths[0]
date = paths[1]
dirName = '/home/clund/Documents/McGill/MScThesis/Thesis/Figures/Final/'
#print(dirName)
os.makedirs(dirName, exist_ok=True)
fileName = 'Scorer'+str(scorer)+'_'+energy.replace('_', '')+'_'+date+'_Final'
#fileName = 'Electrons_'+str(scorer)+'_'+energy.replace('_','')
plt.savefig(dirName+fileName+'.eps', 
        format='eps', 
        dpi=800,
        bbox_inches='tight',
        transparent=False)
#        #pad_inches=0)
plt.savefig(dirName+fileName+'.png',
        format='png',
        dpi=1000,
        bbox_inches='tight'
        #bbox_extra_artists=(lgd,),
        #bbox_inches='tight'
        #pad_inches=1
        )

################################################################################
# Old code

# Electrons will be handled in a unique way so we must ignore them for now
#for i in range(len(particle)-1):
#    # Set the value of the first histogram bins to 0 - it is unused by geant4
#    binValues[i].append(0)
#
#    # Fill in the rest of the values. Note that there was 1 more edge than value because of the left
#    # and right boundaries.
#    for j in range(len(edges) - 1):
#        binWidth = edges[j+1]-edges[j]
#        spectraValue = particle[i][0][j] / (binWidth * 1e+3) # spectra in 1/keV
#        binValue = particle[i][0][j]
#        binValues[i].append(binValue)
#
#
## Get the index of the electrons, then create a new dictionary entry with the first binValue set to 0
## as before. Next, determine all edges where the rightmost edge is less than 1 MeV and add all but
## the last to the electron binValue dictionary entry
#elIndex = particle.index(electrons)
#binValues[elIndex].append(0)
#under1M = [i for i in edges if i < 1e+6]
#nunder1M = len(under1M)
#for i in range(nunder1M - 2):
#    #spectraValue = particle[elIndex][0][i] / ( (edges[i+1] - edges[i]) * 1e+3 )
#    #spectraValues[elIndex].append(spectraValue)
#    binValue = particle[elIndex][0][i]
#    binValues[elIndex].append(binValue)
#
## Combine all bin entries with a rightmost edge greater than or equal to 1 MeV, as well as the value
## for the largest bin with an edge below 1 MeV. Append these to the dictionary entry
#above1M = 0
#for i in range(len(edges)-(nunder1M - 1)):
#    above1M += particle[elIndex][0][i+(nunder1M-2)]
#binValues[elIndex].append(above1M)
#
#    
