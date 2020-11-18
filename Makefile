CXX = g++ 
CXXFLAGS += -std=c++11 

dissem : dissem.o 
	$(CXX) dissem.o -o dissem

dissem.o : dissem.cpp
	$(CXX) $(CXXFLAGS) -c dissem.cpp

clean :
	rm *.o
