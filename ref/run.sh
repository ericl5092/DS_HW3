for((N=1;N<5;N=N+1))
do
    echo ${N}
    ./mydb -f do${N} > do${N}.txt
    ./mydb-ref -f do${N} >ref-do${N}.txt
done
