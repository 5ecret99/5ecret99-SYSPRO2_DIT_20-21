#!/bin/bash
inputFile=$1
input_dir=$2
numFilesPerDirectory=$3


if [[ "$#" -ne 3 ]]; then
	echo "Scrip requires exactly 3 command line arguments(./create_infiles.sh inputFile input_dir numFilesPerDirectory)"
	exit 1
fi

if [[ "$numFilesPerDirectory" < 1 ]]; then
	echo "numFilesPerDirectory is less than 1"
	exit 1
fi

mkdir $input_dir

#gia ka8e grammi
while read line; do
	arr=($line)
	country=${arr[3]}
	#ean den iparxei fakelos gia thn xwra ton ftiaxnei kai ftiaxnei kai ton katallilo ari8mo arxeiwn
	if [[ ! -e $input_dir/$country ]]; then
		mkdir $input_dir/$country
		for (( i = 1; i <= $numFilesPerDirectory; i++ )); do
			file="$country-$i.txt"
			touch $input_dir/$country/$file
		done
	fi
	#dialegi ena arxeio tyxaia
	i="$(($RANDOM%$numFilesPerDirectory+1 ))"
	#echo $i
	file="$country-$i.txt"
	#grafei ton citizen
	echo "$line">> $input_dir/$country/$file

done<$inputFile

exit 0