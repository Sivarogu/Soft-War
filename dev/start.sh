#!/bin/bash
set -e pipefail

choices=(
	"build environment (do this first)"
	"build and run server [watch]"
	"build and run external/easy-event [watch]"
	"build lib-client [watch]"
	"build bridge [watch]"
	"start bridge [watch]"
	"build visualizer [watch]"
)

commands=(
	"make -C dev build-env"
	"make -C dev run-server"
	"make -C dev run-easy-event"
	"make -C dev run-lib-client"
	"make -C dev run-bridge-build"
	"make -C dev run-bridge-start"
	"make -C dev run-visualizer"
)

ask_again=1
while [ $ask_again -eq 1 ]; do
	echo "Which script do you want to launch?"
	choice_count=0
	while [ "x${choices[choice_count]}" != "x" ]; do
		echo "  "$(( $choice_count + 1 ))". ${choices[choice_count]}"
		choice_count=$(( $choice_count + 1 ))
	done
	echo
	read -p "> " user_choice

	if [[ $user_choice < 1 || $user_choice > $choice_count ]]; then
		echo -e "\033[0;31minvalid choice: $user_choice\033[0m\n"
	else
		user_choice=$(( $user_choice - 1 ))
		command="${commands[user_choice]}"
		echo "execute command: $command"
		eval $command
		exit 0
	fi
done
