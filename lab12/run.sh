export PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/:/mnt/c/Users/82454/Desktop/workspace/CS110P/lab12/env/hadoop-3.2.1/bin/:/mnt/c/Users/82454/Desktop/workspace/CS110P/lab12/env/spark-3.1.1-bin-hadoop2.7/bin/
echo "\033[32mExercise 0 begin\033[0m"
make generate-input myinput=65469-0.txt
echo "\033[32mExercise 0 done\033[0m"
echo "\033[32mExercise 1 begin\033[0m"
echo "\033[32mExercise 1 billOfRights\033[0m"
spark-submit wordCount.py seqFiles/billOfRights.txt.seq
echo "\033[32mExercise 1 complete-works-mark-twain\033[0m"
spark-submit wordCount.py seqFiles/complete-works-mark-twain.txt.seq
echo "\033[32mExercise 1 done\033[0m"
echo "\033[32mExercise 2 begin\033[0m"
spark-submit perWordDocumentCount.py seqFiles/billOfRights.txt.seq
echo "\033[32mExercise 2 done\033[0m"
echo "\033[32mExercise 3 begin\033[0m"
spark-submit createIndices.py seqFiles/billOfRights.txt.seq
echo "\033[32mExercise 3 done\033[0m"
echo "\033[32mExercise 4 begin\033[0m"
spark-submit mostPopular.py seqFiles/billOfRights.txt.seq
echo "\033[32mExercise 4 done\033[0m"
