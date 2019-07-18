#ifndef WT_PROCESS_H
#define WT_PROCESS_H


#include <math.h>
#include <lapacke.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <jsoncpp/json/json.h>



//TODO add sensor noise?

class WT {
        struct timeval last_update, current; 
        //parameters


        //state variables
        double levelsArray [9];		//Create an array of levels
        double levelsArray_peri [6];//Create an array of levels
        bool saturatedFlag [9];		//Checks if the containers are saturated *** Wings are also added

        double level_1;				//Percent of water level in container
        double level_2;				//Percent of water level in container
        double level_3;				//Percent of water level in container
        double level_4;				//Percent of water level in container
        double level_5;				//Percent of water level in container
        double level_6;				//Percent of water level in container
        double level_wing_in;       //Percent of water level in the input wing
        double level_wing_out_1;    //Percent of water level in the output wing
        double level_wing_out_2;    //Percent of water level in the output wing

        double level_1_peri;		//Percent of water level in container's perimiter (initially zero)
        double level_2_peri;		//Percent of water level in container's perimiter (initially zero)
        double level_3_peri;		//Percent of water level in container's perimiter (initially zero)
        double level_4_peri;		//Percent of water level in container's perimiter (initially zero)
        double level_5_peri;		//Percent of water level in container's perimiter (initially zero)
        double level_6_peri;		//Percent of water level in container's perimiter (initially zero)

        double polution_1;			//Polution level in container
        double polution_2;			//Polution level in container
        double polution_3;			//Polution level in container
        double polution_4;			//Polution level in container
        double polution_5;			//Polution level in container
        double polution_6;			//Polution level in container
        double polution_in;			//Polution level of ingoing water
        double polution_out;		//Polution level of output water
        double polutionArray[8];     //Array of Polutions
      
        
        //outputs


		//Water Treatment Values
		double water_in;			//amount of water that gets in
		double water_out;			//amount of water gets out
		
		//Valves
		double valvesArray_in [7];	//Create an array of valves for inputs
		double valvesArray_out [7];	//Create an array of levels for outputs

		double valveFlowLimit;		//Limit of each container valve m^3/sec

		double valve_in;
		double valve_out;
		double valve_1_in;				
		double valve_2_in;
		double valve_3_in;
		double valve_4_in;
		double valve_5_in;
		double valve_6_in;

		double valve_1_out;				
		double valve_2_out;
		double valve_3_out;
		double valve_4_out;
		double valve_5_out;
		double valve_6_out;

		//Chemicals
		double chemsArray[6];		//Create an array of chemicals added to water containers

    public:

    	WT();
        void steady_state();
        void set_initial_values(Json::Value inputs);
        void initialize_state_variables();
        void process_values();
        void print_outputs();
        Json::Value get_state_json();

        double change_level_container(int i);
        double change_level_container_peri(int i);
        void set_level(int i);
        double flow_inside_container(int i, double water_push, double water_pull, double currentWaterLevel, double currentWaterLevel_peri);

        void check_balance(double valve_in, double valve_out);
        void check_saturation(int i);
        void check_flow_balance();
        void state_check(int i);

        void set_polution_level(int i);
        double set_wing_level();
        
};



#endif
