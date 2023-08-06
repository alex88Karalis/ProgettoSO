#!/bin/bash

RepoSO=/media/sf_SOLabVM/Git_Repo/SOproj

thisDir=$(pwd)

cp -r $RepoSO $thisDir
if [ $? ]
	then 
		echo "Pull repositori eseguito!"
		echo ""
		ls -l $thisDir
fi

pwd




 
