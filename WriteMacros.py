import sys
import os
import re

################################################################################

def PNameForSource(x):
    if x == 'electron':
        x = 'e-'
    elif x == 'carbon':
        x = 'GenericIon'+'\n'+'/gps/ion 6 12'
    elif x == 'nitrogen':
        x = 'GenericIon'+'\n'+'/gps/ion 7 14'
    elif x == 'oxygen':
        x = 'GenericIon'+'\n'+'/gps/ion 8 16'
    else:
        x = x

    return x


def PNameForFS(x):
    if x == 'e-':
        x = 'Electrons'
    else:
        x = x.title()+'s'

    return x


def EKinForFS(x):
    x = x.replace('.', ',')

    return x
        

def EKinForSource(x):
    # need to add to functionality of 're' by searching for all possible number represenations
    numeric_const_pattern = '[-+]? (?: (?: \d* \. \d+ ) | (?: \d+ \.? ) ) (?: [Ee] [+-]? \d+ ) ?'

    rx = re.compile(numeric_const_pattern, re.VERBOSE)

    value = float(rx.findall(x)[0])

    # account for SI prefix
    if 'M' in x:
        mult = 1e+6
    elif 'k' in x:
        mult = 1e+3
    else:
        mult = 1

    # ensure it is a proper integer before converting back to a string
    return str(int(value * mult))


def IntOfNEvents(x):
    # reformat the string in case it is in scientific notation
    # the string literal:
        # converts x to a float - float(x), 
        # reformats it (as a string) - f'{}'
        # in standard notation (i.e. as a floating point) - :_f
        # with zero decimals - .0
    # basically str(int(float(x))) but with more control
    return f'{float(x):.0f}'


################################################################################

# ADJUST THESE IF YOU ARE NOT ME
basePath  = '/home/chris/geant4/work/Data/Spectra/'
pathToApp = '/home/chris/geant4/work/Spectra/build/spectra'

# open the template macro file and the submissionScript template
templateFile = open('template.mac')
submissionFile = open('template.sh')

# read them into strings, then close them
templateString = templateFile.read()
templateFile.close()

submissionString = submissionFile.read()
submissionFile.close()

if len(sys.argv) == 1:
    nThreads = input('Number of threads: ')

    pName    = input('Particle type: ')

    eKin     = input('Initial neutron kinetic energy (with units): ')

    nEvents  = IntOfNEvents(input('Number of primary neutrons per run: '))

    nRuns    = int(input('Number of runs: '))
    
    date     = input('Date the simulations were started: ')


elif len(sys.argv) == 7:
    nThreads = sys.argv[1]
    pName    = sys.argv[2]
    eKin     = sys.argv[3]
    nEvents  = IntOfNEvents(sys.argv[4])
    nRuns    = int(sys.argv[5])
    date     = sys.argv[6]

else:
    print('Provide, in order:'+'\n'
            +'the number of threads to be used,'+'\n'
            +'the name of the particle to be simulated,'+'\n'
            +'the initial kinetic energy of the particles,'+'\n'
            +'the number of primary particles per run,'+'\n'
            +'the number of runs,'+'\n'
            +'and the date'+'\n'
            +'Otherwise, provide no input and follow the prompts.')
    sys.exit()

################################################################################
# FILL IN MACRO TEMPLATES

# update particle name for use as a source and for use in folder/file names
pNameForSource = PNameForSource(pName)
pNameForFS     = PNameForFS(pName)

# do the same for eKin
eKinForSource = EKinForSource(eKin)
eKinForFS     = EKinForFS(eKin)

# create folder within basePath if it doesn't exist
folderName = basePath+pNameForFS
os.makedirs(folderName, exist_ok=True)

# create date sub-folder within the (potentially) new particle sub-directory
folderName = folderName+'/'+date
os.makedirs(folderName, exist_ok=True)

# set prefix for file names (to be used for both the macros and the output)
filePrefix = 'Spectra_'+pNameForFS+'_'+eKinForFS

# write a new macro file with the placeholder values in the template being replaced by the chosen values
for i in range(nRuns):
    # make a folder for each run
    runName   = 'run'+str(i+1)
    runFolder = folderName+'/'+runName
    os.makedirs(runFolder, exist_ok=True)

    # set the values to be used in the macro
    macroParams = {
           'nThreads'   : nThreads,
           'pName'      : pNameForSource,
           'eKin'       : eKinForSource,
           'folderName' : runFolder,
           'fileName'   : filePrefix,
           'nEvents'    : nEvents
           }

    # set the macro name and write it according to the above dictionary
    macroName = filePrefix+'_'+runName+'.mac'
    with open(macroName, 'w') as f:
        f.write(templateString.format(**macroParams))

    # set the values for replacement in the submission script and add it to the overall bash script
    bashParams = {
            'pathToApp' : pathToApp,
            'macroName' : macroName
            }

    bashName = filePrefix+'_'+runName+'.sh'

    with open(bashName, 'w') as f:
        f.write(submissionString.format(**bashParams))

    with open('pass-to-blade.sh', 'a') as f:
        f.write('sbatch -p long -N1 -n'+nThreads+' '+bashName+'\n')
 

#################################################################################
