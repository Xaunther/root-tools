#As April 2020, LCG and lxplus are quite broken. For the time being, the issue can be overcome by manually appending to LD_LIBRARY_PATH the necessary paths to the missing libraries.
#GSL
LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/cvmfs/lhcb.cern.ch/lib/lcg/releases/GSL/2.5-32fc5/x86_64-centos7-gcc9-opt/lib
#Davix
LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/cvmfs/lhcb.cern.ch/lib/lcg/releases/Davix/0.7.3-d94fa/x86_64-centos7-gcc9-opt/lib64
#tbb
LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/cvmfs/lhcb.cern.ch/lib/lcg/releases/tbb/2020_U1-bbcac/x86_64-centos7-gcc9-opt/lib
#vdt
LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/cvmfs/lhcb.cern.ch/lib/lcg/releases/vdt/0.4.3-992df/x86_64-centos7-gcc9-opt/lib
#gcc
LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/cvmfs/lhcb.cern.ch/lib/lcg/releases/gcc/9.2.0-afc57/x86_64-centos7/lib64
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH
