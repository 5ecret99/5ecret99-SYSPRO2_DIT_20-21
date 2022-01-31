all:		travelMonitor Monitor

child_functions.o: 	child_functions.c
		gcc -c child_functions.c

parent_functions.o: 	parent_functions.c
		gcc -c parent_functions.c

signal.o:	signal.c
		gcc -c signal.c

records.o: 	records.c
		gcc -c records.c

hash.o: 	hash.c
		gcc -c hash.c

skiplist.o: 	skiplist.c
		gcc -c skiplist.c

bloomfilter.o: 	bloomfilter.c
		gcc -c bloomfilter.c

hashtable.o: 	hashtable.c
		gcc -c hashtable.c

pipes.o:	pipes.c
		gcc -c pipes.c

directory.o:	directory.c
		gcc -c directory.c

travelMonitor.o:	travelMonitor.c
		gcc -c travelMonitor.c

Monitor.o:	Monitor.c
		gcc -c Monitor.c

travelMonitor:	pipes.o directory.o travelMonitor.o hashtable.o skiplist.o bloomfilter.o hash.o records.o parent_functions.o signal.o
travelMonitor:	pipes.o directory.o travelMonitor.o hashtable.o skiplist.o bloomfilter.o hash.o records.o parent_functions.o 
	gcc pipes.o directory.o travelMonitor.o hashtable.o skiplist.o bloomfilter.o hash.o records.o parent_functions.o signal.o -o travelMonitor

Monitor: Monitor.o pipes.o child_functions.o records.o hash.o skiplist.o bloomfilter.o hashtable.o signal.o
	gcc Monitor.o pipes.o child_functions.o records.o hash.o skiplist.o bloomfilter.o  hashtable.o  signal.o -o Monitor

clean:
	rm *o travelMonitor Monitor