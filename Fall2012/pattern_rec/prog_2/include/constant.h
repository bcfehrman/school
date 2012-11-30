////////////////////////////////////////////////////////////////////
//          Constants for Backprop Font Data                      //
////////////////////////////////////////////////////////////////////

#define  FV_IN       "data/norm.mix"
#define  NORMALIZE    false
#define  FONT_DATA    true

#define  ALPHA         0.0     // momentum factor
#define  ETA           0.1    // learning rate
#define  MAXIN         14      // number of input nodes
#define  MAXH          20      // number of nodes hidden layer
#define  MAXOUT        26      // number of nodes output layer
#define  N_ITERATIONS  1       // times to train the network
#define  NUMFV         78      // number of feature vectors total
#define  NUMUV         78      // number of test vectors
#define  LOG_MOD       10      // print results every LOG_MOD iterations

#define  RESULTS_OUT_NAME   "data/bp.out"
#define  WEIGHTS_IN         "data/bpw.in"
#define  WEIGHTS_OUT        "data/bpw.out"
#define  WEIGHT_FILE_NAME   "data/genw.out"  // for genweights
