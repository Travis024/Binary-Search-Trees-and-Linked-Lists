#include <iostream>
#include <string>
#include "MovieTree.hpp"

using namespace std;

//The default MovieTree constructor
MovieTree::MovieTree(){
    root = NULL;

};

//The deconstructor; it frees up all memory by deleting the tree.
MovieTree::~MovieTree(){
    //Delete every node using post order transversal.
    //Pass through the root.

    DeleteAll(root);
}


void MovieTree::addMovieNode(int ranking, std::string title, int releaseYear, int quantity){

        MovieNodeBST * newNodeBST = new MovieNodeBST(title[0]);
        MovieNodeBST * parentBST = root;
        MovieNodeBST * tempBST = root;

        MovieNodeLL * newNodeLL = new MovieNodeLL(ranking, title, releaseYear, quantity);

        if(root == NULL){
            root = newNodeBST;
            root -> head = newNodeLL;
            root -> leftChild = NULL;
            root -> rightChild = NULL;
        }

        else{

            while(tempBST != NULL){

                parentBST = tempBST;

                if(tempBST -> letter > title[0]){
                    tempBST = tempBST -> leftChild;
                }
                else if(tempBST -> letter < title[0]){
                    tempBST = tempBST -> rightChild;
                }
                else{
                    break;
                }

            }

            if(parentBST -> letter == title[0]){

                MovieNodeLL * tempLL = parentBST -> head;
                MovieNodeLL * parentLL = tempLL;


                //returns true if greater than, false if less than
                while(tempLL != NULL){

                    bool alphabeticalOrdering;

                    int startIndex = 0;
                    int endIndex;
                    if(newNodeLL -> title.length() > tempLL -> title.length()){
                        endIndex= newNodeLL -> title.length();
                    }
                    else{
                        endIndex = tempLL -> title.length();
                    }

                    while(startIndex != endIndex){
                       if(newNodeLL -> title[startIndex] > tempLL -> title[startIndex]){
                            alphabeticalOrdering = true;
                            break;
                        }
                        else if(newNodeLL -> title[startIndex] < tempLL -> title[startIndex]){
                            alphabeticalOrdering = false;
                            break;
                        }
                        else{
                            startIndex++;
                        }
                    }

                    if(alphabeticalOrdering){
                        parentLL = tempLL;
                        tempLL = tempLL -> next;
                    }
                    else{

                        if(tempLL == parentBST -> head){
                            newNodeLL -> next = parentBST -> head;
                            parentBST -> head = newNodeLL;
                            return;
                        }

                        newNodeLL -> next = parentLL -> next;
                        parentLL -> next = newNodeLL;
                        return;
                    }

                }
                parentLL -> next = newNodeLL;
            }
            else if(parentBST -> letter > title[0]){
                parentBST -> leftChild = newNodeBST;
                newNodeBST -> parent = parentBST;
                newNodeBST -> head = newNodeLL;
            }
            else{
                parentBST -> rightChild = newNodeBST;
                newNodeBST -> parent = parentBST;
                newNodeBST -> head = newNodeLL;
            }

        }


}

MovieNodeBST * MovieTree::searchBST(MovieNodeBST *node, std::string title){

    MovieNodeBST * temp = node;

    if(temp != NULL){

        if(temp -> letter == title[0]){
            return temp;
        }

        else if(temp -> letter > title[0]){
            return searchBST(temp -> leftChild, title);
        }

        else{
            return searchBST(temp -> rightChild, title);
        }
    }
}

void MovieTree::deleteMovieNode(std::string title) {


    //Use the searchBST function to find the correct BST node
    MovieNodeBST * tempBST = searchBST(root, title);

    //If the BST doesn't exist, then the movie must not be in any linked list
    if(tempBST == NULL){
        cout<<"Movie not found."<<endl;
    }
    else{

        //Now, search for the movie title in the linked list
        MovieNodeLL * tempLL = searchLL(tempBST -> head, title);

        //Find the node before the one we're deleting
        //This is for updating pointers.
        //NOTE: Checks != tempLL in case tempLL is the parent.
        MovieNodeLL * parentLL = tempBST -> head;
        while(parentLL -> next != tempLL && parentLL != tempLL){
                parentLL = parentLL -> next;
            }

        //If tempLL is NULL, the movie is NOT in the linked list.
        if(tempLL == NULL){
            cout<<"Movie not found."<<endl;
        }
        else{
            //we're deleting the head of the linked list
            if(tempLL == tempBST -> head){

                //this is the last node, so delete the BST node, too!
                if(tempLL -> next == NULL){

                    //THIS IS THE LEAF CASE
                    if(tempBST -> leftChild == NULL && tempBST -> rightChild == NULL){

                        //Check to see if tempBST is the left or right child
                        if(tempBST -> letter < tempBST -> parent -> letter){
                            tempBST -> parent -> leftChild = NULL;
                            delete tempBST;
                        }

                        else{
                            tempBST -> parent -> rightChild = NULL;
                            delete tempBST;
                        }

                    }

                    //THIS IS A ONE CHILD CASE
                    //Check for root because we want to force it to the two child case.
                    else if((tempBST -> leftChild != NULL || tempBST -> rightChild == NULL) && tempBST != root){

                        //the node being deleted has a left child
                        if(tempBST -> leftChild != NULL){
                            MovieNodeBST * oneChildHolder = tempBST -> leftChild;

                            //the node being deleted is a left child
                            if(tempBST -> letter < tempBST -> parent -> letter){
                                tempBST -> parent -> leftChild = oneChildHolder;
                                oneChildHolder -> parent = tempBST -> parent;
                                delete tempBST;
                                return;
                            }

                            //the node being deleted is a right child
                            else{
                                tempBST -> parent -> rightChild = oneChildHolder;
                                oneChildHolder -> parent = tempBST -> parent;
                                delete tempBST;
                                return;
                            }
                        }

                        //the node being deleted has a right child
                        if(tempBST -> rightChild != NULL){
                            MovieNodeBST * oneChildHolder = tempBST -> rightChild;

                            //the node being deleted is a left child
                            if(tempBST -> letter < tempBST -> parent -> letter){
                                tempBST -> parent -> leftChild = oneChildHolder;
                                oneChildHolder -> parent = tempBST -> parent;
                                delete tempBST;
                                return;
                            }

                            //the node being deleted is a right child
                            else{
                                tempBST -> parent -> rightChild = oneChildHolder;
                                oneChildHolder -> parent = tempBST -> parent;
                                delete tempBST;
                                return;
                            }
                        }

                    }

                    //THIS IS A TWO CHILD CASE
                    else{

                        MovieNodeBST * minRightTree = tempBST -> rightChild;
                        while(minRightTree -> leftChild != NULL){
                            minRightTree = minRightTree -> leftChild;
                        }


                        //the child is the minimum value
                        if(minRightTree == tempBST -> rightChild){
                            //we're deleting the root, so a new one must be delegated
                            if(tempBST == root){
                                minRightTree -> leftChild = tempBST -> leftChild;
                                minRightTree -> leftChild -> parent = minRightTree;
                                root = minRightTree;
                                delete tempBST;
                                return;
                            }

                            //The node being deleted is a left child
                            else if(tempBST -> letter < tempBST -> parent -> letter){
                                tempBST -> parent -> leftChild = minRightTree;
                                minRightTree -> parent = tempBST -> parent;
                                minRightTree -> leftChild = tempBST -> leftChild;
                                minRightTree -> leftChild -> parent = minRightTree;
                                delete tempBST;
                                return;
                            }

                            //the node being deleted is a right child
                            else{
                                tempBST -> parent -> rightChild = minRightTree;
                                minRightTree -> parent = tempBST -> parent;
                                minRightTree -> leftChild = tempBST -> leftChild;
                                delete tempBST;
                                return;
                            }

                        }

                        //the minimum is NOT the child of tempBST!
                        else{
                            //If the min has children, make sure they stay connected.
                            if(minRightTree -> rightChild != NULL){
                                minRightTree -> parent -> leftChild = minRightTree -> rightChild;
                                minRightTree -> rightChild -> parent = minRightTree -> parent;
                            }

                            //If it's the root, delegate a new root.
                            //No parent pointers!
                            if(tempBST == root){
                                minRightTree -> leftChild = tempBST -> leftChild;
                                minRightTree -> rightChild = tempBST -> rightChild;
                                tempBST -> rightChild -> parent = minRightTree;
                                tempBST -> leftChild -> parent = minRightTree;
                                root = minRightTree;
                                delete tempBST;
                                return;
                            }

                            //the node being deleted is a left child
                            else if(tempBST -> letter < tempBST -> parent -> letter){
                               tempBST -> parent -> leftChild = minRightTree;
                               minRightTree -> parent = tempBST -> parent;
                               minRightTree -> leftChild = tempBST -> leftChild;
                               minRightTree -> rightChild = tempBST -> rightChild;
                               tempBST -> rightChild -> parent = minRightTree;
                               tempBST -> leftChild -> parent = minRightTree;
                               delete tempBST;
                               return;
                            }

                            //the node being deleted is a right child
                            else{
                               tempBST -> parent -> rightChild = minRightTree;
                               minRightTree -> parent = tempBST -> parent;
                               minRightTree -> leftChild = tempBST -> leftChild;
                               minRightTree -> rightChild = tempBST -> rightChild;
                               tempBST -> rightChild -> parent = minRightTree;
                               tempBST -> leftChild -> parent = minRightTree;
                               delete tempBST;
                               return;
                            }

                        }

                    }
                    return;
                }

                //the linked list isn't empty, so make sure to delegate a new head for the BST
                else{
                    tempBST -> head = tempLL -> next;
                    delete tempLL;
                    return;
                }
            }

            //In this case, the node is at the end of the linked list
            //Just have to update one pointer and delete.
            else if(tempLL -> next == NULL){
                parentLL -> next = NULL;
                delete tempLL;
                return;
            }

            //In this case, it's in the middle of the linked list.
            //Update a pointer and delete.
            else{
                parentLL -> next = tempLL -> next;
                delete tempLL;
                return;
            }

        }

    }
}



MovieNodeBST * treeMinimum(MovieNodeBST * node) {

    MovieNodeBST * temp = node;

    //Go all the way down to the left end
    //Stop before making tmp NULL
    while(temp -> leftChild != NULL){
        temp = temp -> leftChild;
    }

    return temp;

}

void MovieTree::findMovie(std::string title) {

    //First, see if the letter is in the BST
    MovieNodeBST * firstLetter = searchBST(root, title);

    //If the letter isn't in the BST, then this node will be NULL
    //The movie has not been found.
    if(firstLetter == NULL){
        cout<<"Movie not found."<<endl;
        return;
    }

    //Now, see if the title is in the linked list
    MovieNodeLL * movieTitle = searchLL(firstLetter -> head, title);

    //If the title isn't in the linked list, then this node is NULL
    //The movie has not been found.
    if(movieTitle == NULL){
        cout<<"Movie not found."<<endl;
    }

    //Otherwise, we found the movie.
    //Print all its info to the user.
    else{
        cout << "Movie Info:" << endl;
        cout << "===========" << endl;
        cout << "Ranking:" << movieTitle->ranking << endl;
        cout << "Title:" << movieTitle->title << endl;
        cout << "Year:" << movieTitle->year << endl;
        cout << "Quantity:" << movieTitle->quantity << endl;
    }
}

void MovieTree::rentMovie(std::string title) {


    //See if the title exists in the BST
    MovieNodeBST * movieLetter = searchBST(root, title);
    //If the node is NULL, the letter, and thus the movie, don't exist.
    if(movieLetter == NULL){
        cout<<"Movie not found."<<endl;
        return;
    }

    //search for the movie in the liked list
    MovieNodeLL * movieTitle = searchLL(movieLetter -> head, title);

    //If the node is NULL, the movie isn't in the list.
    if(movieTitle == NULL){
        cout<<"Movie not found."<<endl;
    }
    else{

        //rent the movie out/decrement quantity
        movieTitle -> quantity = movieTitle -> quantity - 1;

        //Display the info
        cout << "Movie has been rented." << endl;
        cout << "Movie Info:" << endl;
        cout << "===========" << endl;
        cout << "Ranking:" << movieTitle->ranking << endl;
        cout << "Title:" << movieTitle->title << endl;
        cout << "Year:" << movieTitle->year << endl;
        cout << "Quantity:" << movieTitle->quantity << endl;

        //If the qunatity is now 0, delete the node
        if(movieTitle -> quantity == 0){

            deleteMovieNode(title);

        }
    }

}

int MovieTree::countMovieNodes(){

    //initialize the number of movies as 0
    int numberOfMovies = 0;

    //Pass its address through thge function so its value is changed at each BST node.
    countMovieNodes(root, &numberOfMovies);

    return numberOfMovies;
}

void MovieTree::countMovieNodes(MovieNodeBST *node, int *c) {

    //This will be an in order transversal of the tree
    if(node!=NULL){
        //First, go through the left side of the tree from smallest to largest
        countMovieNodes(node -> leftChild, c);

        //For every BST node, count every movie it has.
        MovieNodeLL * transverseLL = node -> head;
        while(transverseLL != NULL){
            //Remember to update C using a dereferencer
            *c = *c + 1;
            transverseLL = transverseLL -> next;
        }

        //then go through the entire right side of the tree
        countMovieNodes(node -> rightChild, c);
    }


}

void MovieTree::printMovieInventory(){
    //call the private printMovieInventory function on the root
    printMovieInventory(root);
}

void MovieTree::printMovieInventory(MovieNodeBST * node){
    //This will transverse the tree in order
    //At each BST node, print the linked list from head to end.

    if(node != NULL){

        printMovieInventory(node -> leftChild);

        //Start with the left side of the tree
        MovieNodeLL * transverseLL = node -> head;

        //Go through the linked list and print every movie and its quantity
        while(transverseLL != NULL){
            cout<<"Movie: "<<transverseLL->title<<" "<<transverseLL->quantity<<endl;
            transverseLL = transverseLL -> next;
           }

        //Then do the right side of the tree
        printMovieInventory(node -> rightChild);

     }


    }


MovieNodeLL * MovieTree::searchLL(MovieNodeLL * head, std::string title){

    MovieNodeLL * transverseLL = head;

    while(transverseLL != NULL){
        if(transverseLL -> title == title){
            return transverseLL;
        }
        else{
            transverseLL = transverseLL -> next;
        }
    }

    return NULL;

}

void MovieTree::DeleteAll(MovieNodeBST * node){

    //Post order transversal
    //This way we don't lose the root.
    if(node != NULL){

        DeleteAll(node->leftChild);
        DeleteAll(node->rightChild);

        //Go through and delete each LL node as well
        MovieNodeLL * transverseLL = node -> head;
        MovieNodeLL * transverseNext = transverseLL -> next;

        while(transverseNext != NULL){
            cout<<"Deleting: "<<transverseLL -> title<<endl;
            delete transverseLL;
            transverseLL = transverseNext;
            transverseNext = transverseNext -> next;
        }

        //Since we checked if transverseNext was NULL, transverseLL still exists
        //Delete it now
        cout<<"Deleting: "<<transverseLL -> title<<endl;
        delete transverseLL;

        //And finally, delete the BST node
        delete node;
    }

}
