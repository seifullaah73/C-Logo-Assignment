//---------------------------------------------------------------------------------
//Name: Seifullaah Sherrief
//User: SS00459
//Date: 10 December 2012
//
//Operation: This program reads commands from a text file, seperates each
//command and obtaining the command and the amount following the command, then the
//program draws each command and in the end displays the final shape.
//---------------------------------------------------------------------------------

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>     //All these include files are need to perform certain functions
#include <string>     //include <vector> to perform vector operations declare a vector
#include <sstream>    //or <string> to declare strings and to perform string functions
#include <iterator>
#include <fstream>
#include <cctype>
#include <cstring>
#include <algorithm>

using namespace std;

#include "window.h"
#include "logo.h"       //These files include functions which have to mentioned so the compiler
                        //can look for the function definition/declaration.
void draw(void)
{
    glRotatef(-90, 0,0,1);    //the start position should be facing to the right to match the test images

    for(unsigned o=0;o<actvec.size();o++)   //this loops through a global vector which contains the instructions and the amount
    {                                       //in order they are to be performed and the amount of time to be repeated for repeat functions
        if(actvec[o]=="FORWARD")
        {
            glBegin (GL_LINES);
                    glColor3f (1,1,1);
                    glVertex3f (0,0,0);
                    glVertex3f (0,(amountvec[o]),0);     //creates 2 points of a line and then joins the 2 points
            glEnd();                                    //with a line
            glTranslatef(0,(amountvec[o]),0);           //as the draw line function doesn't move forward when it creates a line
        }                                           //so this function moves forward to the end of the line
        if(actvec[o]=="RIGHT")
        {
            glRotatef((-amountvec[o]), 0,0,1);      //this function performs rotate function to the right and is represented by a negative angle
        }
        if(actvec[o]=="LEFT")
        {
            glRotatef(amountvec[o], 0,0,1);     //performs left rotation function
        }
        if(actvec[o]=="JUMP")
        {
            glTranslatef(0,(amountvec[o]),0);   //since it is a jump command, it just moves the point where it draws a certain amount forward
        }
    }
}

int main(int argc, char** argv)
{
    vector<string> Cmdlist;
    vector<float> numRepeats;
    vector<string> curcomds;    //this part of the program variables, vectors, strings are declared

    displayfunction();          //calls the display function which can be found in the header file
    Cmdlist=opentxtfile();      //this function opens text file and stores the contents in a vector

    for(unsigned j=0;j<Cmdlist.size();j++)  //the purpose of this loop is to loop through each line of command that was obtained
    {                                       //from the text file

        //cout<<"\nExecuting line:"<<Cmdlist[j]<<endl;
        curcomds=vectorprocess(Cmdlist[j],numRepeats);  //this process a vector, which splits a vector into to a vector for each repeat command

        if(curcomds.empty()==1)
        {
            cout<<"Error: The Vector Containing the Commands is empty";     //error message to state the vector is empty somehow
            system("PAUSE");       //To let the reader view the error message and once finished can press any key to exit
            exit(0);
        }
        if(numRepeats.empty()==0)   //this condition statement is used to tell if the repeat vector is not empty while currentcommand vector has commands in it
        {                           //so i can tell if it is a command containing repeat command or just 1 command per line without repeats
            repeatcmd(0,curcomds, numRepeats);  //sends vector of command and repeat number vectors to recursive function

            numRepeats.clear();     //clear the vector to store the next commands in the for loop
            curcomds.clear();
            //if((numRepeats.empty())&&(curcomds.empty()))
            //{
            //    cout<<"the vectors are empty now"<<endl;
            //}
        }
        else
        {
            stringstream am;    //stringstream is useful in manipulating strings
            string action;
            float value;
            am << Cmdlist[j];
            am >> action >> value;  //the stringstream can manipulate and separate the command into a string for command and a float for the amount
            //am.clear();
            //cout<<"Performing Action:"<<action<<endl;
            //cout<<"Amount:"<<value<<endl;
            performcommand(action,value);   //this function performs the commands

            numRepeats.clear();   //clear the vector to store the next commands
            curcomds.clear();
        }
    }

    window w(argc,argv);    //this is the main function which will open the window and display the drawing according to the functions in
    return 0;               //in the void draw function
}
