#!/bin/bash

#GRAPH=../experiment/SYN/syn1000.graph
#DATA=../experiment/SYN/syn1000.data
#INDEX=../experiment/SYN/syn1000.index
GRAPH=../experiment/WIKI/wiki-str-gte3links-udir-graph
DATA=../experiment/WIKI/wiki-data-str-gte3links-2008-0
INDEX=../experiment/WIKI/wiki-str-gte3links-udir-index
PARTITIONS=2
SAMPLES=1000
RHO=0.1
DELTA=0.7

echo graph_compression -i -c STUS -p $PARTITIONS -n $SAMPLES -m 2 -r $RHO -d $DELTA -t 8 -g $GRAPH -v $DATA -s $INDEX
./graph_compression -i -c STUS -p $PARTITIONS -n $SAMPLES -m 2 -r $RHO -d $DELTA -t 8 -g $GRAPH -v $DATA -s $INDEX > tree

sed -e 's/^[ ]*//g' tree | gawk -F " " '
  BEGIN{print "digraph a {" } 
  $1~/\(/ {
    if ($1 !~ /root/) edges[ "\"" $1 "\"->\"" $2 "\""] = "";
    if (NF >= 7)  { 
      split($6, es, "=");
      nodes["\"" $2 "\""] = "[ label=\"" es[2];
      nodes["\"" $2 "\""] = nodes["\"" $2 "\""] ":"
      if(NF <= 30) {
        for (i=7; i<= NF; i++) nodes["\"" $2 "\""] = nodes["\"" $2 "\""] " " $i;
      } else {
        nodes["\"" $2 "\""] = nodes["\"" $2 "\""] "..." 
      }
      nodes["\"" $2 "\""] = nodes["\"" $2 "\""] "\" shape=\"box\"]"
    } else {
      split($3, cs, "=");
      split($4, rs, "=");
      nodes["\"" $2 "\""] = "[ label=\"S(" cs[2] "," rs[2] ")\" ]";
    }      
  } 
  END{
    for (n in nodes) print n " " nodes[n];
    for (e in edges) {
      if (e~/i\"$/ ) print e " [label=\"in\"]";
      else print e " [label=\"out\"]";
    }
    print "}" }' | dot -Tpng > tree.png; eog tree.png &
