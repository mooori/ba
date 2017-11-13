#!/bin/bash
# Download graphs from Konect 
# Place konect/
# unpacks compressed folder
# copies out file to gs/ (stripping out from filename

for g in \
    petster-hamster \
    youtube-u-growth \
    com-amazon \
    wordnet-words \
    opsahl-powergrid \
    roadNet-TX \
    as20000102 \
    moreno_propro \
    roadNet-PA \
    maayan-pdzbase \
    loc-brightkite_edges \
    as-caida20071105 \
    roadNet-CA \
    petster-friendships-cat \
    arenas-jazz \
    petster-friendships-hamster \
    flickrEdges \
    facebook-wosn-links \
    subelj_euroroad \
    moreno_lesmis \
    actor-collaboration
do
    wget konect.uni-koblenz.de/downloads/tsv/$g.tar.bz2
    tar xvf $g.tar.bz2
    cp $g/out.$g gs/$g
done
