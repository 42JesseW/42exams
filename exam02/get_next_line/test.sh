#!/bin/bash

FLAGS="-Wall -Wextra -Werror"
LEAK_PROG=valgrind
LEAK_PROG_FLAGS="--leak-check=full"
GNL_PROG=get_next_line
RANDOM_FILE_NAME=random.txt
RANDOM_FILE_SIZE_KB=100000

echo "----------------------------------------------"
echo "             VARIABLE BUFFER_SIZE             "
echo "----------------------------------------------"
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

	echo -n "DIFF "
	diff <(./$GNL_PROG < file.txt) <(cat < file.txt) &> /dev/null
	if [[ "$?" -eq "1" ]]; then echo "FAILURE"; else echo "SUCCESS"; fi

	if [ ! -z "$LEAK_PROG" ]; then
		echo -e "LEAKS"
		$LEAK_PROG $LEAK_PROG_FLAGS ./$GNL_PROG < file.txt &> out && cat out | grep 'ERROR SUMMARY'
	fi
	rm $GNL_PROG
done

echo
echo "----------------------------------------------"
echo "          SMALL BUFFER_SIZE BIG FILE          "
echo "----------------------------------------------"
openssl rand -out $RANDOM_FILE_NAME -base64 $(( 1024 * $RANDOM_FILE_SIZE_KB ))
gcc -D BUFFER_SIZE=100 $FLAGS main.c get_next_line.c -o $GNL_PROG
./$GNL_PROG < $RANDOM_FILE_NAME > your_out
diff your_out $RANDOM_FILE_NAME
rm your_out $RANDOM_FILE_NAME