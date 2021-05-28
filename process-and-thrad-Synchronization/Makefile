all:compile	run	clean

compile:
	gcc -g -Wall -o part1 part1.c -lpthread -lrt
	
	gcc -g -Wall -o part2 part2.c -lpthread -lrt

run:
	./part1 100 9 test1.txt test2.txt test3.txt test4.txt test5.txt test6.txt test7.txt test8.txt test9.txt out1.txt
	./part2 100 9 test1.txt test2.txt test3.txt test4.txt test5.txt test6.txt test7.txt test8.txt test9.txt out2.txt

clean:	
	rm -fr part1 part1.o *~		
	rm -fr part2 part2.o *~
