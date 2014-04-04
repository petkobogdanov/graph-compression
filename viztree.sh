#!/bin/bash

DATADIR=/home/petko/Dropbox/slice_tree/src/experiment
SAMPLING=STBS
#EXHAUSTIVE="-x"
EXHAUSTIVE=""

NAMES=""

GRAPH=$DATADIR/SYN/syn1000.graph
DATA=$DATADIR/SYN/syn1000.data
INDEX=$DATADIR/SYN/syn1000.index
PARTITIONS=5
SAMPLES=100
RHO=0.1
DELTA=0.5

<<commentWIKI
GRAPH=$DATADIR/WIKI/wiki-gte3links-udir-graph
DATA=$DATADIR/WIKI/wiki-data-gte3links-2008-0
INDEX=$DATADIR/WIKI/wiki-gte3links-udir-index
PARTITIONS=5
SAMPLES=173000
RHO=0.1
DELTA=0.5
commentWIKI

<<commentDBLP
GRAPH=$DATADIR/DBLP/dblp.graph
DATA=$DATADIR/DBLP/dblp_dm_net.data
INDEX=$DATADIR/DBLP/dblp.index
NAMES=$DATADIR/DBLP/authors-id-name-topconf3-names
PARTITIONS=20
SAMPLES=15000
RHO=0.5
DELTA=0.1
commentDBLP

<<commentT
GRAPH=$DATADIR/TWITTER-CRAWL/twitter-crawl-udir.graph
DATA=$DATADIR/TWITTER-CRAWL/twitter-crawl-tags_log.data
INDEX=$DATADIR/TWITTER-CRAWL/twitter-crawl-udir.index
NAMES=$DATADIR/TWITTER-CRAWL/twitter-crawl-users-id-name-topht3
PARTITIONS=50
SAMPLES=15000
RHO=0.9
DELTA=0.1
commentT


TREENAME=${DATA}_${PARTITIONS}_alg-${SAMPLING}_rho-${RHO}_delta-${DELTA}_samples-${SAMPLES}$EXHAUSTIVE

#<<comment
echo ./graph_compression -i $EXHAUSTIVE -c $SAMPLING -p $PARTITIONS -n $SAMPLES -m 2 -r $RHO -d $DELTA -t 8 -g $GRAPH -v $DATA -s $INDEX
./graph_compression -i -c $SAMPLING -p $PARTITIONS -n $SAMPLES -m 2 -r $RHO -d $DELTA -t 8 -g $GRAPH -v $DATA -s $INDEX > $TREENAME.tree
#comment

sed -e 's/^[ ]*//g' $TREENAME.tree | gawk -F " " -v anfn=$NAMES '
  BEGIN{
    print "digraph a {" 
    if (anfn!="") {
      # parse the author topconferences
      while((getline line < anfn)>0) {
        split(line,sline,",");
        anames[sline[1]]=sline[2] "[" sline[3];
        for(i=4; i<=length(sline); i++) anames[sline[1]] = anames[sline[1]] ";" sline[i];
        anames[sline[1]] = anames[sline[1]] "]";
      }
    }
  } 
  $1~/\(/ {
    if ($1 !~ /root/) edges[ "\"" $1 "\"->\"" $2 "\""] = "";
    
    if (NF >= 7)  { 
      split($6, mus, "=");
      split($5, es, "=");
      nodes["\"" $2 "\""] = "[ label=\"" mus[2] "(E=" es[2] ")";
      nodes["\"" $2 "\""] = nodes["\"" $2 "\""] ":"
      
      if(anfn!="") for (i=7; i<= 20; i++) nodes["\"" $2 "\""] = nodes["\"" $2 "\""] "\\n" anames[$i];
      else for (i=7; i<= 20; i++) nodes["\"" $2 "\""] = nodes["\"" $2 "\""] "\\n" $i;
      if (NF>20) nodes["\"" $2 "\""] = nodes["\"" $2 "\""] "..." 
      nodes["\"" $2 "\""] = nodes["\"" $2 "\""] "\" shape=\"box\"]"
    } else {
      split($3, cs, "=");
      split($4, rs, "=");
      split($5, es, "=");
      split($6, mus, "=");
      if(anfn!="") nodes["\"" $2 "\""] = "[ label=\"S(" anames[cs[2]] "," rs[2] ",mu=" mus[2] ",E=" es[2] ")\" ]";
      else nodes["\"" $2 "\""] = "[ label=\"S(" cs[2] "," rs[2] ",mu=" mus[2] ",E=" es[2] ")\" ]";
    }      
  } 
  END{
    for (n in nodes) print n " " nodes[n];
    for (e in edges) {
      if (e~/i\"$/ ) print e " [label=\"in\"]";
      else print e " [label=\"out\"]";
    }
    print "}" }' > $TREENAME.tree.dot 

dot -Tpng $TREENAME.tree.dot > $TREENAME.tree.png; eog $TREENAME.tree.png &
