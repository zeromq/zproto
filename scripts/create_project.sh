#!/bin/bash

function help() {
	echo "usage:"
	echo "  $0 <project directory>"
	echo
	exit 1
}

[ $# -eq 1 ] || help
if [ -d "$1" ];then
	printf "\nError: Direcotry $1 already exists !!!\nPlease remove it first.\n\n"
fi

mkdir -p "$1"
if [ ! -d "$1" ];then
	printf "\nError: Direcotry $1 was not created !!!\n\n"
	help
fi

PROJECT_DIR="$(dirname $(dirname $(readlink -m $0)))"
MY_PROJ_NAME=$(basename "$1")

cp -r $PROJECT_DIR/skeleton/* "$1"/ || exit 2
cd "$1" || exit 3
git init || exit 4

find ./ -type f -exec sed -i -e 's/myproj/'$MY_PROJ_NAME'/g' {} \; || exit 5

for f in $(find ./ -type f -name "*myproj*" -exec echo {} \;); do
	mv $f ${f/myproj/$MY_PROJ_NAME} || exit 6
done

echo ""
echo "###########################################################"
echo "Project $MY_PROJ_NAME was creates successfully at $1"
echo "###########################################################"
