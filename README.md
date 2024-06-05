# μOrCS

This is a veeeeeeeery minimalistic version of OrCS[1], the ORdinary Computer
Simulator, which is based on SiNUCA[2].

Professor Marco Zanata, PhD, removed this code from the original OrCS source and
my friend Gabriel G. de Brito took the liberty of refactoring a little and calling
“μOrCS” (i.e. “Micro OrCS”).

In this release, I'm working on implementing a Branch Target Buffer and a gshare/bimodal Branch Predictor.
uOrCS is trace driven, so by entering the main name of the memory, dynamic and static trace files contained in the folder, it is able to read the files and generate a report at the end about the branches and accuracy rate of the implemented methods.

## Refs

[1] - https://github.com/mazalves/OrCS
[2] - https://bitbucket.org/mazalves/sinuca/src/master/
