#!/bin/bash

source proverki.sh

sudo goaccess $logpath --log-format=COMBINED --ignore-panel=REQUESTS_STATIC --ignore-panel=REFERRING_SITES --ignore-panel=KEYPHRASES --ignore-panel=GEO_LOCATION > goaccess.html
echo создали файл goaccess.html всё там

    

