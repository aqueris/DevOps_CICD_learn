#!/usr/bin/bash
# move
ls
mv s21_cat /usr/local/bin/
mv s21_grep /usr/local/bin/

#check
ls /usr/local/bin/
if [ ! -f /usr/local/bin/s21_cat ] || [ ! -f /usr/local/bin/s21_grep ]; then echo "Deploy failed!"; exit 1; fi
echo "Deploy completed successfully!"