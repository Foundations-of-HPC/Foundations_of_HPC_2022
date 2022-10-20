
set terminal png size 2048,1600 enhanced
set terminal png font "Garamond"
set output "misses.png"


if (!exists("infile")) infile="cache_size.dat"

stats infile using 2:6 nooutput
min_cpe=STATS_min_y
max_cpe=STATS_max_y


set lmargin 24
set rmargin 24
set bmargin 9
set tmargin 4

set xlabel "SET SIZE (in 32K capacity)" font ", 40" offset 0,-4
set ylabel "L1 miss" font ",40" offset -8
set y2label "L2 miss" font ",40" offset 8

set tics font ", 32"
set xtics offset 0,-1.5

set yrange [0:0.14]
set y2range [0:0.3]
set ytics nomirror
set y2tics nomirror

set key on inside center right font ", 32"
plot infile u ($2/4096):14 axis x1y2 w p ps 4 pt 3 lc -1 title "L2 miss", "" u ($2/4096):10 w lp ps 4 pt 7 lw 2 lc 1 title "L1 miss"

set output "cpe.png"
set yrange [ min_cpe*0.98: max_cpe*1.02]
#set y2range [0.5:0.85]

set ylabel "CPE" font ",40" offset -8
set y2tics nomirror

set key on inside center right font ", 32"
plot infile u ($2/4096):14 axis x1y2 w p ps 4 pt 3 lc -1 title "L2 miss", "" [:][C_min:C_max] u ($2/4096):6 axis x1y1 w lp ps 4 pt 7 lw 2 lc 1 title "Cycle per el."

set terminal x11 enhanced
