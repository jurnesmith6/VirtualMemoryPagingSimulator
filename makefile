# Jurne Smith
# Red ID: 824190049
# Prof. Shen
# CS480 - Operating Systems
# 11/1/25

# compiler
CXX = g++
CXXFLAGS = -std=c++11 -Wall -g3 -c

# object files
OBJS = main.o PageTable.o PageTracker.o log_helpers.o vaddr_tracereader.o PageNode.o

# Program name
PROGRAM = pagingwithpr

$(PROGRAM) : $(OBJS)
	$(CXX) -o $(PROGRAM) $^

PageTable.o : PageTable.h PageTable.cpp PageNode.h
	$(CXX) $(CXXFLAGS) PageTable.cpp

PageNode.o : PageNode.h PageNode.cpp
	$(CXX) $(CXXFLAGS) PageNode.cpp

PageTracker.o : PageTracker.h PageTracker.cpp
	$(CXX) $(CXXFLAGS) PageTracker.cpp

main.o : main.cpp PageTable.h PageTracker.h vaddr_tracereader.h log_helpers.h
	$(CXX) $(CXXFLAGS) main.cpp

vaddr_tracereader.o : vaddr_tracereader.h vaddr_tracereader.cpp
	$(CXX) $(CXXFLAGS) vaddr_tracereader.cpp

log_helpers.o : log_helpers.h log_helpers.cpp
	$(CXX) $(CXXFLAGS) log_helpers.cpp

clean:
	rm -f *.o $(PROGRAM)
