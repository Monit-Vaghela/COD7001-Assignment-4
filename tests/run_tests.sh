#!/bin/bash

# ---------------------------------------
# Lab 4 - Test Runner
# Directory structure:
# tests/
#   easy/
#   medium/
#   hard/
#   invalid/
# ---------------------------------------

VM_BIN="../VM"
OUT_FILE="output.txt"

# Reset output file
echo "LAB 4 VM TEST RESULTS" > "$OUT_FILE"
echo "=====================" >> "$OUT_FILE"
echo "" >> "$OUT_FILE"

run_folder () {
    local folder=$1

    if [ ! -d "$folder" ]; then
        return
    fi

    echo "==============================" >> "$OUT_FILE"
    echo "TEST GROUP: $folder" >> "$OUT_FILE"
    echo "==============================" >> "$OUT_FILE"
    echo "" >> "$OUT_FILE"

    for test in "$folder"/*.asm; do
        # Skip if no .asm files
        [ -e "$test" ] || continue

        echo "----------------------------------" >> "$OUT_FILE"
        echo "TEST FILE: $test" >> "$OUT_FILE"
        echo "----------------------------------" >> "$OUT_FILE"

        "$VM_BIN" -ae "$test" >> "$OUT_FILE" 2>&1

        echo "" >> "$OUT_FILE"
    done
}

# Run all test folders
run_folder "easy"
run_folder "medium"
run_folder "hard"
run_folder "invalid"

echo "==============================" >> "$OUT_FILE"
echo "ALL TESTS COMPLETED" >> "$OUT_FILE"
echo "==============================" >> "$OUT_FILE"
