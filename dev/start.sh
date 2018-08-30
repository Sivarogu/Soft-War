#!/bin/bash
set -e pipefail

choices=(
	"build environment (do this first)"
	"build external/easy-event [watch]"
	"build bridge [watch]"
	"build lib-client [watch]"
	"build visualizer [watch]"
	"build and run server [watch]"
	"run bridge [watch]"
)

commands=(
	"make -C dev build-env"
	"make -C dev build-easy-event"
	"make -C dev build-bridge"
	"make -C dev build-lib-client"
	"make -C dev build-visualizer"
	"make -C dev dev-server"
	"make -C dev run-bridge"
)

while [ 1 ]; do
	echo "Which script do you want to launch?"
	choice_count=0
	while [ "x${choices[choice_count]}" != "x" ]; do
		echo "  $choice_count. ${choices[choice_count]}"
		choice_count=$(($choice_count + 1))
	done
	echo
	read -p "> " user_choice

	if [[ $user_choice < 0 || $user_choice -ge $choice_count ]]; then
		echo -e "\033[0;31minvalid choice: $user_choice\033[0m\n"
	else
		command="${commands[user_choice]}"
		echo "execute command: $command"
		eval $command
		exit 0
	fi
done
