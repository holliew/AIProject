#include "controller.h"
#include <chrono>
#include "aStar.h"
#include <iostream>
#include <sys/time.h>

void controller::start() {
    //initialize variables. Also needs to empty masterNodeList on
    //every run after the first.
    a.createNeighborPairs();
    runs++;
    sorted = false;
    node init;
    init.gValue = 0;

    //set node.eightState to chosen state
    choice = ui.chooseInitialState();
    if (choice == 1){
        init.eightState.assign(init1.begin(), init1.end());
    }
    else{
        init.eightState.assign(init2.begin(), init2.end());
    }

    //set values for initial node
    init.hValue = a.runAlg(init, runs);
    init.fValue = init.hValue + init.gValue;
    init.parent = -1;


    //add first node to master list
    a.addNode(init);





    //send to work
    work();

}

void controller::work() {
    int current; // holds the index value of the node with lowest f value
    std::vector<node> children;
    timeval start, end;
    gettimeofday(&start, NULL);
    while (sorted != true) {

        children.clear();
        //Compare all F values
       // ui.printAll(a.masterNodeList);
       current = compareF( a.masterNodeList );
       //std::cout<<"current: " << current;//debugging

 //Generate Successors--------------------------------------------

        a.generateSuccessors(a.masterNodeList[current], children, current);
        a.masterNodeList[current].hasChildren = true;
       // std::cout<<"hello1\n";
 //Get H values of children---------------------------------------

        for (int i = 0; i < children.size(); i++){
           children[i].hValue = a.runAlg(children[i], runs);
        }
       // std::cout<<"hello2\n";
 //if h value of a child is 0 then set sorted=true-----------------

           for(int i = 0; i < children.size(); i++)
            {   children[i].fValue = children[i].hValue + children[i].gValue;
                if (children[i].hValue == 0){
                sorted = true;
                final = i;
                std::cout<<"congratulations\n";
                ui.printNode(children[i]);
                gettimeofday(&end, NULL);
                    addData(start, end, children[i]);
                }
            }
       // std::cout<<"hello3\n";
           for(int i = 0; i < children.size(); i++){
               a.addNode(children[i]);
           }

    }

ui.printData(arr);

}

//returns the index of node with lowest f value
int controller::compareF(std::vector<node> m){
    int bestF = 1000;
    int index;

    for (int i = 0; i < m.size();i++) {
        if (m[i].hasChildren == false) {
            if (m[i].fValue < bestF) {
                bestF = m[i].fValue;
                index = i;
                std::cout<<"fvalue:"<<m[i].fValue<<"\n"; //debugging
            }
        }
    }
return index;
}


void controller::addData(timeval start, timeval end, node n) {
    //get time:
    arr[runs][6] = 1;
    const double runtime = end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec) / 1000000.0;
    arr[runs][0] = runtime;
    std::cout<<arr[runs][0]<<"\n";

    //get total nodes:
    arr[runs][1]= a.masterNodeList.size();

    //Get Nodes that were expanded:
    for (int i = 0; i < a.masterNodeList.size(); i++){
        if(a.masterNodeList[i].hasChildren == true)
            arr[runs][2] += 1;
    }
     std::cout<<"nodes Expanded: "<< arr[runs][2]<<" ";

    //deepest branch
    arr[runs][3] = n.gValue;

    //Branching Factor
    arr[runs][4] = (arr[runs][1]/arr[runs][3]);

    //Memory Used
    arr[runs][5] = a.masterNodeList.size()*sizeof(node);

}