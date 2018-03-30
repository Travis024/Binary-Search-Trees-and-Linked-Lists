#include "MovieTree.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

//The default MovieTree constructor
MovieTree::MovieTree(){};

//The deconstructor; it frees up all memory by deleting the tree.
MovieTree::~MovieTree(){
    //Call the delete tree function here
}

/*
A function to add a node to the linked list of movies by
Pre-conditions: Data has been read in and a ranking, title, release year, and quantity are set.
Post-conditions: The movie is added to the linked list under the proper BST node.
*/
void MovieTree::addMovieNode(int ranking, std::string title, int releaseYear, int quantity){

        cout<<"we're here"<<endl;

        //To begin, create a new BST node and holders for searching through the BST
        MovieNodeBST * newNodeBST = new MovieNodeBST(title[0]);
        MovieNodeBST * parentBST = root;
        MovieNodeBST * tempBST = root;

        //Also create a linked list node for when we find where to put it.
        MovieNodeLL * newNodeLL = new MovieNodeLL(ranking, title, releaseYear, quantity);

        cout<<"now here"<<endl;

        //If the root is NULL, we want to establish the current BST as the root.
        if(root == NULL){
            cout<<"not here"<<endl;
            root = newNodeBST;
            root -> head = newNodeLL;
            root -> leftChild = NULL;
            root -> rightChild = NULL;
        }

        //Otherwise, a root has already been established.
        else{

            //Search through and find where the next BST node should be placed.
            while(tempBST != NULL){

                parentBST = tempBST;

                //The first two conditions will bring us to a leaf.
                if(tempBST -> letter > title[0]){
                    tempBST = tempBST -> leftChild;
                }
                else if(tempBST -> letter < title[0]){
                    tempBST = tempBST -> rightChild;
                }
                //If the node already exists, we want to stop here!
                else{
                    break;
                }

            }

            //This is the case for if the BST node already exists; want to now add a new linked list node to this part of the tree.
            if(parentBST -> letter == title[0]){

                MovieNodeLL * tempLL = parentBST -> head;
                MovieNodeLL * parentLL = tempLL;


                while(tempLL != NULL){

                    //Check if the movie title has a greater ASCII value or a smaller ASCII value
                    bool alphabeticalOrdering;

                    //Find how long we should compare titles for(for example, Star Wars A New Hope vs. Star Wars Revenge of the Sith)
                    int startIndex = 0;
                    int endIndex;
                    if(newNodeLL -> title.length() > tempLL -> title.length()){
                        endIndex= newNodeLL -> title.length();
                    }
                    else{
                        endIndex = tempLL -> title.length();
                    }


                    while(startIndex != endIndex){
                       //It's got a greater ASCII value, which means it should come after the current title
                       if(newNodeLL -> title[startIndex] > tempLL -> title[startIndex]){
                            alphabeticalOrdering = true;
                            break;
                        }
                        //It's got a smaller ASCII value, which means it should come before the current title
                        else if(newNodeLL -> title[startIndex] < tempLL -> title[startIndex]){
                            alphabeticalOrdering = false;
                            break;
                        }
                        //Values are equal, so check the next letters against each other.
                        else{
                            startIndex++;
                        }
                    }

                    //Move temp forward and check the title against the next node.
                    if(alphabeticalOrdering){
                        parentLL = tempLL;
                        tempLL = tempLL -> next;
                    }
                    //We have to insert the node!
                    else{

                        //If the title is less than the head's title, establish a new head.
                        if(tempLL == parentBST -> head){
                            newNodeLL -> next = parentBST -> head;
                            parentBST -> head = newNodeLL;
                            return;
                        }

                        //If we're in the middle of the list, position pointers in new order
                        newNodeLL -> next = parentLL -> next;
                        parentLL -> next = newNodeLL;
                        return;
                    }

                }

                //Otherwise, we must be at the end of the list. Just set one new pointer.
                parentLL -> next = newNodeLL;
            }

            //THE BST'S ARE NOT THE SAME
            //Move left on the tree
            else if(parentBST -> letter > title[0]){
                parentBST -> leftChild = newNodeBST;
                newNodeBST -> parent = parentBST;
                newNodeBST -> head = newNodeLL;
            }
            //Move right on the tree
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
                                minRightTree -> leftChild -> parent = minRightTree;
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
    //The movie ahs not been found.
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
