#!/usr/bin/bash
echo "### Installing Lexer/Parser ..."

git submodule update --init --recursive
cd external/xion
poetry install

echo "### Saving Lexer/Parser site-packages ..."

path=$(poetry env info --path)
version=$(poetry run python --version | cut -f2 -d " " | cut -f1,2 -d "." )

mkdir -p build

echo "$path/lib/python$version/site-packages" > .site-packages
cp .site-packages ../../build

echo "Done."

