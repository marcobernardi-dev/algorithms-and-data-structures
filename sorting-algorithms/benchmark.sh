#!/bin/bash

FIELDS=(1 2 3 4)
K_VALUES=(0 10 50 100 500 1000 5000 10000 1000000)

mkdir -p benchmark_results

for field in "${FIELDS[@]}"; do

    case $field in
        1) name="id" ;;
        2) name="field1" ;;
        3) name="field2" ;;
        4) name="field3" ;;
    esac

    for k in "${K_VALUES[@]}"; do

        output="/tmp/sorted_${name}_k${k}.bin"
        log="benchmark_results/${name}_k${k}.txt"

        echo "========================================"
        echo "Field: $field ($name) | k: $k"
        echo "========================================"

        start=$(date +%s)

        perl -e 'alarm 300; exec @ARGV' \
            /usr/bin/time -p \
            ./bin/main_ex1 ../records.bin "$output" "$field" "$k" \
            2> "$log"

        status=$?

        end=$(date +%s)
        elapsed=$((end - start))

        if [ $status -ne 0 ]; then
            echo "TIMEOUT (> 5 minuti)"
            echo "TIMEOUT (> 5 minuti)" > "$log"
        else
            echo "Terminato in ${elapsed} secondi"
        fi

        rm -f "$output"

    done
done