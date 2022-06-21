#!/bin/bash

echo "RUN with n = $1 and distance $2";

echo "n_cores;ff_time;threads_time;threads2_time" > "par_results_$1_$2.csv";
for n_cores in {1..128}
do
    seq_avg=0;
    ff_avg=0;
    threads_avg=0;
    threads2_avg=0;

    for i in {1..5}
    do
        if  [[ $n_cores -eq 1 ]]
        then
            # getting times
            times=$(./main $1 1 5 $2 | cut -d " " -f12);
            seq_time=$(echo $times | cut -d " " -f1);
            ff_time=$(echo $times | cut -d " " -f2);
            threads_time=$(echo $times | cut -d " " -f3);
            threads2_time=$(echo $times | cut -d " " -f4);

            seq_avg=$(echo $seq_avg+$seq_time | bc -l);
        else
            # getting times
            times=$(./main $1 $n_cores 5 $2 | cut -d " " -f12);
            ff_time=$(echo $times | cut -d " " -f1);
            threads_time=$(echo $times | cut -d " " -f2);
            threads2_time=$(echo $times | cut -d " " -f3);
        fi
        
        ff_avg=$(echo $ff_time+$ff_avg | bc); 
        threads_avg=$(echo $threads_time+$threads_avg | bc);
        threads2_avg=$(echo $threads2_time+$threads2_avg | bc); 
    done

    if [[ $n_cores -eq 1 ]]
    then
        seq_avg=$(echo "scale=2; $seq_avg/5" | bc -l);
        echo "$seq_avg" > "seq_result_$1_$2.csv";
    fi

    ff_avg=$(echo "scale=2; $ff_avg/5" | bc -l);
    threads_avg=$(echo "scale=2; $threads_avg/5" | bc -l);
    threads2_avg=$(echo "scale=2; $threads2_avg/5" | bc -l);
    
    echo "$n_cores;$ff_avg;$threads_avg;$threads2_avg" >> "par_results_$1_$2.csv";

    echo "scale=2; $n_cores/128*100" | bc -l
done