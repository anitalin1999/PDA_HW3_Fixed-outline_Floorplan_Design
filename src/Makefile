CXX := g++
CXXFLAGS := -std=c++11 -O3 -Wall -Wextra -g
LIBS := -lm
EXEC := ../bin/hw3
SRCS := hw3.cpp Data.hpp Parse.hpp SA.hpp Tree.hpp GlobalTimer.hpp

all: $(EXEC)

$(EXEC): $(SRCS)
		$(CXX) $(CXXFLAGS) $(LIBS) $< -o $@

clean:
		rm -f $(EXEC)

ifeq (test, $(firstword $(MAKECMDGOALS)))
  TESTCASE_NAME := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(TESTCASE_NAME):;@:)
endif

test: $(EXEC)
		@echo test on $(TESTCASE_NAME) 0.15
		./$(EXEC) ../testcases/$(TESTCASE_NAME).hardblocks ../testcases/$(TESTCASE_NAME).nets ../testcases/$(TESTCASE_NAME).pl ../output/$(TESTCASE_NAME).floorplan 0.15
		../verifier/verifier ../testcases/$(TESTCASE_NAME).hardblocks ../testcases/$(TESTCASE_NAME).nets ../testcases/$(TESTCASE_NAME).pl ../output/$(TESTCASE_NAME).floorplan 0.15
.PHONY: all clean test
