#!/bin/bash

set -eou pipefail

gcc -ggdb -fsanitize=address -Wall -Wextra -Wpedantic -Werror -o test-stdin test-stdin.c
gcc -ggdb -fsanitize=address -Wall -Wextra -Wpedantic -Werror -o test-monte test-monte.c

err() { echo "Error."; exit 1; }
url() { printf "\e]8;;%s\e\\%s\e]8;;\e\\" "$1" "$1"; }

echo "Testing against the NIST Cryptographic Algorithm Validation Program (CAVP)"
echo "Using requirements from the Secure Hash Standard Validation System (SHAVS) defined in FIPS 180-4"
echo -n "See "; url "https://csrc.nist.gov/Projects/Cryptographic-Algorithm-Validation-Program/Secure-Hashing" "the project page"; echo " for more information"
if ! cd shabytetestvectors 2>/dev/null; then
    echo "Downloading test vectors"
    wget --quiet https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Algorithm-Validation-Program/documents/shs/shabytetestvectors.zip || err
    unzip -q shabytetestvectors.zip || err
    cd shabytetestvectors || err
fi

for typ in Long Short; do
    echo "Testing Selected ${typ} Messages"
    grep -E '^Len|^Msg|^MD' "SHA256${typ}Msg.rsp" | cut -d ' ' -f 3 | \
            paste -d " " - - - | tr -d '\r' | \
    while read -r len msghex expected; do
        if [[ "$len" == "0" ]] && [[ "$msghex" == "00" ]]; then
            msghex=""
        fi
        output=$(echo -n "$msghex" | xxd -p -r | ../test-stdin | tr -d '\n')
        if [[ "$output" != "$expected" ]]; then
            echo "Mismatch."
            echo "input 0x$msghex"
            echo -n "$msghex" | xxd -p -r | xxd
            echo "-$expected"
            echo "+$output"
            diff -Naur <(printf "%s" "$output" | sed 's/./&\n/g') <(printf "%s" "$expected" | sed 's/./&\n/g')
            err;
            # shellcheck disable=SC2317 # unreachable
            break;
        fi
        echo -n '.'
    done
    echo
done

seed=$(grep ^Seed SHA256Monte.rsp | cut -d ' ' -f 3 | tr -d '\r')
echo "Testing Psudorandomly Generated Messages (Monte Carlo)"
echo "seed=$seed"
mapfile -t expected < <(grep ^MD SHA256Monte.rsp | cut -d ' ' -f 3 | tr -d '\r')
mapfile -t output < <(../test-monte "$seed")
for j in {0..99}; do
    if [[ "${output[j]}" != "${expected[j]}" ]]; then
        echo -e "\nMismatch."
        echo "-${expected[j]}"
        echo "+${output[j]}"
        diff -Naur <(printf "%s" "${output[j]}" | sed 's/./&\n/g') <(printf "%s" "${expected[j]}" | sed 's/./&\n/g')
        err
        # shellcheck disable=SC2317 # unreachable
        break;
    fi
    # MD=("$seed" "$seed" "$seed")
    # for i in {3..1002}; do
    #     M="${MD[i-3]}${MD[i-2]}${MD[i-1]}"
    #     MD[i]=$(echo "$M" | xxd -p -r | ../test-stdin | tr -d '\n')
    # done
    # MD[j]=${MD[1002]}
    # seed=${MD[j]}
    # if [[ "${MD[j]}" != "${expected[j]}" ]]; then
    #     echo -e "\nMismatch."
    #     echo "-${expected[j]}"
    #     echo "+${MD[j]}"
    #     diff -Naur <(printf "%s" "${MD[j]}" | sed 's/./&\n/g') <(printf "%s" "${expected[j]}" | sed 's/./&\n/g')
    #     err
    #     # shellcheck disable=SC2317 # unreachable
    #     break;
    # fi
    echo -n '.'
done
echo
echo "All tests passed"
cd ..
