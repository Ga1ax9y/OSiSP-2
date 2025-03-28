#!/bin/bash

#chmod +x screensaver.sh
bouncing_clock() {
    clear
    echo -ne "\e[?25l"
    rows=$(tput lines)
    cols=$(tput cols)

    x=$((cols / 2))
    y=$((rows / 2))

    dx=1
    dy=1

    prev_x=$x
    prev_y=$y

    while true; do
        tput cup $prev_y $prev_x
        echo -n "           "
        tput cup $((prev_y + 1)) $prev_x
        echo -n "                           "

        x=$((x + dx))
        y=$((y + dy))

        if [ $x -ge $((cols - 20)) ] || [ $x -le 1 ]; then
            dx=$((dx * -1))
            x=$((x + dx * 2))
        fi
        if [ $y -ge $((rows - 3)) ] || [ $y -le 1 ]; then
            dy=$((dy * -1))
            y=$((y + dy * 2))
        fi

        tput cup $y $x
        current_time=$(date +"%T")
        current_date=$(date +"%A, %d %B %Y")
        echo -e "\e[1;37m$current_time\e[0m"
        tput cup $((y + 1)) $x
        echo -e "\e[1;34m$current_date\e[0m"

        prev_x=$x
        prev_y=$y

        sleep 0.1
    done
}

bouncing_clock
