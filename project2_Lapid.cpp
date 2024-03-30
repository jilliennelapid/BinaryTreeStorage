// project2_lapid.cpp | Jillienne Lapid | COP3415.04 | 03/29/2024
#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <string.h>
#include <string>
#include <cmath>

FILE *outfile = fopen("out.txt", "w");

// Classes (C)
// C: Item Node Class
/// Nodes that will be used for the smaller trees, holding animal population data.
class itemNode
{
  public:
    char name[50];
    int count;
    itemNode *left, *right;

    itemNode()
    {
      name[0] = '\0';
      count = 0;
      left = NULL;
      right = NULL;
    }

    itemNode(char itemName[], int population)
    {
      strcpy(name, itemName);
      count = population;
      left = NULL;
      right = NULL;
  }
};

// C: Names Tree Class
/// Represents a node in the main BST that holds the root nodes for the other trees.
class treeNameNode
{
  public:
    char treeName[50];
    treeNameNode *left, *right;
    itemNode *theTree;

    treeNameNode()
    { 
      treeName[0] = '\0';
      theTree = NULL;        // Pointer to the root node of the item tree associated with this node.
      left = NULL;
      right = NULL;
    }

    treeNameNode(char name[])
    {
      strcpy(treeName, name);
      theTree = NULL;
      left = NULL;
      right = NULL;
    }

    treeNameNode* insertN(treeNameNode* root, char name[]);
    treeNameNode* buildNameTree(FILE* input, int iter, treeNameNode* root);
    void inOrder(treeNameNode* root);
};

// Item Node Functions (IF)
// IF: Helps to ensure that the Item Trees are alphebetized.
/// Returns true if the name comes earlier in the alphabet,
/// Returns false is the name comes later in the alphabet.
int alphabetize(itemNode* itemRoot, char newName[])
{
  char itemName[50];

  strcpy(itemName, itemRoot->name);

  return strcmp(newName, itemName);
}

// IF: Inserts a new node into an Item Tree.
itemNode* insertI(itemNode* root, char itemName[], int pop)
{
  if(root == NULL)
  {
    itemNode* newNode = new itemNode(itemName, pop);
    return newNode;
  }

  // alphabetize() ensures that the new node is inserted in the correct subtree.
  int result = alphabetize(root, itemName);

  // result == 0 indicates that a node already exists with the desired name.
  // result < 0 indicates that the node goes on the left.
  // result > 0 indicates that the node goes on the right.
  if(result == 0)
  {
    std::cout << "Error: A node with the name " << itemName << " already exists in the Names Tree." << std::endl;
    fprintf(outfile, "%s%s%s\n", "Error: A node with the name ", itemName," already exists in the Names Tree.");
    return root;
  }
  else if(result < 0)
  {
    root->left = insertI(root->left, itemName, pop);
  }
  else
  {
    root->right = insertI(root->right, itemName, pop);
  }

  return root;
}

// IF: Prints the items in a tree out in order.
void inOrder(itemNode* root)
{
  if(root == NULL) 
  {
    return;
  } 

  // Recurs through the left and right subtrees,
  /// and prints the name of the node along the way.
  inOrder(root->left);

  std::cout << root->name << " ";
  fprintf(outfile, "%s ", root->name);

  inOrder(root->right);
}

// Names Tree Functions (NF)
// NF: Prints out the Names Tree in inOrder traversal.
void treeNameNode::inOrder(treeNameNode* root)
{
  if(root == NULL) 
  {
    return;
  } 

  // Recurs through the left and right subtrees,
  /// and prints the name of the node along the way.
  inOrder(root->left);

  std::cout << root->treeName << " ";
  fprintf(outfile, "%s ", root->treeName);

  inOrder(root->right);
}

// NF: Helps to ensure that the Name Tree is alphebetized
/// Returns true if the name comes earlier in the alphabet,
/// Returns false is the name comes later in the alphabet.
int alphabetize(treeNameNode* namesRoot, char newName[])
{
  char rootName[50];

  strcpy(rootName, namesRoot->treeName);

  return strcmp(newName, rootName);
}

// NF: Inserts a new node into the Names Tree.
treeNameNode* treeNameNode::insertN(treeNameNode* root, char name[])
{
  if(root == NULL)
  {
    treeNameNode* newNode = new treeNameNode(name);
    return newNode;
  }

  // alphabetize() ensures that the new node is inserted in the correct subtree.
  int result = alphabetize(root, name);

  // result == 0 indicates that a node already exists with the desired name.
  // result < 0 indicates that the node goes on the left.
  // result > 0 indicates that the node goes on the right.
  if(result == 0)
  {
    std::cout << "Error: A node with the name " << name << " already exists in the Names Tree." << std::endl;
    fprintf(outfile, "%s%s%s\n", "Error: A node with the name ", name," already exists in the Names Tree.");
    return root;
  }
  else if(result < 0)
  {
    root->left = insertN(root->left, name);
  }
  else
  {
    root->right = insertN(root->right, name);
  }

  return root;
}

// NF: Handles the directing to build the Names Tree.
/// Uses fscanf() to read the Names Tree node name from the input file, saves it to a char array,
/// and if the read was successful the node is inserted into the Names Tree with insertN().
treeNameNode* treeNameNode::buildNameTree(FILE* input, int iter, treeNameNode* root)
{
  // Iterates the amount of times given by the integer listed at the top of in.txt
  for(int i = 0; i < iter; i++)
  {
    char treeNodeN[50];    // Stores the Names Tree node name.

    // Attains data from the .txt file.
    int read = fscanf(input, "%s", treeNodeN);

    // If read was successful, sends data to be added to the Names Tree with insertN().
    if(read == 1)
    {
      if(root == NULL)
      {
        root = insertN(root, treeNodeN);
      }
      else
      {
        insertN(root, treeNodeN);
      }
    }
    else
    {
      std::cout << "Failure to read in.txt" << std::endl;
      fprintf(outfile, "%s\n", "Failure to read in.txt");

      return root;
    }
  }
  return root;
}
  
// NF: Finds a node in the Names Tree given a char, then returns the found node.
/// Uses recursion and strcmp() to iterate through the whole Names Tree, and if there is
/// a name match then that matched node is returned.
treeNameNode* searchNameNode(treeNameNode* root, char treeName[50])
{
  if(root == NULL)
  {
    return NULL;
  }

  // If strcmp() == 0, then the desired node has been found. Return that node.
  if(strcmp(treeName, root->treeName) == 0) 
  {
    return root;
  } 
  // Else, if the search name is earlier in the alphabet than the current nodes name, go to the left.
  else if(strcmp(treeName, root->treeName) < 0) 
  {
    return searchNameNode(root->left, treeName);
  } 
  // Otherwise, the search name comes later in the alphabet, so go right.
  else 
  {
    return searchNameNode(root->right, treeName);
  }
}

// F: Handles the directing to build the Item Trees.
/// Uses fscanf() to read the Names Tree node name, Item Tree node name, and count of an item from the input file,
/// saving the read data to a char array. If the read was successful, the node is inserted into the given Item Tree 
/// with insertI().
treeNameNode* buildItemTree(FILE* input, int iter, treeNameNode* namesRoot)
{
  // Iterates the amount of times given by the integer listed at the top of in.txt
  for(int i = 0; i < iter; i++)
  {
    char treeNodeN[50];    // Stores the Names Tree node name.
    char itemNodeN[50];    // Stores an Item Tree node name.
    int population = 0;    // Stores the count of a given item.

    // Attains data from the .txt file.
    int read = fscanf(input, "%s %s %d", treeNodeN, itemNodeN, &population);

    // If read was successful, sends data to be added to an Item Tree with insertI().
    if(read == 3)
    {
      // Uses searchNameNode() to find the node of the correct item tree to add to.
      treeNameNode* branchNode = searchNameNode(namesRoot, treeNodeN);

      if(branchNode->theTree == NULL)
      {
        branchNode->theTree = insertI(branchNode->theTree, itemNodeN, population);
      }
      else
      {
        insertI(branchNode->theTree, itemNodeN, population);
      }
    }
    else
    {
      std::cout << "Error in reading input file. Ensure data is in (%s %s %d) format." << std::endl;
      fprintf(outfile, "%s\n", "Error in reading input file. Ensure data is in (%s %s %d) format.");
    }
  }
  
  return namesRoot;
}

// Query Functions (QF)
// QF: Searches for a particular item in a given tree.
/// If the item is found, displays the count of the item,
/// otherwise displays that the item is not found/tree does not exist.
void search(itemNode* root, char treeName[], char itemName[])
{
  // If root is NULL, the item does not exist in the item tree.
  if(root == NULL)
  {
    std::cout << itemName << " not found in " << treeName << std::endl;
    fprintf(outfile, "%s%s%s\n", itemName," not found in ", treeName);
    return;
  }

  // If strcmp() == 0, itemName matched a node name. The item was found in the tree.
  if(strcmp(root->name, itemName) == 0)
  {
    std::cout << root->count << " " << itemName << " found in " << treeName << std::endl;
    fprintf(outfile, "%d%s%s%s%s\n", root->count, " ", itemName," found in ", treeName);
    return;
  }

  // Otherwise, we recur on the left or right subtree to iterate through the whole tree.
  /// If strcmp() < 0, go left for earlier in the alphabet.
  /// If strcmp() > 0, go right for later in the alphabet.
  if(strcmp(itemName, root->name) < 0)
  {
    search(root->left, treeName, itemName);
  }
  else
  {
    search(root->right, treeName, itemName);
  }
}

// QF: Returns a value representing how many nodes come before a given node in an item tree.
/// Using recursion and strcmp(), a variable count keeps track of the amount of iterations until
/// the desired item is found. Then returns that count value.
int item_before(itemNode* root, char itemName[]) 
{
  if(root == nullptr) 
  {
    return 0;
  }

  int count = 0;

  // If the name we are checking for comes after the current name, 
  /// counts all left nodes + 1, and begins search on right nodes.
  if(strcmp(itemName, root->name) > 0) 
  {
    count = 1 + item_before(root->left, itemName);
    count += item_before(root->right, itemName);
  }
  // If a match is found, we count just the left subtree nodes.
  else if(strcmp(itemName, root->name) == 0) 
  {
    count = item_before(root->left, itemName);
  }
  // Same for if the name we are checking comes before the current name.
  else 
  {
    count = item_before(root->left, itemName);
  }

  return count;
}

// Helper function to height_balance, traverses through the left subtree.
int traverse_left(itemNode* root)
{
  if(root == NULL)
  {
    return 0;
  }

  // Counts the number of times it goes through the left and right subtrees of a subtree.
  int leftSub = traverse_left(root->left);
  int rightSub = traverse_left(root->right);

  // Condensed return, returns the greater of the two sub-subtrees + 1 to account for the root node.
  return (leftSub > rightSub) ? leftSub+1 : rightSub+1;
}

// Helper function to height_balance, traverses through the right subtree.
int traverse_right(itemNode* root)
{
  if(root == NULL)
  {
    return 0;
  }

  // Counts the number of times it goes through the left and right subtrees of a subtree.
  int leftSub = traverse_right(root->left);
  int rightSub = traverse_right(root->right);

  // Condensed return, returns the greater of the two sub-subtrees + 1 to account for the root node.
  return (leftSub > rightSub) ? leftSub+1 : rightSub+1;
}

// QF: Checks if a given item tree is balanced or not.
/// Uses the helper functions traverse_left() and traverse_right() to get the counts of the left and right subtrees.
/// Finds the difference of those values: Equal to 0, balanced; not equal to 0, not balanced.
void height_balance(itemNode* root, char treeRootName[])
{
  if(root == NULL)
  {
    std::cout << "Tree is empty." << std::endl;
    fprintf(outfile, "%s\n", "Tree is empty");
  }

  // traverse_left() and traverse_right() track the height of the subtrees via recursion.
  int lcount = traverse_left(root->left);
  int rcount = traverse_right(root->right);

  // Take the absolute value of the difference.
  int difference = abs(lcount - rcount);

  // Display the result.
  std::cout << treeRootName << ": Left Height " << lcount << ", Right Height " << rcount << ", Difference " << difference;
  fprintf(outfile, "%s%s%d%s%d%s%d ", treeRootName, ": Left Height ", lcount, ", Right Height ", rcount, ", Difference ", difference);

  if(difference != 0)
  {
    std::cout << ", Not Balanced" << std::endl;
    fprintf(outfile, "%s\n", "Not Balanced");
  }
  else
  {
    std::cout << ", Balanced" << std::endl;
    fprintf(outfile, "%s\n", "Balanced");
  }
}

// QF: Returns the sum of all counts of all items in a given tree.
/// Uses recursion to iterate through all nodes so that their counts can be summed.
int count(itemNode* root, int c)
{
  if(root == NULL)
  {
    return 0;
  }

  // Stores the counts of the item nodes in the left and right subtrees of the item tree.
  int leftC = count(root->left, c);
  int rightC = count(root->right, c);

  // Adds the left and right subtree counts + the last node's count to get the total.
  int total = leftC + rightC + root->count;

  // Returns the count.
  return total;
}

// F: Handles which queries need to run based on the input file in.txt
/// Depending on the name of the query, the function will read 3 total lines for search() and item_before()
/// and 2 total lines for height_balance() and count(). The lines are read using fscanf().
void queryCommand(FILE* input, int qCount, treeNameNode* namesRoot)
{
  // Iterates the amount of times given by the integer listed at the top of in.txt
  for(int i = 0; i < qCount; i++)
  {
    char queryType[50];    // Stores word that determins the query type
    char treeNodeN[50];    // Stores the Names Tree node name 
    char itemNodeN[50];    // Stores an Item Tree noe name

    // Reads the first word which determines the type of query.
    int read1 = fscanf(input, "%s", queryType);

    // If the read was successful:
    if(read1 == 1)
    {
      // If the query was search() or item_before():
      if(strcmp(queryType, "search") == 0 || strcmp(queryType, "item_before") == 0)
      {
        // Read two words which will be the Names Tree and Item Tree node names.
        int read2 = fscanf(input, "%s %s", treeNodeN, itemNodeN);

        if(read2 == 2)
        {
          // Find the respective Names Tree node pointing to the desired item tree.
          treeNameNode* itemNameRoot = searchNameNode(namesRoot, treeNodeN);

          // Pass to search()
          if(strcmp(queryType, "search") == 0 && itemNameRoot != NULL)
          {
            search(itemNameRoot->theTree, treeNodeN, itemNodeN);
          }
          // Pass to item_before()
          else if(strcmp(queryType, "item_before") == 0)
          {
            int countBefore = item_before(itemNameRoot->theTree, itemNodeN);
            std::cout << "Items before " << itemNodeN << ": " << countBefore << std::endl;
            fprintf(outfile, "%s%s%s%d\n", "Items before ", itemNodeN, ": " , countBefore );
          }
          // Otherwise, the Names Tree node doesn't exist.
          else
          {
            std::cout << treeNodeN << " does not exist." << std::endl;
            fprintf(outfile, "%s%s\n", treeNodeN, " does not exist.");
          }
        }
      }
      // Else, if the query was height_balance() or count():
      else if(strcmp(queryType, "height_balance") == 0 || strcmp(queryType, "count") == 0)
      {
        // Read one word which is the Names Tree node name.
        int read3 = fscanf(input, "%s", treeNodeN);

        if(read3 == 1)
        {
          // Find the respective Names Tree node pointing to the desired item tree.
           treeNameNode* itemNameRoot = searchNameNode(namesRoot, treeNodeN);

          // Pass to height_balance()
          if(strcmp(queryType, "height_balance") == 0)
          {
            height_balance(itemNameRoot->theTree, treeNodeN);
          }
          // Pass to count()
          else if(strcmp(queryType, "count") == 0)
          {
            int countTotal = count(itemNameRoot->theTree, 0);
            std::cout << treeNodeN << " count: " << countTotal << std::endl;
            fprintf(outfile, "%s%s%d\n", treeNodeN, " count: ", countTotal);
          }
          // Otherwise, the Names Tree node doesn't exist.
          else
          {
            std::cout << treeNodeN << " does not exist." << std::endl;
            fprintf(outfile, "%s%s\n", treeNodeN, " does not exist.");
          }
        }
      }
      // Handles if reading for the query command was unsuccessful.
      else
      {
        std::cout << "Error in reading input file. Ensure queries are written correctly." << std::endl;
        fprintf(outfile, "%s\n", "Error in reading input file. Ensure queries are written correctly.");
      }
    
    }
  }
}

// F: Prints out the categories in the tree starting with the Names Tree.
/// The user can choose between the smaller trees where the items will be printed.
void traverse_in_traverse(treeNameNode* root)
{
  treeNameNode* rootCopy = root;

  // Prints the Names Tree nodes to choose from
  rootCopy->inOrder(rootCopy);

  // Saves user input
  char response[50];

  // While the user does not type in '---' to quit searching, allow the user to type in the item tree they want to see.
  while(strcmp(response, "---") != 0)
  {
    std::cout << "\nType in the tree to traverse: " << "To Quit: '---'" << std::endl;
    std::cin >> response; 

    // Checks if the tree requested is in Names Tree.
    if(searchNameNode(rootCopy, response) != NULL)
    {
      // Find the respective Names Tree node pointing to the desired item tree.
      treeNameNode* nameRoot = searchNameNode(root, response);

      // Print the item tree contents.
      inOrder(nameRoot->theTree);
    }
    // Otherwise, there is not an item tree with the desired user input name.
    else if(searchNameNode(rootCopy, response) == NULL && strcmp(response, "---") != 0)
    {
      std::cout << "Tree does not exist.";
      fprintf(outfile, "%s\n", "Tree does not exist");
    }

    std::cout << "\n-----------------------------------------";
    fprintf(outfile, "\n%s\n", "-----------------------------------------");
  }

  std::cout << "\nUser Search Ending.\n" << std::endl;
}

int main() 
{
  treeNameNode* nameTreeRoot = NULL;
  
  int treeNameCount = 0;    // Integer representing the number of lines pertaining to filling the Names Tree.
  int itemCount = 0;        // Integer representing the number of lines pertaining to filling the Item Trees.
  int queryCount = 0;       // Integer representing the number of lines pertaining to queries.

  // 1) Initializes a pointer of type FILE to allow access to in.txt 
  /// and for assisting in reading lines successively.
  FILE *infile = fopen("in.txt", "r");

  // 2) Reads the first 3 integers in the file that determine the amount of nodes in the Names Tree,
  /// the amount of items in total are being added to different trees, and the amount of queries.
  if(infile != NULL)
  {
    int read = fscanf(infile, "%d %d %d", &treeNameCount, &itemCount, &queryCount);

    if(read != 3)
    {
      std::cout << "Error reading in.txt" << std::endl;
      fprintf(outfile, "%s\n", "Error reading in.txt");
    }
  }

  // 3) Create the Names Tree
  nameTreeRoot = nameTreeRoot->buildNameTree(infile, treeNameCount, nameTreeRoot);

  // 4) Create the Item Trees
  nameTreeRoot = buildItemTree(infile, itemCount, nameTreeRoot);

  // 5) Runs traverse_in_traverse(), allowing users to type in a specific item tree to print out.
  std::cout << "Start of Tree Display\n" << "------------------------" << std::endl;
  fprintf(outfile, "%s\n%s\n", "Start of Tree Display", "------------------------");
  traverse_in_traverse(nameTreeRoot);

  // 6) Required Queries, automatically run after the user input to display the item trees.
  std::cout << "Start of Queries\n" << "------------------------" << std::endl;
  fprintf(outfile, "%s\n%s\n", "Start of Queries", "------------------------");
  queryCommand(infile, queryCount, nameTreeRoot);
  
  // 7) Closes in.txt and out.txt at the end of the program.
  fclose(infile);
  fclose(outfile);
}