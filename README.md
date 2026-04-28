# RBT-CSE2331 :deciduous_tree:  
Red-Black Tree - Foundations 2 Final Project   
Implemented in C    

rbt.c : Contains Red-Black Tree implementation, with test suite method implementations called in main.  
## Performance Analysis  
### Insertion:
Insertion requires two processes: finding the insertion point, then fixing the tree to adhere to Red-Black Tree properties (the methods BSTInsert() and fixInsert() accomplish this in my implementation, with insert() serving as a wrapper).  
To do this, a maximum of two rotations are required for rebalancing, and recoloring may be required from the leaves to the root.  
Performing the recoloring will take a maximum of O(log n) steps, since the height of the tree is log n.  
Thus, insertion has a worst-case time complexity of O(log n).  
### Deletion:
Comparably to insertion, deletion requires two processes: deletion for a BST, then fixing the tree to adhere to Red-Black Tree properties (delete() and fixDelete() accomplish this).  
A maximum of three deletions are required for a single deletion. This is bounded by the height of the tree.  
Therefore, the worst-case time complexity of deletion is O(log n).
### Searching:
The time complexity of the search operation depends on the height of the tree. The height is guaranteed to be O(log n), since Red-Black trees ensure that the longest path is less than or equal to twice the length of the shortest path.  
Consequently, searching has a worst-case time complexity of O(log n).  
### Performance Comparison to Other Structures
#### BST Trees:  
Unlike Red-Black Trees, BSTs can be very skewed, where the height of the tree is equal to n. As a result, searching, inserting, and deleting in BSTs run in O(n) time in the worst-case.  
#### AVL Trees:  
AVL trees are strictly balanced, while Red-Black trees are not entirely height balanced. Further, AVL trees require many rotations to maintain balance, while Red-Black trees require a maximum of two rotations for insertion and three rotations for deletion. Like Red-Black trees, AVL trees searching, inserting, and deleting operations have a time complexity of O(log n). Although, due to its strict balancing requirements, AVL trees may take longer to complete insertions and deletions than Red-Black trees.  
## Documentation - Algorithm Explanations
#### Data Structure:  
Nodes are created using a struct, with a pointer to left, right, and pointer. And enum is used to define and assign color (red or black).  
### Basic Operations  
#### Rotation: 
leftRotate() and rightRotate() handle tree rotation operations that are crucial to rebalancing.
#### Insertion:  
The insertion operation is broken into three methods:
1) insert(): a wrapper function handling the entire process
2) BSTInsert(): uses BST logic for inserting a node
3) fixInsert(): a method to fix the violations of RBT properties caused by inserting a node using BST logic
#### Deletion:
The deletion operation is broken into two methods:
1) delete(): traverses the tree to find the desired node, adjusts references to delete it, and frees memory as would be done in a BST
2) fixDelete(): as in fixInsert(), fixes the violations of RBT properties caused by deleting a node using BST logic
#### Search:
search() recursively traverses the tree to find the desired node.
#### Traversal:
An inorder mode is used in inorder(), which prints the data of each node in left, root, right order.
### Helper Methods  
1) minNode(): returns a pointer to the minimum node
2) replaceNode(): effectively swaps nodes in the tree
3) freeTree(): frees the memory allocated in malloc, travels from leaves to root
4) getBlackHeight(): returns the height of a black node
5) verifyRB(): verifies the validity of a Red-Black Tree by ensuring its properties are adhered too. This method was crucial in testing.  
  
runTestSuite(): five testing methods are included here, which are run altogether in the main method. All tests pass.
