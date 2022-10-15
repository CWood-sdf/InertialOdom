/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       woodc                                                     */
/*    Created:      10/15/2022, 3:03:16 PM                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

using namespace vex;
// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain       Brain;
using namespace std;
#define BASE <C:/Users/woodc/OneDrive/GitHub/Libs-Actual/Libs
/*<>*/
#include BASE/basefile.h> //Libs
// define your global instances of motors and other devices here
inertial Inertial = inertial(PORT1);
PVector pos = PVector(0,0,0);
PVector vel = PVector(0,0,0);
int main() {
    gyroInit(Inertial);
    
    LinkedList<PVector> accelerations = {};
    LinkedList<PVector> velocities = {};
    double sleepTime = 10;
    thread printThread = thread([](){
        while(true){
            //Clear the screen
            Brain.Screen.clearScreen();
            //Print the position and velocity
            Brain.Screen.printAt(1, 10, "Position: %f, %f, %f", pos.x, pos.y, pos.z);
            Brain.Screen.printAt(1, 20, "Velocity: %f, %f, %f", vel.x, vel.y, vel.z);
            //Cout the position and velocity
            cout << "Position: " << pos.x << ", " << pos.y << ", " << pos.z << endl;
            cout << "Velocity: " << vel.x << ", " << vel.y << ", " << vel.z << endl;
            //Sleep for 10 milliseconds
            this_thread::sleep_for(10);
            //Wait for brain reset
            Brain.Screen.waitForRefresh();
        }
    });
    while(1) {

        //Store the acceleration values in a PVector
        PVector accel = PVector(Inertial.acceleration(vex::axisType::xaxis), Inertial.acceleration(vex::axisType::yaxis), Inertial.acceleration(vex::axisType::zaxis) - 1);
        //Convert acceleration to in/s^2
        accel.mult(386.09);
        if(accelerations.size(2)){
            //Use simpsons rule to integrate the acceleration
            PVector accel1 = accelerations.getBase();
            PVector accel2 = accelerations.getBase().getNext();
            PVector accel3 = accel;
            PVector velEstimate = (accel1 + (accel2 * 4.0) + accel3) * ((sleepTime/1000.0)/6.0) + vel/*<<Don't forget '+ C'*/;
            //Clear accelerations list
            accelerations.clear();
            //Add vel to the list of velocities
            velocities.push_back(velEstimate);
        }
        //Add the acceleration to the list of accelerations
        accelerations.push_back(accel);

        //Use simpsons rule to integrate the velocity
        if(velocities.size(3)){
            PVector vel1 = velocities.getBase();
            PVector vel2 = velocities.getBase().getNext();
            PVector vel3 = velocities.getEnd();
            pos = (vel1 + (vel2 * 4.0) + vel3) * ((sleepTime/1000.0)/6.0) + pos/*<<Don't forget '+ C'*/;
            //Clear first 2 velocity elements
            velocities.popBase();
            velocities.popBase();
        }
        //Print the position
        Brain.Screen.printAt(0, 0, "X: %f", pos.x);
        Brain.Screen.printAt(0, 20, "Y: %f", pos.y);
        Brain.Screen.printAt(0, 40, "Z: %f", pos.z);
        //Print velocity
        Brain.Screen.printAt(0, 60, "VX: %f", vel.x);
        Brain.Screen.printAt(0, 80, "VY: %f", vel.y);
        Brain.Screen.printAt(0, 100, "VZ: %f", vel.z);

        


        // //Rotate the acceleration values to the world frame
        // //Construct a rotation matrix from the gyro values
        // //PVector gyro = PVector(Inertial.pitch(), Inertial.roll(), Inertial.yaw());

        // accelerations.push_back(accel);
        // //Apply integral to get velocity
        // //Use simpsons rule to integrate
        

        // //Clear the list if it gets too big
        // if(accelerations.size() > 100) {
        //     accelerations.clear();
        // }
        // // Allow other tasks to run
        this_thread::sleep_for(sleepTime);
    }
}
