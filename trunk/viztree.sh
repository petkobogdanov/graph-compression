#!/bin/bash

TREENAME=$1
NAMES=$2

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
  $0~/PRINTING TREE/ {
    # a new tree dump -> cleanup
    for (n in nodes) delete nodes[n];
    for (e in edges) delete edges[e];
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
      #print("the mapping of -" cs[2] "- is " anames[cs[2]]);
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
