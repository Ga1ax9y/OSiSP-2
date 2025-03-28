#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <input_file>"
    exit 1
fi

input_file=$1

if [ ! -f "$input_file" ]; then
    echo "File $input_file does not exist."
    exit 1
fi
tail -c 1 "$input_file" | read -r _ || echo >> "$input_file"
temp_file=$(mktemp)

while IFS=, read -r name quantity price; do
    name=$(echo "$name" | xargs)
    quantity=$(echo "$quantity" | xargs)
    price=$(echo "$price" | xargs)

    if ! [[ "$name" =~ ^[a-zA-Zа-яА-Я\ ]+$ ]] || ! [[ "$quantity" =~ ^[0-9]+$ ]] || ! [[ "$price" =~ ^[0-9]+(\.[0-9]+)?$ ]]; then
        echo "Invalid data: $name, $quantity, $price"
        continue
    fi

    total_cost=$(echo "$quantity * $price" | bc)

    echo "$name,$quantity,$total_cost" >> "$temp_file"
done < "$input_file"

awk -F, '{
    name=$1;
    quantity=$2;
    total_cost=$3;
    sum_quantity[name] += quantity;
    sum_total_cost[name] += total_cost;
} END {
    for (name in sum_quantity) {
        print name, sum_quantity[name], sum_total_cost[name];
    }
}' "$temp_file" > output.txt

rm "$temp_file"

echo "Processing complete. Results saved to output.txt"
