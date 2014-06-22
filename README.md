About
-----

  - A counter is incremented 
	- A radio message is sent whenever a timer fires
  - Whenever a radio message is received, 
		 the three LSB bits of the counter payload are displayed on the LEDs 
  - As long as the motes are within range of each other, the LEDs on both will keep changing  

Compilation
-----------

	$ make telosb
	# replace telosb with any other mote

Burn Hex
--------

	$ make telosb install.1
	# replace 1 with Node ID

Simulation
----------

	- Open Cooja Simulator
	- New Simulation (or open Simulation -> simulation/basic_multihop_sim.csc)
	- Motes -> Add Motes -> Create Mote -> Sky Mote -> Browse -> main.exe
		# find main.exe from build/telosb/main.exe
	- Simulation Control -> start
	- Move nodes around and observe connectivity

Results
-------

	- screenshots of simulation and real time images are available in the screenshots/ folder


