GCC=g++ -O3
OBJS=HorizontalTreeMiner.o CoreList.o FrequentTreeList.o TupleTree.o CanonicalTree.o FreeTree.o Bipartite.o
FreeTreeMiner: HorizontalTreeMiner.o CoreList.o FrequentTreeList.o TupleTree.o CanonicalTree.o FreeTree.o Bipartite.o
	${GCC} -o FreeTreeMiner HorizontalTreeMiner.o CoreList.o FrequentTreeList.o TupleTree.o CanonicalTree.o FreeTree.o Bipartite.o;
	rm -f ${OBJS};
HorizontalTreeMiner.o: HorizontalTreeMiner.cpp CoreList.h FrequentTreeList.h TupleTree.h CanonicalTree.h FreeTree.h Bipartite.h misc.h
	${GCC} -c HorizontalTreeMiner.cpp
CoreList.o: CoreList.cpp CoreList.h FrequentTreeList.h CanonicalTree.h FreeTree.h misc.h
	${GCC} -c CoreList.cpp
FrequentTreeList.o: FrequentTreeList.cpp FrequentTreeList.h TupleTree.h CanonicalTree.h FreeTree.h misc.h
	${GCC} -c FrequentTreeList.cpp 
TupleTree.o: TupleTree.cpp TupleTree.h FreeTree.h Bipartite.h misc.h
	${GCC} -c TupleTree.cpp
CanonicalTree.o: CanonicalTree.cpp CanonicalTree.h FreeTree.h misc.h
	${GCC} -c CanonicalTree.cpp
FreeTree.o: FreeTree.cpp FreeTree.h misc.h
	${GCC} -c FreeTree.cpp
Bipartite.o: Bipartite.cpp Bipartite.h misc.h
	${GCC} -c Bipartite.cpp
