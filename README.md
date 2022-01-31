SDI:1115201800220 GEORGIOS GEORGIOU

commands:
	travelMonitor:
		make: make
		run: ./travelMonitor -m numMonitors -b bufferSize -s sizeOfBloom -i input_dir
		clean: make clean

	script/create_infiles.sh:
		run: ./create_infiles.sh inputFile input_dir numFilesPerDirectory

tested on: linux mint(my personal computer),linux university machines

To programma aptous elegxous mou doulevei gia olles tis periptwseis pou den einai akrees(to exw dokimasei me 20+ monitor buffer 1 sizeofBloom 100000 se 70000 records) kai den kanei timed out to poll.Exw simperilavi sxolia ston kwdika gia olo ton neo kwdika pou e3igoun ti kanw kata gramma,opotan to 8ewrisa fliaro na grafw ta idia kai sto Readme.

arxeia:

	child_functions.c/.h:simperilanvani tis functions pu xrhsimopoiw stin main tou Monitor.c gia tin eisagwgi twn dedomenwn stis domes kai tin ekperewsi twn entolwn pu dinontai apto xrhsth

	parent_functions.c/.h:simperilanvani tis functions pu xrhsimopoiw stin main tou travelMonitor.c gia tin ekperewsi merikwn entolwn pu dinontai apto xrhsth kai to diavasma ton bloomfilters apo ka8e paidi

	signal.c/.h:simperilamvani tin sinartisi gia thn dhmiourgia twn logfiles

	records.c/.h:domh twn records ka8ws kai sinartiseis gia ta dates (time_t char* metatropes kai ta loipa)

	hash.c/.h:sinartiseis gia hashing opws mas dw8ikan stin prwth ergasia ka8ws kai i global metavliti HVALUE pu xrhsimopoiw gia hashing

	skiplist.c/.h:sinartisis gia tin skiplist

	bloomfilter.c/.h:sinartiseis gia to bloomfilter

	hashtable.c/.h:sinarthseis gia ton hashtable ka8ws kai oi domes pu xrhsimopoiw mazi tou

	pipes.c/.h:sinartisies gia dhmiourgia/diagrafi named pipes kai encode/decode se named pipes me to buffersize

	directory.c/.h:sinarthseis pu anate8oun tin ka8e xwra ston swsto monitor

	travelMonitor.c:kyrio proramma parent

	Monitor.c:child

	script/create_infiles.sh:to scriptfile

simantika:
1.o fakelos log_files prepei na iparxei me auto akrivws to onoma gia na vazei mesa ta logfiles to programma
2.o sighandler toso ston patera oso kai sto paidi ginete afou diavastoun/stal8un ta bloomfilters
3.mou parousiazontan provlima otan ekana close(fd) kai amesws opend(fd) opou kolouse to programma etsi ilopoiisa tin sinartisi waiting() sto pipes.h/.c
***telika anti na kligw kai na anoigw ta named pipes sinexia ta anoigw kai ta kleinw mia fora opotan den m xriasthke to waiting() alla afisa tin sinartisi waiting() ston kwdika
