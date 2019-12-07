CXX = g++
CXXFLAGS = -Wall -pedantic -Wno-long-long -O0 -ggdb -std=c++11
LD = g++
LDFLAGS = -Wall -pedantic -Wno-long-long -O0 -ggdb -std=c++11

compile: pacman

pacman: main.o pacman.o map.o tiles.o ghosts.o 
	$(LD) $(LDFLAGS) main.o pacman.o map.o tiles.o ghosts.o -o pacman -lncurses

ghosts.o: ghosts.cpp ghosts.h map.h tiles.h pacman.h
	$(CXX) $(CXXFLAGS) -c $< -o $@
main.o: main.cpp map.h tiles.h pacman.h ghosts.h
	$(CXX) $(CXXFLAGS) -c $< -o $@
map.o: map.cpp map.h tiles.h
	$(CXX) $(CXXFLAGS) -c $< -o $@
pacman.o: pacman.cpp pacman.h map.h tiles.h
	$(CXX) $(CXXFLAGS) -c $< -o $@
tiles.o: tiles.cpp tiles.h pacman.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm *.o
run:
	./pacman ${MAKECMDGOALS}

#automatically gen



