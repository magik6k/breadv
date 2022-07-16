#!/usr/bin/env bash

curl http://127.0.0.1:13333/dump/$(ls -l text | awk '{print $5}') > /tmp/mem

hexdump -C text > /tmp/a
hexdump -C /tmp/mem > /tmp/b

diff -u --color /tmp/a /tmp/b
