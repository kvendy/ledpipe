#!/bin/bash
COMMANDSFILE=params.txt
PIPEFILE=pipe.txt
OLDIFS=$IFS
IFS="|"
let count=0
commands=()

[ ! -f $PIPEFILE ] && { echo "$PIPEFILE file not found"; exit 99; }
pipename=$(head -n 1 $PIPEFILE)

echo "Availible commands:"

[ ! -f $COMMANDSFILE ] && { echo "$COMMANDSFILE file not found"; exit 99; }
while read command description
do
        echo "$count : $description"
        commands+=($command)
        let count++
done < $COMMANDSFILE
IFS=$OLDIFS

while true
do
        echo "Choose command:"
        read index

        if [ $index -eq $index 2>/dev/null ] && ! [ -z "$index" ]; then
                if (($index > 0)) && (($index < count)); then
                        echo "Argument:"
                        read argument
                        echo ${commands[index]} $argument > $pipename
                        cat $pipename
                else
                        echo "Wrong command"
                fi
        else
                echo "Wrong command"
        fi
done
