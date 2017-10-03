A base project where I can try to implement random things I feel like trying.

Current: Pathfinder

Generates a random 2d mapgrid somewhere between 100 and 10000 in size.

The program generates the map and start and end possition.

If no path is found the program sets the title to negative duration
otherwise the time it took for the path to be found is displayed.

The pathfinding auto quits if the calculated shortest path is longer than 
the output buffer.

The path finding also exists if the time is closing in on 5 sekonds.

The pathfinding itself is a gready A* aproach using vector lists.
In the runs I have done the memmory have never been a problem for the algorithm.

Controls: Staggered "camera" movement.

WASD to move upp,down,left,right

RF to increase/ decrease jump speed

TG zoom in/ out

YH increase/ decrease zoom speed

N to grnerate new map and path