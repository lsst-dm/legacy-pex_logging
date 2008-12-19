#! /usr/bin/env python
#
# routines for testing logging output 
#
# The wrapped test program writes output to the screen.  This module tests
# the output by redirecting it to a file and comparing with canonical
# results.  Note that when the underlying test program is changed, the file
# containing the canonical results usually must change as well.  

import sys, os
import subprocess 

def captureOutput(prog, outfile):
    """execute the given program and append the output to the given file.
    """
    out = open(outfile, 'w')
    test = subprocess.Popen(prog, bufsize=1, close_fds=True, stderr=out)
    excode = test.wait()
    out.close()

    if excode:
        print >> sys.stdout, prog, "exited with status", excode
    return excode == 0

def compareOutput(test, canon, showdiff=True):
    """return True if the two files have the same contents; False, otherwise.
    @param test       the output file to write results to
    @param canon      a file containing the "correct" results
    @param showdiff   if True (the default), print differences to stdout
    """
    diffout = None
    if showdiff:  diffout = sys.stdout
    excode = subprocess.call(["diff", test, canon],
                             stdout=sys.stdout, stderr=sys.stderr)
    return (excode == 0)

def checkOutput(prog, outfile, canon, showdiff=True):
    """run the test program and compare its (text) output to the canonical 
    results found in a given file.  If the generated output is the same,
    return True; otherwise, return False.  Any differences are written to
    standard out when showdiff=True.
    @param prog       the program to run
    @param outfile    the output file to write results to
    @param canon      a file containing the "correct" results
    @param showdiff   if True (the default), print differences to stdout
    """
    if os.path.exists(outfile):
        os.remove(outfile)
    okay = captureOutput(prog, outfile)
    same = False
    if os.path.exists(outfile):
        same = compareOutput(outfile, canon, showdiff)

    return okay and same

def test(prog, canon_suffix="_correct.txt", test_suffix="_results.txt"):
    outdir = os.path.dirname(sys.argv[0])
    if len(outdir) > 0:  prog = os.path.join(outdir, prog)
    outfile = prog + test_suffix
    canon = prog + canon_suffix
    
    return checkOutput(prog, outfile, canon)


