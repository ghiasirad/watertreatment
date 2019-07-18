#include "WT_process.h"

#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>
#include <iostream>
#include <ctime>
#include <time.h>
#include <math.h>


using namespace std;

double containerBase_in = 40, containerBase_out = 30, containerHight = 3.99, containerHight_peri = 2.99, wingHight = 3.99, wingBase = 80;	//Each of 6 container volume in m^2

double stepSize = 1, sumValves = 0, sumValves_out = 0;

WT::WT() {
    gettimeofday(&current, NULL);
    last_update = current;
    
}

void WT::set_initial_values(Json::Value inputs) {

    //define temp variables to use as a medium for getting data from the input json

    double value_valve_1_in = valve_1_in;
    double value_valve_2_in = valve_2_in;
    double value_valve_3_in = valve_3_in;
    double value_valve_4_in = valve_4_in;
    double value_valve_5_in = valve_5_in;
    double value_valve_6_in = valve_6_in;

    double value_valve_1_out = valve_1_out;
    double value_valve_2_out = valve_2_out;
    double value_valve_3_out = valve_3_out;
    double value_valve_4_out = valve_4_out;
    double value_valve_5_out = valve_5_out;
    double value_valve_6_out = valve_6_out;


    double value_valve_in = valve_in;
    double value_valve_out = valve_out;

    double value_chem_in = polution_in;


    //read values from json input

    //Read Main Valves
    if (inputs["inputs"].isMember("valveValuesMain_sp")) {
        value_valve_in = inputs["inputs"]["valveValuesMain_sp"]["value_valve_in"].asDouble();
        value_valve_out = inputs["inputs"]["valveValuesMain_sp"]["value_valve_out"].asDouble();
    }

    //Chemicals
    if (inputs["inputs"].isMember("valveValuesMain_sp")) {
        value_chem_in = inputs["inputs"]["valveValuesMain_sp"]["value_chem_in"].asDouble();
    }

    //Modified Valves
    if (inputs["inputs"].isMember("valveValues1_sp")) {
        value_valve_1_in = inputs["inputs"]["valveValues1_sp"]["value_valve_in"].asDouble();
        value_valve_1_out = inputs["inputs"]["valveValues1_sp"]["value_valve_out"].asDouble();
    }

    if (inputs["inputs"].isMember("valveValues2_sp")) {
        value_valve_2_in = inputs["inputs"]["valveValues2_sp"]["value_valve_in"].asDouble();
        value_valve_2_out = inputs["inputs"]["valveValues2_sp"]["value_valve_out"].asDouble();
    }

    if (inputs["inputs"].isMember("valveValues3_sp")) {
        value_valve_3_in = inputs["inputs"]["valveValues3_sp"]["value_valve_in"].asDouble();
        value_valve_3_out = inputs["inputs"]["valveValues3_sp"]["value_valve_out"].asDouble();
    }

    if (inputs["inputs"].isMember("valveValues4_sp")) {
        value_valve_4_in = inputs["inputs"]["valveValues4_sp"]["value_valve_in"].asDouble();
        value_valve_4_out = inputs["inputs"]["valveValues4_sp"]["value_valve_out"].asDouble();
    }

    if (inputs["inputs"].isMember("valveValues5_sp")) {
        value_valve_5_in = inputs["inputs"]["valveValues5_sp"]["value_valve_in"].asDouble();
        value_valve_5_out = inputs["inputs"]["valveValues5_sp"]["value_valve_out"].asDouble();
    }

    if (inputs["inputs"].isMember("valveValues6_sp")) {
        value_valve_6_in = inputs["inputs"]["valveValues6_sp"]["value_valve_in"].asDouble();
        value_valve_6_out = inputs["inputs"]["valveValues6_sp"]["value_valve_out"].asDouble();
    }

    //put values in variables to be stored and be worked on
    /*water_in = value_water_in;
    water_out = value_water_out;*/

    valve_1_in = value_valve_1_in;
    valve_2_in = value_valve_2_in;
    valve_3_in = value_valve_3_in;
    valve_4_in = value_valve_4_in;
    valve_5_in = value_valve_5_in;
    valve_6_in = value_valve_6_in;
    valve_in = value_valve_in;
    valve_out = value_valve_out;

    valve_1_out = value_valve_1_out;
    valve_2_out = value_valve_2_out;
    valve_3_out = value_valve_3_out;
    valve_4_out = value_valve_4_out;
    valve_5_out = value_valve_5_out;
    valve_6_out = value_valve_6_out;

    valvesArray_in[0] = valve_1_in;
    valvesArray_in[1] = valve_2_in;
    valvesArray_in[2] = valve_3_in;
    valvesArray_in[3] = valve_4_in;
    valvesArray_in[4] = valve_5_in;
    valvesArray_in[5] = valve_6_in;
    valvesArray_in[6] = valve_in;

    valvesArray_out[0] = valve_1_out;
    valvesArray_out[1] = valve_2_out;
    valvesArray_out[2] = valve_3_out;
    valvesArray_out[3] = valve_4_out;
    valvesArray_out[4] = valve_5_out;
    valvesArray_out[5] = valve_6_out;
    valvesArray_out[6] = valve_out;



    //Chemicals added to each container g/m^3
    polution_in = value_chem_in;

    //Randomization was considered in the first place
    polutionArray[6] = (100 * (polution_in * 0.9) + (rand() %10))/100;

    /*for (int i = 0 ; i < 6 ; i++){
    	saturatedFlag[i] = false;
    }*/

}

//Initializes the state variable which are not set by the user
void WT::initialize_state_variables(){

	/*//Input and Output Water before random factor
	water_in = 100;
	water_out = 100;*/

	//Set limit of the container valves
	valveFlowLimit = 10;

	//Instantiate levels
    level_1 = 3;
    level_2 = 3;
    level_3 = 3;
    level_4 = 3;
    level_5 = 3;
    level_6 = 0;
    level_1_peri = 2;
    level_2_peri = 2;
    level_3_peri = 2;
    level_4_peri = 2;
    level_5_peri = 2;
    level_6_peri = 0;

    level_wing_in = 2;
    level_wing_out_1 = 2;
    level_wing_out_2 = 2;

    //Inner levels
    levelsArray[0] = level_1;
    levelsArray[1] = level_2;
    levelsArray[2] = level_3;
    levelsArray[3] = level_4;
    levelsArray[4] = level_5;
    levelsArray[5] = level_6;
    levelsArray[6] = level_wing_in;
    levelsArray[7] = level_wing_out_1;
    levelsArray[8] = level_wing_out_2;

    levelsArray_peri[0] = level_1_peri;
    levelsArray_peri[1] = level_2_peri;
    levelsArray_peri[2] = level_3_peri;
    levelsArray_peri[3] = level_4_peri;
    levelsArray_peri[4] = level_5_peri;
    levelsArray_peri[5] = level_6_peri;



    /*//Instantiate polutions
    polution_1 = 0.20;
    polution_2 = 0.21;
    polution_3 = 0.22;
    polution_4 = 0.23;
    polution_5 = 0.24;
    polution_6 = 0.25;
    polution_in = 0.12;
    polution_out = 0.28;*/

    /*polutionArray[0] = polution_1;
    polutionArray[1] = polution_2;
    polutionArray[2] = polution_3;
    polutionArray[3] = polution_4;
    polutionArray[4] = polution_5;
    polutionArray[5] = polution_6;
    polutionArray[6] = polution_in;
    polutionArray[7] = polution_out;*/
}

void WT::print_outputs() {

	printf("\n*******************************************");
	//printf("\nStep Size: %f",stepSize);

    printf("\nwater_in = %.2f", water_in);
    printf("\t\tvalve_in = %.2f", valvesArray_in[6]);
    printf("\t\tPolution_in = %.2f",polutionArray[6]);
    printf("\nwater_out = %.2f", water_out);
    printf("\tvalve_out = %.2f", valvesArray_out[6]);
    printf("\tPolution_out = %.2f\n",polutionArray[7]);

    printf("\nvalve_1_in = %.2f", valvesArray_in[0]);
    printf("\tvalve_2_in = %.2f", valvesArray_in[1]);
    printf("\tvalve_3_in = %.2f", valvesArray_in[2]);
    printf("\tvalve_4_in = %.2f", valvesArray_in[3]);
    printf("\tvalve_5_in = %.2f", valvesArray_in[4]);
    printf("\tvalve_6_in = %.2f", valvesArray_in[5]);

    printf("\nvalve_1_out = %.2f", valvesArray_out[0]);
    printf("\tvalve_2_out = %.2f", valvesArray_out[1]);
    printf("\tvalve_3_out = %.2f", valvesArray_out[2]);
    printf("\tvalve_4_out = %.2f", valvesArray_out[3]);
    printf("\tvalve_5_out = %.2f", valvesArray_out[4]);
    printf("\tvalve_6_out = %.2f\n", valvesArray_out[5]);

    printf("\nlevel_1 = %.2f",levelsArray[0]);
    printf("\tlevel_1_peri = %.2f",levelsArray_peri[0]);
    printf("\tpolution_1 = %.2f",polutionArray[0]);
    printf("\nlevel_2 = %.2f",levelsArray[1]);
    printf("\tlevel_2_peri = %.2f",levelsArray_peri[1]);
    printf("\tpolution_2 = %.2f",polutionArray[1]);
    printf("\nlevel_3 = %.2f",levelsArray[2]);
    printf("\tlevel_3_peri = %.2f",levelsArray_peri[2]);
    printf("\tpolution_3 = %.2f",polutionArray[2]);
    printf("\nlevel_4 = %.2f",levelsArray[3]);
    printf("\tlevel_4_peri = %.2f",levelsArray_peri[3]);
    printf("\tpolution_4 = %.2f",polutionArray[3]);
    printf("\nlevel_5 = %.2f",levelsArray[4]);
    printf("\tlevel_5_peri = %.2f",levelsArray_peri[4]);
    printf("\tpolution_5 = %.2f",polutionArray[4]);
    printf("\nlevel_6 = %.2f",levelsArray[5]);
    printf("\tlevel_6_peri = %.2f",levelsArray_peri[5]);
    printf("\tpolution_6 = %.2f\n",polutionArray[5]);

    printf("\nlevel_wing_in = %.2f",levelsArray[6]);
    printf("\nlevel_wing_out_1 = %.2f",levelsArray[7]);
    printf("\nlevel_wing_out_2 = %.2f",levelsArray[8]);

    printf("\n===========================================\n");

}


Json::Value WT::get_state_json() {
    Json::Value state;
    state["process"] = "WaterTreatmentFacility";

    state["inputs"]["water_in"] = water_in;
    state["inputs"]["water_out"] = water_out;

    state["inputs"]["valve_1_in"] = valvesArray_in[0];
    state["inputs"]["valve_2_in"] = valvesArray_in[1];
    state["inputs"]["valve_3_in"] = valvesArray_in[2];
    state["inputs"]["valve_4_in"] = valvesArray_in[3];
    state["inputs"]["valve_5_in"] = valvesArray_in[4];
    state["inputs"]["valve_6_in"] = valvesArray_in[5];
    state["inputs"]["valve_in"] = valvesArray_in[6];

    state["inputs"]["valve_1_out"] = valvesArray_out[0];
    state["inputs"]["valve_2_out"] = valvesArray_out[1];
    state["inputs"]["valve_3_out"] = valvesArray_out[2];
    state["inputs"]["valve_4_out"] = valvesArray_out[3];
    state["inputs"]["valve_5_out"] = valvesArray_out[4];
    state["inputs"]["valve_6_out"] = valvesArray_out[5];
    state["inputs"]["valve_out"] = valvesArray_out[6];

    state["outputs"]["level_1"] = levelsArray[0];
    state["outputs"]["level_2"] = levelsArray[1];
    state["outputs"]["level_3"] = levelsArray[2];
    state["outputs"]["level_4"] = levelsArray[3];
    state["outputs"]["level_5"] = levelsArray[4];
    state["outputs"]["level_6"] = levelsArray[5];
    state["outputs"]["level_wing_in"] = levelsArray[6];
    state["outputs"]["level_wing_out_1"] = levelsArray[7];
    state["outputs"]["level_wing_out_2"] = levelsArray[8];
    state["outputs"]["level_1_peri"] = levelsArray_peri[0];
    state["outputs"]["level_2_peri"] = levelsArray_peri[1];
    state["outputs"]["level_3_peri"] = levelsArray_peri[2];
    state["outputs"]["level_4_peri"] = levelsArray_peri[3];
    state["outputs"]["level_5_peri"] = levelsArray_peri[4];
    state["outputs"]["level_6_peri"] = levelsArray_peri[5];

    state["outputs"]["polution_1"] = polutionArray[0];
    state["outputs"]["polution_2"] = polutionArray[1];
    state["outputs"]["polution_3"] = polutionArray[2];
    state["outputs"]["polution_4"] = polutionArray[3];
    state["outputs"]["polution_5"] = polutionArray[4];
    state["outputs"]["polution_6"] = polutionArray[5];
    state["outputs"]["polution_in"] = polutionArray[6];
    state["outputs"]["polution_out"] = polutionArray[7];

    /*state["outputs"]["saturation_1"] = saturatedFlag[0];
    state["outputs"]["saturation_2"] = saturatedFlag[1];
    state["outputs"]["saturation_3"] = saturatedFlag[2];
    state["outputs"]["saturation_4"] = saturatedFlag[3];
    state["outputs"]["saturation_5"] = saturatedFlag[4];
    state["outputs"]["saturation_6"] = saturatedFlag[5];
    state["outputs"]["saturation_wing_in"] = saturatedFlag[6];
    state["outputs"]["saturation_1_wing_out_1"] = saturatedFlag[7];
    state["outputs"]["saturation_1_wing_out_2"] = saturatedFlag[8];*/
    
    return state;

}

// Simulation is done here

void WT::process_values(){

    //Consider random factor
    water_in = double(90.0 + (rand() %10))/10;
    water_out = double(90.0 + (rand() %10))/10;

	//check_balance();

    sumValves = 0;
	for (int j = 0 ; j < 6 ; j++){
		sumValves = sumValves + valvesArray_in[j];
	}

	//Set levels of the containers

    double tempOutPolution = 0;
	for (int i = 0 ; i < 6 ; i++){
		set_level(i);
        set_polution_level(i);
        tempOutPolution = tempOutPolution + polutionArray[i];
		state_check(i);
	}

    //Counting Active Containers for Polution Calculations
    double totalActiveContainers = 0;
    for (int i = 0 ; i < 6 ; i++){
        if (valvesArray_in[i] > 0){
            totalActiveContainers +=1;
        }
    }
    polutionArray[7] = tempOutPolution/totalActiveContainers;

    //For the Wings
    set_wing_level();
    for (int i = 0 ; i < 3 ; i++){
	   check_saturation(i+6);
    }

    //See if the security constraint for input water holds
    check_flow_balance();

    //cout << get_state_json() << endl;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Set levels for each container
void WT::set_level(int i){
	double change, change_peri, level = levelsArray[i], level_peri = levelsArray_peri[i];

	change = change_level_container(i);

	//The main container
	level = change + level;
	if (level > containerHight){
		level = containerHight + 0.01;
	} else if (level < 0){
        level = 0;
    }

	//The perimiter of the main container
	change_peri = change_level_container_peri(i);
	level_peri = change_peri + level_peri;
	if (level_peri > containerHight_peri){
		level_peri = containerHight_peri + 0.01;
		saturatedFlag[i] = true;
		//std::cout << "Danger in container: "<< i+1 << endl;
	} else if (level_peri < 0){
        level_peri = 0;
        saturatedFlag[i] = false;
    } else{
		saturatedFlag[i] = false;
	}

	//Need to define the water between inner and outer containers

	//Set the levels
	levelsArray[i] = level;
	levelsArray_peri[i] = level_peri;
	return;
}

double WT::change_level_container(int i){ // Change in level_peri per stepSize (1 second)

	double changeLevel = 0 , flowInside;

	//Instantiate the inside flow
	flowInside = flow_inside_container(i, valvesArray_in[i]*valveFlowLimit, valvesArray_out[6]*valveFlowLimit, levelsArray[i], levelsArray_peri[i]);

	if (flowInside == 0 && levelsArray[i] <= containerHight && water_in*valvesArray_in[6] > 0 && levelsArray[6] > 0){
		changeLevel = valvesArray_in[i]*valveFlowLimit/(sumValves * containerBase_in);
	}else if (flowInside != 0 && levelsArray[i] > containerHight){
		changeLevel = 0;
	}

	return changeLevel;
}

double WT::change_level_container_peri(int i){
	double changeLevel_peri = 0, flowInside;

	//Instantiate the inside flow
	flowInside = flow_inside_container(i, valvesArray_in[i]*valveFlowLimit, valvesArray_out[6]*valveFlowLimit, levelsArray[i], levelsArray_peri[i]);

	if (!flowInside == 0 && levelsArray_peri[i] <= containerHight_peri){
		changeLevel_peri = (flowInside - valvesArray_out[i]*valveFlowLimit)/containerBase_out;
	}


	return changeLevel_peri;
}

//Checks the flow of water between the inner container and the perimiter
double WT::flow_inside_container(int i, double water_push, double water_pull, double currentWaterLevel, double currentWaterLevel_peri){
	double flowInsideContainer = 0;

	//Consider different scenarios
	if (water_push > 0 && currentWaterLevel < containerHight){
		flowInsideContainer = 0;
	}else if (water_push > 0 && currentWaterLevel > containerHight && currentWaterLevel_peri < containerHight_peri){
		flowInsideContainer = water_push;
	}else if (water_push > 0 && currentWaterLevel > containerHight && currentWaterLevel_peri > containerHight_peri){ // A trick, so the current is not zero, but the action is the same
		flowInsideContainer = 0;
	}else{
		flowInsideContainer = 0;
	}

	return flowInsideContainer;
}

double WT::set_wing_level(){

    //for the water getting in and out from each wing
    double sumIn = 0, sumOut_1 = 0, sumOut_2 = 0;
    double sumValves_out_1 = 0, sumValves_out_2 = 0;
    sumValves_out = 0;

    for (int i = 0 ; i < 6 ; i++){
        sumIn = sumIn + valvesArray_in[i]*valveFlowLimit;
        sumValves_out = sumValves_out + valvesArray_out[i];
        if (i < 3 && levelsArray_peri[i] > 0){
            sumOut_1 = sumOut_1 + valvesArray_out[i]*valveFlowLimit;
            sumValves_out_1 = sumValves_out_1 + valvesArray_out[i];
        }
        else if (i >= 3 && levelsArray_peri[i] > 0){
            sumOut_2 = sumOut_2 + valvesArray_out[i]*valveFlowLimit;
            sumValves_out_2 = sumValves_out_2 + valvesArray_out[i];
        }
    }

    //Calculate the levels

    //Inner wing
    levelsArray[6] = levelsArray[6] + (water_in*valvesArray_in[6] - sumIn)/wingBase;
    if (levelsArray[6] < 0){
        saturatedFlag[6] = false;
        levelsArray[6] = 0;
    } else if (levelsArray[6] > wingHight){
        levelsArray[6] = wingHight + 0.01;
        saturatedFlag[6] = true;
    } else{
        saturatedFlag[6] = false;
    }

    //Outer wings
    levelsArray[7] = levelsArray[7] + (sumOut_1 - water_out*valvesArray_out[6]*sumValves_out_1/sumValves_out)/wingBase;
    if (levelsArray[7] < 0){
        levelsArray[7] = 0;
        saturatedFlag[7] = false;
    } else if (levelsArray[7] > wingHight){
        levelsArray[7] = wingHight + 0.01;
        saturatedFlag[7] = true;
    } else{
        saturatedFlag[7] = false;
    }

    levelsArray[8] = levelsArray[8] + (sumOut_2 - water_out*valvesArray_out[6]*sumValves_out_2/sumValves_out)/wingBase;
    if (levelsArray[8] < 0){
        levelsArray[8] = 0;
        saturatedFlag[8] = false;
    } else if (levelsArray[8] > wingHight){
        levelsArray[8] = wingHight + 0.01;
        saturatedFlag[8] = true;
    } else{
        saturatedFlag[8] = false;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Set polution level for each container
void WT::set_polution_level(int i){
    //Need to set a base for polution reduction
    //Consider the average of valve levels for the input and output
    polutionArray[i] = -exp (-(valvesArray_in[i]+valvesArray_out[i])/2)*polutionArray[6] + polutionArray[6];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Security Constraints UI
void WT::check_balance(double valve_in, double valve_out){
	double balance = 0;

	balance = valve_in - valve_out;

	if (balance < 0){
		printf("- Balance is negative!\n");
	}else if (balance > 0){
		printf("- Balance is positive!\n");
	}
}

void WT::check_saturation(int i){
	if (saturatedFlag[i]){
        if (i < 6){
		  printf("- Over flown!!!");
        } else if (i == 6){
            printf("\n----------------------\n");
            printf("- Overflown in input Wing");
        } else if (i == 7){
            printf("\n----------------------\n");
            printf("- Overflown in upper output Wing");
        } else if (i == 8){
            printf("\n----------------------\n");
            printf("- Overflown in lower output Wing");
        }
	}
}

void WT::check_flow_balance(){

	double valveInputFlowLimitSum = 0, valveOutputFlowLimitSum = 0;

	//Input Balance Flow
	for (int j = 0 ; j < 6 ; j++){
		valveInputFlowLimitSum = valveInputFlowLimitSum + valvesArray_in[j]*valveFlowLimit;
	}

	if (valveInputFlowLimitSum != water_in*valvesArray_in[6]){
		printf("\nNo balance for the input water valves!");
	}

	//Output Balance Flow
	for (int j = 0 ; j < 6 ; j++){
		valveOutputFlowLimitSum = valveOutputFlowLimitSum + valvesArray_out[j]*valveFlowLimit;
	}

	if (valveOutputFlowLimitSum != water_out*valvesArray_out[6]){
		printf("\nNo balance for the output water valves!\n");
	}
}

void WT::state_check(int i){

	printf("\n----------------------\n");

	if (i > 5){
		//printf("Check the state for the main stream: \n");
	}else{
		printf("Check the state for container %i: \n",i+1);
	}
	//Check the state of different containers and inputs
	//Different flags are defined here for the user interface

	//First check the balance of input and output
	check_balance(valvesArray_in[i],valvesArray_out[i]);

	check_saturation(i);

	printf("\n----------------------\n");
}
