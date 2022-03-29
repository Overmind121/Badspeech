#!/usr/bin/env bash
make clean && make

rm lookups.dat branches_traversed.dat hash_load.dat bloom_load.dat height_table height_filter.dat
echo "Running tests"
for ((i=0; $i!=5; i+=1)); do
    echo $i
done

for ((i=1000; $i!=1000000; i+=1000)); 
do
    echo -n "$i " >> branches_traversed.dat
    ./banhammer -s -t $i < alice29.txt | awk '$3=="size:"{print $4}' >> branches_traversed.dat

    #echo -n "$i " >> lookups.dat
    #./banhammer -s -t $i < alice29.txt | awk '$3=="value:"{print $4}' >> lookups.dat

    #echo -n "$i " >> hash_load.dat
    #./banhammer -s -t $i < alice29.txt | awk '$1=="Hash"{print $4}' >> hash_load.dat
    
    #echo -n "$i " >> bloom_load.dat
    #./banhammer -s -f $i < alice29.txt | awk '$1=="Bloom"{print $4}' >> bloom_load.dat
    
    #echo -n "$i " >> height_table.dat
    #./banhammer -s -t $i < alice29.txt | awk '$3=="height:"{print $4}' >> height_table.dat
    
    #echo -n "$i " >> height_filter.dat
    #./banhammer -s -f $i < alice29.txt | awk '$3=="height:"{print $4}' >> height_filter.dat
done
echo "Completed"
gnuplot << EOF
set terminal pdf
set key outside
set zeroaxis


set output "BranchesTraversed Vs TableSize.pdf"
set title "BranchesTraversed vs Table size"
plot "branches_traversed.dat" using 1:2 with lines title "BranchesTraversed"
set xlabel 'Table Size'
set xtics rotate
set ylabel 'BranchesTraversed'




EOF
make clean
echo "done."
