# 指针引用

声明对指针的引用的方式与声明对对象的引用差不多。对指针的引用是一个可像常规指针一样使用的可修改的值。此代码示例演示了使用指向指针的指针与使用对指针的引用之间的差异。虽然 Add1 和 Add2 函数的调用方式不同，但它们在功能上是等效的。二者的区别是，Add1 使用双间接寻址，而 Add2 利用了对指针的引用的便利性。

```cpp
// references_to_pointers.cpp
// compile with: /EHsc

#include <iostream>
#include <string>

// C++ Standard Library namespace
using namespace std;

enum {
   sizeOfBuffer = 132
};

// Define a binary tree structure.
struct BTree {
   char  *szText;
   BTree *Left;
   BTree *Right;
};

// Define a pointer to the root of the tree.
BTree *btRoot = 0;

int Add1( BTree **Root, char *szToAdd );
int Add2( BTree*& Root, char *szToAdd );
void PrintTree( BTree* btRoot );

int main( int argc, char *argv[] ) {
   // Usage message
   if( argc < 2 ) {
      cerr << "Usage: " << argv[0] << " [1 | 2]" << "\n";
      cerr << "\nwhere:\n";
      cerr << "1 uses double indirection\n";
      cerr << "2 uses a reference to a pointer.\n";
      cerr << "\nInput is from stdin. Use ^Z to terminate input.\n";
      return 1;
   }

   char *szBuf = new char[sizeOfBuffer];
   if (szBuf == NULL) {
      cerr << "Out of memory!\n";
      return -1;
   }

   // Read a text file from the standard input device and
   //  build a binary tree.
   while( !cin.eof() )
   {
      cin.get( szBuf, sizeOfBuffer, '\n' );
      cin.get();

      if ( strlen( szBuf ) ) {
         switch ( *argv[1] ) {
            // Method 1: Use double indirection.
            case '1':
               Add1( &btRoot, szBuf );
               break;
            // Method 2: Use reference to a pointer.
            case '2':
               Add2( btRoot, szBuf );
               break;
            default:
               cerr << "Illegal value '"
                  << *argv[1]
                  << "' supplied for add method.\n"
                     << "Choose 1 or 2.\n";
               return -1;
         }
      }
   }
   // Display the sorted list.
   PrintTree( btRoot );
}

// PrintTree: Display the binary tree in order.
void PrintTree( BTree* MybtRoot ) {
   // Traverse the left branch of the tree recursively.
   if ( MybtRoot->Left )
      PrintTree( MybtRoot->Left );

   // Print the current node.
   cout << MybtRoot->szText << "\n";

   // Traverse the right branch of the tree recursively.
   if ( MybtRoot->Right )
      PrintTree( MybtRoot->Right );
}

// Add1: Add a node to the binary tree.
//       Uses double indirection.
int Add1( BTree **Root, char *szToAdd ) {
   if ( (*Root) == 0 ) {
      (*Root) = new BTree;
      (*Root)->Left = 0;
      (*Root)->Right = 0;
      (*Root)->szText = new char[strlen( szToAdd ) + 1];
      strcpy_s((*Root)->szText, (strlen( szToAdd ) + 1), szToAdd );
      return 1;
   }
   else {
      if ( strcmp( (*Root)->szText, szToAdd ) > 0 )
         return Add1( &((*Root)->Left), szToAdd );
      else
         return Add1( &((*Root)->Right), szToAdd );
   }
}

// Add2: Add a node to the binary tree.
//       Uses reference to pointer
int Add2( BTree*& Root, char *szToAdd ) {
   if ( Root == 0 ) {
      Root = new BTree;
      Root->Left = 0;
      Root->Right = 0;
      Root->szText = new char[strlen( szToAdd ) + 1];
      strcpy_s( Root->szText, (strlen( szToAdd ) + 1), szToAdd );
      return 1;
   }
   else {
      if ( strcmp( Root->szText, szToAdd ) > 0 )
         return Add2( Root->Left, szToAdd );
      else
         return Add2( Root->Right, szToAdd );
   }
}

/**
Usage: references_to_pointers.exe [1 | 2]

where:
1 uses double indirection
2 uses a reference to a pointer.

Input is from stdin. Use ^Z to terminate input.
 **/
```
