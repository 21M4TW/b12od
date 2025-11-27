#!/bin/sh

for i in `cat offer_strings`
do
    echo "$i"
    ../test_bolt12_offer "$i"
done
