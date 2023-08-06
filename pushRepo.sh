#!/bin/bash

RepoSO=/media/sf_SOLabVM/Git_Repo/SOproj/

thisDir=$(pwd)
cd $thisDir/SOproj
thisDir=$(pwd)
cartellaLavoro=projSO

cp -r $thisDir/$cartellaLavoro $RepoSO
if [ $? ]
	then 
		echo "Push di $thisDir/$cartellaLavoro in $RepoSO eseguito!"
		echo ""
		ls -l $thisDir/projSO
		ls -l $RepoSO/$cartellaLavoro
fi

pwd




 
