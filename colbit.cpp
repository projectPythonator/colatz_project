/* title  :: colatz cycle finding
 * author :: agis daniels
 * date   :: September 25 2016
 * my docs
 * this program is for running through numbers in the colatz conjecture
 * it checks for cycles that are greater then there n in starting range
 * it uses an avl tree 
 * we use the number 2 billion to minimize the amount of inserts we do
 * in truth we are more worried about throwing our selves into a cycle
 * asap then how fast we relize we are in a cycle  other then the 
 * trival 4,2,1,4,2,1...
 * my method is n=3*n+1 then right shift n till its odd again
 * works in cunjution with the library gmp 6.1.1
 * get from here https://gmplib.org/
 * compilied with clang++ -O2 -lgmp -lm -std=gnu++11 colbit.cpp -ocolatz
 */

#include <algorithm>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdio.h>
#include <string>
#include <gmp.h>

typedef std::chrono::high_resolution_clock Clock; 

/* AVL node */
template <class T>
class AVLnode 
{
public:
	T key;
	int balance;
	AVLnode *left, *right, *parent;

	AVLnode(T k, AVLnode *p) : key(k), balance(0), parent(p),
	left(NULL), right(NULL) {}

	~AVLnode() 
    {
		delete left;
		delete right;
	}
};

/* AVL tree */
template <class T>
class AVLtree 
{
public:
	AVLtree(void);
	~AVLtree(void);
	bool insert(T key);
	void deleteKey(const T key);

private:
	AVLnode<T> *root;

	AVLnode<T>* rotateLeft          ( AVLnode<T> *a );
	AVLnode<T>* rotateRight         ( AVLnode<T> *a );
	AVLnode<T>* rotateLeftThenRight ( AVLnode<T> *n );
	AVLnode<T>* rotateRightThenLeft ( AVLnode<T> *n );
	void rebalance                  ( AVLnode<T> *n );
	int height                      ( AVLnode<T> *n );
	void setBalance                 ( AVLnode<T> *n );
	void clearNode                  ( AVLnode<T> *n );
};

/* AVL rebalance 
 * rebalances the tree to keep its logn property
 */
template <class T>
void AVLtree<T>::rebalance(AVLnode<T> *n) 
{
	setBalance(n);

	if (n->balance == -2) 
    {
		if (height(n->left->left) >= height(n->left->right))
        {
		    n = rotateRight(n);
        }
		else
        {
		    n = rotateLeftThenRight(n);
        }
	}
	else if (n->balance == 2) 
    {
		if (height(n->right->right) >= height(n->right->left))
        {
		    n = rotateLeft(n);
        }
		else
        {
		    n = rotateRightThenLeft(n);
        }
	}
	if (n->parent != NULL) 
    {
		rebalance(n->parent);
	}
	else 
    {
		root = n;
	}
}

/*
 *avl left rotate rotates a node in the tree
 */
template <class T>
AVLnode<T>* AVLtree<T>::rotateLeft(AVLnode<T> *a) 
{
	AVLnode<T> *b = a->right;
	b->parent = a->parent;
	a->right = b->left;

	if (a->right != NULL)
    {
	    a->right->parent = a;
    }

	b->left = a;
	a->parent = b;

	if (b->parent != NULL) 
    {
		if (b->parent->right == a) 
        {
			b->parent->right = b;
		}
		else 
        {
			b->parent->left = b;
		}
	}
	setBalance(a);
	setBalance(b);
	return b;
}

/*
 * does the right rotate on the tree
 */
template <class T>
AVLnode<T>* AVLtree<T>::rotateRight(AVLnode<T> *a) 
{
	AVLnode<T> *b = a->left;
	b->parent = a->parent;
	a->left = b->right;

	if (a->left != NULL)
    {
	    a->left->parent = a;
    }

	b->right = a;
	a->parent = b;

	if (b->parent != NULL) 
    {
		if (b->parent->right == a) 
        {
			b->parent->right = b;
		}
		else 
        {
			b->parent->left = b;
		}
	}
	setBalance(a);
	setBalance(b);
	return b;
}
/*
 * name says it all
 */
template <class T>
AVLnode<T>* AVLtree<T>::rotateLeftThenRight(AVLnode<T> *n) 
{
	n->left = rotateLeft(n->left);
	return rotateRight(n);
}
/*
 * name says it all
 */
template <class T>
AVLnode<T>* AVLtree<T>::rotateRightThenLeft(AVLnode<T> *n) 
{
	n->right = rotateRight(n->right);
	return rotateLeft(n);
}

/*
 * gets the height of the current node based on the large
 * of the two sub trees heights
 */
template <class T>
int AVLtree<T>::height(AVLnode<T> *n) 
{
	if (n == NULL)
    {
	    return -1;
    }
	return 1 + std::max(height(n->left), height(n->right));
}

/*
 * sets the ballance for a node in the tree
 */
template <class T>
void AVLtree<T>::setBalance(AVLnode<T> *n) 
{
	n->balance = height(n->right) - height(n->left);
}

//class constructor
template <class T>
AVLtree<T>::AVLtree(void) : root(NULL) {}

//class destructor
template <class T>
AVLtree<T>::~AVLtree(void) 
{
	delete root;
}

/* avl tree insert---- its not recursive function
 * given key of type T
 * return true if key is inserted from being the only one 
 * false otherwise
 */
template <class T>
bool AVLtree<T>::insert(T key)
{
	if (root == NULL) 
    {
		root = new AVLnode<T>(key, NULL);
	}
	else
    {
		AVLnode<T>
		*n = root,
		*parent;

		while (true) 
        {
			if (n->key == key)
            {
			    return false;
            }

			parent = n;

			bool goLeft = n->key > key;
			n = goLeft ? n->left : n->right;

			if (n == NULL)
            {
				if (goLeft) 
                {
					parent->left = new AVLnode<T>(key, parent);
				}
				else 
                {
					parent->right = new AVLnode<T>(key, parent);
				}

				rebalance(parent);
				break;
			}
		}
	}
	return true;
}

template <class T>
void AVLtree<T>::deleteKey(const T delKey)
{
	if (root == NULL)
    {
	    return;
    }

	AVLnode<T>
	*n       = root,
	*parent  = root,
	*delNode = NULL,
	*child   = root;

	while (child != NULL) 
    {
		parent = n;
		n = child;
		child = delKey >= n->key ? n->right : n->left;
		if (delKey == n->key)
        {
		    delNode = n;
        }
	}

	if (delNode != NULL) 
    {
		delNode->key = n->key;

		child = n->left != NULL ? n->left : n->right;

		if (root->key == delKey) 
        {
			root = child;
		}
		else 
        {
			if (parent->left == n) 
            {
				parent->left = child;
			}
			else 
            {
				parent->right = child;
			}
			rebalance(parent);
		}
	}
}

/*check_file()
 * param none
 * return true if the file to get input is empty false otherwise
 */
bool check_file()
{
    std::ifstream ifs ( "numsleft.txt" , std::ios::ate ); // std::ios::ate means open at end
    if( ifs.tellg() == 0)
    {
        //return true when empty
        ifs.close();
        return true;
    }
    //return false when not enpty
    ifs.close();
    return false;
}

/*mod_the_of_file()
 *this is to modify the file i took numbers
 *from so i dont need to manually do it
 *i just used a comment make temp copy it over
 *then exclude the first two lines of the file
 */
void mod_the_og_file()
{
    //open both files
    std::ofstream my_temp_file;
    my_temp_file.open      ( "tnumleft.txt" );
    std::ifstream mod_file ( "numsleft.txt" );
    //make my string to write to them and skip the first two lines
    std::string write_str; 
    std::getline ( mod_file , write_str );
    //write the rest of the lines to the temp file
    while( std::getline( mod_file , write_str ))
    {
        my_temp_file << write_str+'\n';
    }
    //close both files and then rename the temp one to be the same as old
    my_temp_file.close ();
    mod_file.close     ();
    std::string oldname = "tnumleft.txt";
    std::string newname = "numsleft.txt";
    int result = std::rename ( oldname.c_str() , newname.c_str() );
    if ( result == 0 )
        puts ( "File successfully renamed" );
    else
        perror( "Error renaming file" );
}

/* mpz_function reviews(big integer functions)
 *
 * VARRIABLE TYPES:
 * mpz_t       :: is the big integer type from gmp library ( it is represented as pointer but gets seen as int)
 * mp_bitcnt_t :: an index of a bit represented as an int 
 *
 * FUNCTION TYPES:
 * mpz_init_set_str :: sets the mpz_t to equal the string
 *                     form = func_call ( mpz_t x , string str , int str_in_base )
 *
 * mpz_init_set_si  :: sets the mpz_t to euqal the unsigned long int
 *                     form = func_call ( mpz_t x , int number )
 *
 * mpz_cmp          :: form = func_call ( mpz_t x , mpz_t y )
 *                     returns int > 0 if x > y
 *                                   0 if x == y
 *                             int < 0 if x < y
 *
 * mpz_add          :: form = func_call (mpz_t x , mpz_t x , mpz_t y)
 *                     adds y to x  and stores it in x
 *
 * mpz_mul          :: same as add but does multiplication not addition
 *
 * mpz_tdiv_q_2exp  :: form = func_call ( mpz_t x , mpz_t x , mp_bitcnt_t b )
 *                     does a division of the form x = x/(2^b)
 *
 * mpz_scan1        :: form = func_call ( mpz_t x , bit t or f (int form) )
 *                     finds right most bit matching t or f and return index
 *
 * mpz_set_str      :: form = func_call ( char* c , int b , mpz_t x )
 *                     when c is null returns char* with the allocated space
 *                     to hold x in base b
 * mpz_set          :: form = func_call ( mpz_t x , mpz_t y )
 *                     sets x equal to the value of y 
 */
int main() 
{
    //explained at very bottom
    Begining:
    //declaring varriables used in program
    mpz_t mainVar , ONE , TWO , THREE , HELPER;
    //setting up strings and file mods needed in program
    std::string strStar;
    std::ifstream in_file_nums ("numsleft.txt");
    std::getline       (in_file_nums, strStar);
    in_file_nums.close ();
    mod_the_og_file    ();
    //this is where all the inits before the main range loop happen 
    mpz_init_set_str    ( mainVar , strStar.c_str() , 10 ); //main varriable that we work with
    mpz_init_set_str    ( HELPER  , strStar.c_str() , 10 ); //helper to use in the compare function
    mpz_init_set_si     ( ONE     , 1 );                    //var One
    mpz_init_set_si     ( TWO     , 2 );                    //var Two
    mpz_init_set_si     ( THREE   , 3 );                    //var Three
    std::cout <<   strStar  << std::endl;
    //i like to keep time
    auto t1 = Clock::now();
    /*main range loop
     * params
     * checker::int, we use it to cut down on how many times
     *          we insert into the avl tree(see docs at top for more)
     * cycle_checker_tree::avl tree, the tree i use to catch
     *          any cycles
     * the while loop inside is the main working loop
     * see docs above for desciption of functions used otherwise
     */
    for( long amt = 100000000000 ; amt ; --amt , mpz_add ( mainVar , HELPER , TWO ) )
    {
        AVLtree<std::string>* cycle_checker_tree = new AVLtree<std::string>();

        mpz_set         ( HELPER  , mainVar );
        mpz_mul         ( mainVar , mainVar , THREE );
        mpz_add         ( mainVar , mainVar , ONE );
        mpz_tdiv_q_2exp ( mainVar , mainVar , mpz_scan1 ( mainVar , 1 ));

        for( int checker = 2000000000 ; mpz_cmp ( mainVar , HELPER ) >= 0 ; --checker)
        {
            if(!checker)
            {
                const char* temp = mpz_get_str ( NULL , 16 , mainVar );
                std::string umar(temp);
                delete [] temp;

                if(cycle_checker_tree->insert(umar))
                {
                    mpz_mul         ( mainVar , mainVar , THREE);
                    mpz_add         ( mainVar , mainVar , ONE);
            	    mpz_tdiv_q_2exp ( mainVar , mainVar , mpz_scan1 ( mainVar , 1 ));
                    checker = 100000000;
                }
                else
                {
                    std::cout << mpz_get_str( NULL,10, mainVar ) << std::endl;
                    return 0;
                }
            }
            else
            {
                mpz_mul         ( mainVar , mainVar , THREE );
                mpz_add         ( mainVar , mainVar , ONE );
            	mpz_tdiv_q_2exp ( mainVar , mainVar , mpz_scan1 ( mainVar , 1)  );
            }
        }
        delete cycle_checker_tree;  
    }
    //printing the time our range took and the numbers we covered between
    auto t2 = Clock::now();
    std::cout << "Delta t2-t1: " 
              << std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count()
              <<           " seconds "           << std::endl;
    std::cout <<             strStar             << std::endl;
    std::cout << mpz_get_str( NULL,10, mainVar ) << std::endl;
    //here we check if the nums file is empty and jump to start of main if not empty
    if(!check_file())
    {
        goto Begining;
    }
    else
    {
        return 0;
    }
}

