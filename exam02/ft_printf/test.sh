#!/bin/bash

FLAGS="-Wall -Werror -Werror -Wno-format-zero-length -Wno-format"

gcc $FLAGS -D REAL ft_printf.c -o real
gcc $FLAGS ft_printf.c -o mine

diff <(./real) <(./mine)

rm mine real
