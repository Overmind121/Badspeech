# Badspeech
This is a program that filters out "badspeak" words from a text file. This utilizes a bloomfilter and a hashtable made up of binary search trees and bit vectors.
## Building
In order to build the program you need to run this command:

$ make clean
$ make all


## Running
To run this program:

### To generate keys
$./banhammer [-hs] [-t size] [-f size] < your input txt file

OPTIONS
*  -h          Program usage and help.
*  -s          Program program statistics.
*  -t size     Specify hash table size (default: 2^16).
*  -f size     Specify Bloom filter size (default: 2^20).


## Bugs and Errors
There are no bugs or errros that you should be worried about.
