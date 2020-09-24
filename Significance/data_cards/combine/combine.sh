#!/bin/bash
#cd /afs/cern.ch/user/y/yian/work/PKU-Cluster/HiggsCombineTools/CMSSW_8_1_0/src
#cmsenv
#cd -

#cp /afs/cern.ch/user/y/yian/work/PKU-Cluster/Significance/CR/data_cards/txt/*16* .
#cp /afs/cern.ch/user/y/yian/work/PKU-Cluster/Significance/data_cards/txt/*16* .
#cp /afs/cern.ch/user/y/yian/work/PKU-Cluster/Significance/CR/data_cards/txt/*17* .
#cp /afs/cern.ch/user/y/yian/work/PKU-Cluster/Significance/data_cards/txt/*17* .
#cp /afs/cern.ch/user/y/yian/work/PKU-Cluster/Significance/CR/data_cards/txt/*18* .
#cp /afs/cern.ch/user/y/yian/work/PKU-Cluster/Significance/data_cards/txt/*18* .
#combineCards.py mu*16*.txt ele*16*.txt >& full16_test.txt
#combineCards.py mu*17*.txt ele*17*.txt >& full17_test.txt
#combineCards.py mu*18*.txt ele*18*.txt >& full18_test.txt
files="\
full16_test.root
full17_test.root
full18_test.root
"
for rootfile in $files
do
name=`echo $rootfile | awk -F. '{print $1}'`
sed 's/lnN.*$//' ${name}.txt >tmp1
sed  's/\s\+$//g' tmp1 >tmp
num=`sed -n -e '/rate/=' tmp`
line=$[$num+1]
echo "$line"
sed -i "1,$line d" tmp
theory=`sed -n -e '/theory/p' tmp`
sed -i '/theory/d' tmp
echo "$theory"
sed -n -e '/Stat/p' tmp > tmp2
sed -i ':label;N;s/\n/,/;t label' tmp2
#sed -i "$ a $theory" tmp
mv tmp2 freeze_${name}.txt
NP=`cat freeze_${name}.txt`
echo "$NP"
#combine -M Significance --expectSignal=1 -t -1 ${name}.txt > result_${name}.txt 
#combine -M Significance --expectSignal=1 -t -1 ${name}.txt --freezeParameters all > result_freezeAll${name}.txt 
#combine -M Significance --expectSignal=1 -t -1 ${name}.txt --freezeParameters $NP > result_freezeMCStat${name}.txt 
text2workspace.py ${name}.txt -m 125
combineTool.py -M Impacts -d ${name}.root -m 125 --doInitialFit --robustFit 1
combineTool.py -M Impacts -d ${name}.root -m 125 --robustFit 1 --doFits
combineTool.py -M Impacts -d ${name}.root -m 125 -o impacts_${name}.json
plotImpacts.py -i impacts_${name}.json -o impacts_${name}
done