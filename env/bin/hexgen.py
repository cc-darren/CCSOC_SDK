#!/usr/bin/python

import sys, getopt
import binascii

def usage():
    print ("%s -i <inputfile> -o <outputfile>" %sys.argv[0])

def create_hex(dest, src):
    rfile = open(src,'rb')
    wfile = open(dest,'w')
    while True:
        data = rfile.read(4)
        #print(binascii.hexlify(data))
        if not data:
            break
        wfile.write(binascii.hexlify(data[3]))
        wfile.write(binascii.hexlify(data[2]))
        wfile.write(binascii.hexlify(data[1]))
        wfile.write(binascii.hexlify(data[0]))
        wfile.write('\n')
    rfile.close()
    wfile.close()

def main(argv):
    inputfile = ''
    outputfile = ''
    try:
        opts, args = getopt.getopt(argv,"hi:o:",["help","ifile=","ofile="])
    except getopt.GetoptError:
        usage()
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            usage()
            sys.exit()
        elif opt in ("-i", "--ifile"):
            inputfile = arg
        elif opt in ("-o", "--ofile"):
            outputfile = arg

    if inputfile != '' and outputfile != '':
        create_hex(outputfile, inputfile)
        print "Generate HEX file is done..."
    else:
        usage()

    sys.exit()

if __name__ == "__main__":
    if len(sys.argv) <= 1:
        usage()
    else:
        main(sys.argv[1:])