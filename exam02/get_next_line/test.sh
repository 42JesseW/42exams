#!/bin/bash

FLAGS="-Wall -Wextra -Werror"
LEAK_PROG=valgrind
LEAK_PROG_FLAGS="--leak-check=full"
GNL_PROG=get_next_line

if ! command -v $LEAK_PROG &> /dev/null
then
	echo "Valgrind not installed. Can not do leak check"
	LEAK_PROG=""
	LEAK_PROG_FLAGS=""
fi

for i in {1..20}
do
	echo -n -e "[BUFFER_SIZE=$i]\t"
	if [ "$i" -lt 10 ]; then
		echo -n -e "\t"
	fi
	gcc -D BUFFER_SIZE=$i $FLAGS main.c get_next_line.c -o $GNL_PROG
	echo -n -e "COMPILATION SUCCESS\t"

	echo "DIFF"
	diff <(./$GNL_PROG < file.txt) <(cat < file.txt)

	if [ ! -z "$LEAK_PROG" ]; then
		echo -n -e "LEAKS"
		$LEAK_PROG $LEAK_PROG_FLAGS ./$GNL_PROG < file.txt &> out && cat out | grep 'ERROR SUMMARY'
	fi
	echo
	rm get_next_line out
done
