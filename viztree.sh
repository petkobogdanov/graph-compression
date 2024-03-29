#!/bin/bash

TREENAME=$1
DATA=$2
NAMES=$3
CUTS=$4

sed -e 's/^[ ]*//g' $TREENAME.tree | gawk -F " " -v anfn=$NAMES -v dfn=$DATA -v cuts=$CUTS -v ofn=$TREENAME.tree.dot '
  BEGIN{
    PROCINFO["sorted_in"] = "@val_num_desc"
    print "digraph a { " > ofn
    if (anfn!="") {
      # parse the author topconferences
      while((getline line < anfn)>0) {
        split(line,sline,",");
        split(sline[3],conf,":");
        anames_short[sline[1]]=sline[2] " [" conf[2] "]"
        anames[sline[1]]=sline[2] "[" sline[3];
        for(i=4; i<=length(sline); i++) 
          anames[sline[1]] = anames[sline[1]] ";" sline[i];
        anames[sline[1]] = anames[sline[1]] "]";
      }
      close(anfn)
    }
    # parse author counts 
    while((getline line < dfn)>0) {
      split(line,sline,",");
      avals[sline[1]]=sline[2];  
    }
    close(dfn)
    if (cuts=="") cuts = 2000;
    stopreading=0;
  } 

  $0~/PRINTING TREE/ && stopreading==0 {
    if ($NF == cuts) stopreading=1
    else {
      print "Reading tree " $NF;
      # a new tree dump -> cleanup
      for (n in nodes) delete nodes[n];
      for (e in edges) delete edges[e];
    }
  }
  $1~/\(/ && stopreading==0 {
    if ($1 !~ /root/) edges[ "\"" $1 "\"->\"" $2 "\""] = ""; 
    if (NF >= 7)  { 
      split($6, mus, "=");
      split($5, es, "=");
      nodes["\"" $2 "\""] = "[ label=\" AVG:" int(mus[2]) ", E:" es[2];
      if(anfn!="") {
        #for (i=7; i<= NF; i++) cauth[anames[$i]]=avals[$i];
		for (i=7; i<= NF; i++) cauth[anames_short[$i]]=avals[$i];
        i=0;
        # calculating % of DM from the mixture of DM+CN
        cncnt=0;
        dmcnt=0;
        for (ca in cauth) {
          if (ca ~ /\[CN\]$/) cncnt += 1;
          if (ca ~ /\[DM\]$/ ) dmcnt += 1;
        }
        #nodes["\"" $2 "\""] = nodes["\"" $2 "\""] "DM:" int(100*dmcnt/(cncnt+dmcnt)) "%";
        pcdm = dmcnt/(cncnt+dmcnt+0.0001);  
        for (ca in cauth) {
          if (ca !~/\[\]$/) {
            le = split(ca,cas," ");
            #nodes["\"" $2 "\""] = nodes["\"" $2 "\""] "\\n" ca "-" cauth[ca];
            if (i%1==0) nodes["\"" $2 "\""] = nodes["\"" $2 "\""] "\\n"
		    nodes["\"" $2 "\""] = nodes["\"" $2 "\""] substr(cas[1],1,1) ". " cas[le-1] " " cas[le];
            i++;
          } 
          if (i>=8) break;
        }
        for (ca in cauth) delete cauth[ca]; 
      }
      else 
        for (i=7; i<= 20; i++) 
          nodes["\"" $2 "\""] = nodes["\"" $2 "\""] "\\n" $i;
      if (NF>23) nodes["\"" $2 "\""] = nodes["\"" $2 "\""] "..." 
      nodes["\"" $2 "\""] = nodes["\"" $2 "\""] "\" shape=\"circle\"";
      nodes["\"" $2 "\""] = nodes["\"" $2 "\""] " style=\"wedged\" fillcolor=\"yellow;" pcdm ":lightskyblue\" color=\"white\" fontsize=\"16\" fixedsize=true height=3]"
    } else {
      split($3, cs, "=");
      le = split(anames_short[cs[2]],cas," ");
      split($4, rs, "=");
      split($5, es, "=");
      split($6, mus, "=");
      #if(anfn!="") nodes["\"" $2 "\""] = "[ label=\""anames[cs[2]] "-" avals[cs[2]] "," rs[2] ",mu=" mus[2] ",E=" es[2] ")\" ]";
      if (cas[le]~/DM/) fcolor= "shape=\"box\" fontsize=\"18\" style=\"filled\" fillcolor=\"lightskyblue\"] "
      else if (cas[le]~/CN/) fcolor= "shape=\"box\" fontsize=\"18\" style=\"filled\" fillcolor=\"yellow\"] "
      else fcolor="shape=\"box\" fontsize=\"18\" style=\"filled\" fillcolor=\"yellow\"] "
      if(anfn!="") nodes["\"" $2 "\""] = "[ label=\"" substr(cas[1],1,1) ". " cas[le-1] ", R:" rs[2] "\\n AVG:" int(mus[2]) ",E:" int(es[2]) "\" " fcolor;
      else nodes["\"" $2 "\""] = "[ label=\"S(" cs[2] "," rs[2] ",mu=" mus[2] ",E=" es[2] ")\" ]";
    }      
  } 
  END{
    for (n in nodes) print n " " nodes[n] > ofn;
    print "subgraph clusterA { label=Legend; \"Computer Networks\" [shape=\"box\" style=\"filled\" fillcolor=\"yellow\" fontsize=\"20\"]; \"Data Management\" [shape=\"box\" style=\"filled\" fillcolor=\"lightskyblue\" fontsize=\"20\"] } " > ofn;
    for (e in edges) {
      if (e~/i\"$/ ) print e " [label=\"in\" fontsize=\"20\" arrowType=\"normal\" penwidth=2]" > ofn;
      else print e " [label=\"out\" fontsize=\"20\" arrowType=\"empty\" penwidth=2]" > ofn;
    }
    print "}" > ofn }'

dot -Teps $TREENAME.tree.dot > $TREENAME.eps; #eog $TREENAME.png &
