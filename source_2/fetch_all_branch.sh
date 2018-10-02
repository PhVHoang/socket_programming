#!/bin/bash
for branch in `git branch -a | grep remotes | grep -v HEAD | grep -v master `; do
    git branch --track ${branch#remotes/origin/} $branch
done

for branch in `git branch `; do
    git checkout $branch
    git pull
done
