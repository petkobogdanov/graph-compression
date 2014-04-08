DATADIR=/home/petko/Dropbox/slice_tree/src/experiment
SAMPLING=STBS
#EXHAUSTIVE="-x"
EXHAUSTIVE=""


#<<commentT
GRAPH=$DATADIR/TWITTER-CRAWL/twitter-crawl-udir.graph
DATA=$DATADIR/TWITTER-CRAWL/twitter-crawl-tags_log.data
INDEX=$DATADIR/TWITTER-CRAWL/twitter-crawl-udir.index
NAMES=$DATADIR/TWITTER-CRAWL/twitter-crawl-users-id-name-topht3
PARTITIONS=50
SAMPLES=15000
RHO=0.9
DELTA=0.1

TREENAME=${DATA}_${PARTITIONS}_alg-${SAMPLING}_rho-${RHO}_delta-${DELTA}_samples-${SAMPLES}$EXHAUSTIVE
date
echo ./graph_compression -i $EXHAUSTIVE -c $SAMPLING -p $PARTITIONS -n $SAMPLES -m 2 -r $RHO -d $DELTA -t 8 -g $GRAPH -v $DATA -s $INDEX
./graph_compression -i -c $SAMPLING -p $PARTITIONS -n $SAMPLES -m 2 -r $RHO -d $DELTA -t 8 -g $GRAPH -v $DATA -s $INDEX > $TREENAME.tree
./viztree.sh $TREENAME $NAMES
#commentT

#<<commentDBLP
GRAPH=$DATADIR/DBLP/dblp.graph
DATA=$DATADIR/DBLP/dblp_dm_net.data
INDEX=$DATADIR/DBLP/dblp.index
NAMES=$DATADIR/DBLP/authors-id-name-topconf3-names
PARTITIONS=50
SAMPLES=100000
RHO=0.9
DELTA=0.1

TREENAME=${DATA}_${PARTITIONS}_alg-${SAMPLING}_rho-${RHO}_delta-${DELTA}_samples-${SAMPLES}$EXHAUSTIVE
date
echo ./graph_compression -i $EXHAUSTIVE -c $SAMPLING -p $PARTITIONS -n $SAMPLES -m 2 -r $RHO -d $DELTA -t 8 -g $GRAPH -v $DATA -s $INDEX
./graph_compression -i -c $SAMPLING -p $PARTITIONS -n $SAMPLES -m 2 -r $RHO -d $DELTA -t 8 -g $GRAPH -v $DATA -s $INDEX > $TREENAME.tree
./viztree.sh $TREENAME $NAMES
#commentDBLP

<<commentSYN
NAMES=""
GRAPH=$DATADIR/SYN/syn1000.graph
DATA=$DATADIR/SYN/syn1000.data
INDEX=$DATADIR/SYN/syn1000.index
PARTITIONS=20
SAMPLES=100
RHO=0.9
DELTA=0.1

TREENAME=${DATA}_${PARTITIONS}_alg-${SAMPLING}_rho-${RHO}_delta-${DELTA}_samples-${SAMPLES}$EXHAUSTIVE
date
echo ./graph_compression -i $EXHAUSTIVE -c $SAMPLING -p $PARTITIONS -n $SAMPLES -m 2 -r $RHO -d $DELTA -t 8 -g $GRAPH -v $DATA -s $INDEX
./graph_compression -i -c $SAMPLING -p $PARTITIONS -n $SAMPLES -m 2 -r $RHO -d $DELTA -t 8 -g $GRAPH -v $DATA -s $INDEX > $TREENAME.tree
./viztree.sh $TREENAME $NAMES
commentSYN

<<commentWIKI
GRAPH=$DATADIR/WIKI/wiki-gte3links-udir-graph
DATA=$DATADIR/WIKI/wiki-data-gte3links-2008-0
INDEX=$DATADIR/WIKI/wiki-gte3links-udir-index
NAMES=$DATADIR/WIKI/wiki-gte3links-names
PARTITIONS=20
SAMPLES=100000
RHO=0.5
DELTA=0.1

TREENAME=${DATA}_${PARTITIONS}_alg-${SAMPLING}_rho-${RHO}_delta-${DELTA}_samples-${SAMPLES}$EXHAUSTIVE
date
echo ./graph_compression -i $EXHAUSTIVE -c $SAMPLING -p $PARTITIONS -n $SAMPLES -m 2 -r $RHO -d $DELTA -t 8 -g $GRAPH -v $DATA -s $INDEX
./graph_compression -i -c $SAMPLING -p $PARTITIONS -n $SAMPLES -m 2 -r $RHO -d $DELTA -t 8 -g $GRAPH -v $DATA -s $INDEX > $TREENAME.tree
./viztree.sh $TREENAME $NAMES
commentWIKI

