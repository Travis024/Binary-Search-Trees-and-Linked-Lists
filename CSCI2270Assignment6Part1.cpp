#include "MovieTree.hpp"
#include "MovieTree.cpp"

using namespace std;

int main(int argc, char * argv[]){

    //First things first, we want to read in the file passed and construct a BST with linked lists.
    MovieTree initTree;

    ifstream in_stream(argv[1]);

    //Check to see if opening the file worked properly
    if(in_stream.is_open()){

        string currentLine = "";

        //iterate through every line of the text file
        while(getline(in_stream,currentLine)){

            //These variables are variables needed to create a MovieNodeLL instance
            int ranking, year, quantity;
            string movieTitle;

            //Will store the ranking/title/year/quantity as we go through each line, delimiting by comma
            string currentVariable = "";
            //The ranking is always 1st, title 2nd, year 3rd, quantity 4th
            int variableIndex = 0;

            //Add a comma to the end of the line for delimiting purposes
            currentLine = currentLine + ',';
            int lineLength = currentLine.length();

            for(int i = 0; i < lineLength; i++){
                //If character isn't comma, add it on to the variable
                if(currentLine[i] != ','){
                    currentVariable = currentVariable + currentLine[i];
                }

                //If the character is a comma, then store the variable, reset the variable, and increment variableIndex.
                else{

                    if(variableIndex == 0){
                        ranking = stoi(currentVariable);
                        currentVariable = "";
                        variableIndex++;
                    }

                    else if(variableIndex == 1){
                        movieTitle = currentVariable;
                        currentVariable = "";
                        variableIndex++;
                    }

                    else if(variableIndex == 2){
                        year = stoi(currentVariable);
                        currentVariable = "";
                        variableIndex++;
                    }

                    else{
                        quantity = stoi(currentVariable);
                    }

                }
            }

            //Add the data as a new node using the addMovieNode class method
            initTree.addMovieNode(ranking, movieTitle, year, quantity);
        }

    }

    //Report that there was an error opening the file
    else{
        cout<<"Error opening file "<<argv[1]<<endl;
    }

    //Will control the menu screen being constantly re-printed to the user.
    bool continueMenu = true;

    while(continueMenu){
        cout << "======Main Menu======" << endl;
        cout << "1. Find a movie" << endl;
        cout << "2. Rent a movie" << endl;
        cout << "3. Print the inventory" << endl;
        cout << "4. Delete a movie" << endl;
        cout << "5. Count the movies" << endl;
        cout << "6. Quit" << endl;

        char userChoice;
        cin>>userChoice;

        if(userChoice == '1'){
            //Clear the stream
            cin.ignore();

            string movieToFind;

            //Get a movie and pass through function
            cout<<"Enter title:"<<endl;
            getline(cin, movieToFind);

            initTree.findMovie(movieToFind);
        }

        else if(userChoice == '2'){

            //clear the stream
            cin.ignore();

            string movieToRent;

            //Get a movie and pass through function
            cout<<"Enter title:"<<endl;
            getline(cin, movieToRent);

            initTree.rentMovie(movieToRent);
        }

        else if(userChoice == '3'){
            //Call the function for tree transversal
            initTree.printMovieInventory();
        }

        else if(userChoice == '4'){

            //Clear the stream
            cin.ignore();

            string movieToDelete;

            //Get a movie and pass through function
            cout<<"Enter title:"<<endl;
            getline(cin, movieToDelete);

            initTree.deleteMovieNode(movieToDelete);
        }

        else if(userChoice == '5'){

            //Call function for transversal and counting
            cout<<"Tree contains: "<<initTree.countMovieNodes()<<" movies." << endl;
        }

        else if(userChoice == '6'){

            //Exit the loop
            cout<<"Goodbye!"<<endl;
            continueMenu = false;


        }

        else{
            cout<<"Invalid choice!"<<endl;
        }
    }

}

