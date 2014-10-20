Light Display
============

This controls LED light displays as part of *Beach Pong*, an augmented reality version of Pong, and an installation at Nocturne Halifax  2014.

Code under *Arduino* controls a grid of LED lights. One is a simple row/column single color LED grid, the other are Britestar Symphony of Lights tri-color LED christmas lights, which use a simple packet protocol.

Code under *Controllers* receives messages on a virtual Pong ball position and controls the Arduino-connected lights via serial. The Britestar controller in particular can be adapted to drive more complex light patterns.