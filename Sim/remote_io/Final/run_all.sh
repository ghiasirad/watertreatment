#!/bin/bash

sudo pkill python
sudo python Valve_in.py &
sudo python Valve_out.py &
sudo python Valve_1_in.py &
sudo python Valve_1_out.py &
sudo python Valve_2_in.py &
sudo python Valve_2_out.py &
sudo python Valve_3_in.py &
sudo python Valve_3_out.py &
sudo python Valve_4_in.py &
sudo python Valve_4_out.py &
sudo python Valve_5_in.py &
sudo python Valve_5_out.py &
sudo python Valve_6_in.py &
sudo python Valve_6_out.py &
sudo python analyzer.py &
