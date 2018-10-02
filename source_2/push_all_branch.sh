#!/bin/bash
for branch in `git branch `; do
    git checkout $branch
    git push origin $branch
done
