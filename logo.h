#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <fstream>
#include <cctype>
#include <cstring>
#include <algorithm>

using namespace std;

vector<float> amountvec;    //global vector variable to be able to transfer the commands to be processed to be sent to
vector<string> actvec;      //the void draw function in the main cpp file.

class Commands      //Class declaration for base class Commands
{
    protected:
        float amount;   //protected data which only derived class can access

    public:
        Commands();                 //default constructor
        virtual ~Commands();                //default destructor
        virtual void set_values(float amnt)     //this member function can help set the values sent to the class assigned to the protected
        {amount = amnt;}                        //data amount
        virtual void performaction(){};     //performaction member function
};

Commands::Commands(){}

Commands::~Commands(){}     //declaration of the constructor and destructor

class Forward: public Commands  //class Forward inherits public and protected data from Command Class
{
    public:
        void performaction()        //the performaction inherited from command class
        {
            actvec.push_back("FORWARD");    //add string command and amt value to global vector to send to the void draw function
            amountvec.push_back(amount);
        }
};

class Right : public Commands       //The same layout used for the class forward is used for the rest of the different left
{                                   //right jump command classes and the public inheritance of member functions from the base class command
    public:
        void performaction()
        {
            actvec.push_back("RIGHT");
            amountvec.push_back(amount);
        }

};

class Left : public Commands
{
    public:
        void performaction()
        {
            actvec.push_back("LEFT");
            amountvec.push_back(amount);
        }
};

class Jump : public Commands
{
    public:
        void performaction()
        {
            actvec.push_back("JUMP");
            amountvec.push_back(amount);
        }
};

void displayfunction(void)      //This function's operation is to display the instructions to the user on how to use the program
{
    cout<<"                    THE LOGO PROGRAM"<<endl;
    cout<<endl;
    cout<<"Welcome to the logo turtle program, a text file has been provided \n"
    <<"called command.txt, any commands should be spelt properly and written \n"
    <<"on the text file and saved. Valid Commands that are allowed include: \n"
    <<"FORWARD, JUMP, LEFT, RIGHT, REPEAT. Each command should be followed  \n"
    <<"by a number. When using the REPEAT command add square brackets [] \n"
    <<"after the number and place inside the command you would like repeated \n"
    <<"e.g. REPEAT 10[ FORWARD 10], you can include multiple nested repeat \n"
    <<"Statements e.g. REPEAT 10[ FORWARD 10 REPEAT 30[ FORWARD 9 LEFT 10]]"<<endl;
}

vector<string> vectorprocess(string cmd, vector<float>& nRepeats)
{
    stringstream ss(cmd);       //The operation of this function is to turn a string into a vector of commands
    stringstream act;           //where each element is stored with the commands for each repeat command
    string Action;
    vector <string> CommandList;
    float value;
    char braces[]="[]";

    CommandList.clear();
    nRepeats.clear();

    for (unsigned int y = 0; y < strlen(braces); ++y)   //remove the [ and ] chars from the string to be able to obtain the string and amount value
    {
      cmd.erase (remove(cmd.begin(), cmd.end(), braces[y]), cmd.end());
    }

    //cout<<"Instructions without the brackets\n"<<cmd<<endl;
    ss<<cmd;
    while( ss >> Action >> value )  //this obtains the string and value for all the pairs in the string until the end of the string
    {
        if( string("REPEAT") == Action )    //if first command is repeat store the amount to be repeated in repeat vector
        {
            //cout << "\nNew row : (REPEAT," << value << ") ";
            nRepeats.push_back(value);
            CommandList.push_back( string() );  //inserts a new line in the command list to insert the commands to be performed for each repeat function
        }
        else
        {
            //cout << '(' << Action << ',' << value << ')' << ' ';
            act.str( string() );
            act << Action << ' ' << value << ' ';
            if(nRepeats.empty())                    //this section combines the string and amount and then concatenates with the other
            {                                       //command and value that is to be included for the first repeat command
                CommandList.push_back(act.str());   //if there is no repeat command in the current string just add the command to the back of the vector
            }
            else CommandList.back() += act.str();   //concatenates all the commands for the current repeat command
        }
    }
    return CommandList;     //returns the vector back to the main cpp file where it was called.
}


void performcommand(string commandstr, float amnt)  //This function does the perform command part of the program where each value is sent to the corresponding class
{
        if(commandstr=="LEFT")
        {
            Left comdleft;              //conditional statements depending on the command, which class to the send the values to
            comdleft.set_values(amnt);
            comdleft.performaction();
        }
        else if(commandstr=="RIGHT")
        {
            Right comdleft;             //declares an object of the class right
            comdleft.set_values(amnt);  //sets the valie of amount sent to this function to be assigned to the amount value in the base class
            comdleft.performaction();   //call the memberfunction performaction in the derived right class
        }
        else if(commandstr=="FORWARD")
        {                                   //the above procedure is the same for all the commands that are passed to this function
            Forward comdleft;
            comdleft.set_values(amnt);
            comdleft.performaction();
        }
        else if(commandstr=="JUMP")
        {
            Jump comdleft;
            comdleft.set_values(amnt);
            comdleft.performaction();
        }
        else cout<<"The Command was not identifiable"<<endl;    //this states that if it does not below to any of the above commands then it is not a proper or legal command
}

void repeatcmd(unsigned int index, vector<string> current, vector<float> repeat)        //This is the recursive function for the repeat command to generate variable number of nested loops
{                                                                                   //according to how much nested repeat commands there are.
    string comd,action;
    float value;
    vector<string> tempcomd;

    comd=current[index];
    //cout<<"\n"<<comd<<endl;           //the main principle behind this function is pass an intial index value of 0 to the function as well as the vectors containing the command
    stringstream as(comd);

    while( as >> action >> value )      //split each string in the vector into parts containing one command and the following value
    {
        as << action << ' ' << value << ' ';
        tempcomd.push_back(as.str());       //then store it separately in a vector
        //cout<<action<<","<<value<<endl;
    }

	for(unsigned i=0;i<repeat[index];i++)       //this loops through a vector repeat which contains all the repeat amounts
	{
        for(unsigned x=0;x<tempcomd.size();x++)     //loop through the commands in the vector
        {
            as.clear();
            as << tempcomd[x];
            as >> action >> value;
            as.clear();
            //cout<<"Performing Action:"<<action<<endl;
            //cout<<"Amount:"<<value<<endl;
            performcommand(action,value);           //perform the command
        }
		if((index+1) < repeat.size())           //if index, which at start is 0 is less than than repeat vector size, which is equal to the number of nested loops required
		{                                       //call this function but increment the index until the last command is reached and then it is performed a number of times. it comes out of the
			repeatcmd(index+1, current, repeat);    //of the last command goes to previous command as is the property of a recursive function perform that command and then perform the last command again and so on
		}                                           // and so forth until all the commands performed for a certain number of times.
	}
}

vector<string> opentxtfile(void)    //this function opens a text file and obtains the commands from the file
{
    string currentline;
    vector<string> cmdlist;

    ifstream myfile ("c:\\Users\\Safiyullaah\\C++\\Final\\command.txt");    //ofstream is for outputting to a text file and ifstream input to textfile
    if (myfile.is_open())
    {
        while ( getline(myfile,currentline) )
        {
                cmdlist.push_back( currentline + '\n' );    //this creates a list of commands in the order they appear in the text file and are placed in the vector
        }
        if(cmdlist.empty())
        {
            cout<<"\nText file appears to be empty according to the first line"<<endl;      //prompt the user the text file does not contain anything or the first line is empty
            system("PAUSE");
            exit(0);
        }
        myfile.close();     //closes the file
    }
    else
    {
        cout<<"\nSorry the file could not be opened, please make sure the filename command.txt \nexists in the program directory"<<endl;   //error message if the file could not be opened.
        system("PAUSE");
        exit(0);
    }
    return cmdlist;     //return the commandlist vector back to where it was called in the main cpp file.
}
