#!/bin/bash

TESTCASE_NAME="n100"
for i in {1..100};do
    ./../bin/hw3 ../testcases/${TESTCASE_NAME}.hardblocks ../testcases/${TESTCASE_NAME}.nets ../testcases/${TESTCASE_NAME}.pl ../output/${TESTCASE_NAME}.floorplan 0.15 $i >> ${TESTCASE_NAME}.csv
done


TESTCASE_NAME="n200"
for i in {1..100};do
    ./../bin/hw3 ../testcases/${TESTCASE_NAME}.hardblocks ../testcases/${TESTCASE_NAME}.nets ../testcases/${TESTCASE_NAME}.pl ../output/${TESTCASE_NAME}.floorplan 0.15 $i >> ${TESTCASE_NAME}.csv
done

TESTCASE_NAME="n300"
for i in {1..100};do
    ./../bin/hw3 ../testcases/${TESTCASE_NAME}.hardblocks ../testcases/${TESTCASE_NAME}.nets ../testcases/${TESTCASE_NAME}.pl ../output/${TESTCASE_NAME}.floorplan 0.15 $i >> ${TESTCASE_NAME}.csv
done