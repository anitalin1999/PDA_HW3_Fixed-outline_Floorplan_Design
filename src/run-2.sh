#!/bin/bash

# TESTCASE_NAME="n100"
# for i in {1..1000};do
#     ./../bin/hw3 ../testcases/${TESTCASE_NAME}.hardblocks ../testcases/${TESTCASE_NAME}.nets ../testcases/${TESTCASE_NAME}.pl ../output/${TESTCASE_NAME}.floorplan 0.1 $i >> ${TESTCASE_NAME}_1.csv
# done


# TESTCASE_NAME="n200"
# for i in {1..1000};do
#     ./../bin/hw3 ../testcases/${TESTCASE_NAME}.hardblocks ../testcases/${TESTCASE_NAME}.nets ../testcases/${TESTCASE_NAME}.pl ../output/${TESTCASE_NAME}.floorplan 0.1 $i >> ${TESTCASE_NAME}_1.csv
# done

TESTCASE_NAME="n300"
for i in {1..1000};do
    ./../bin/hw3 ../testcases/${TESTCASE_NAME}.hardblocks ../testcases/${TESTCASE_NAME}.nets ../testcases/${TESTCASE_NAME}.pl ../output/${TESTCASE_NAME}.floorplan 0.1 $i >> ${TESTCASE_NAME}_1.csv
done