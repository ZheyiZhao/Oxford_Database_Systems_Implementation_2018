//
//  main.cpp
//  CoverDB
//
//  Created by Charlotte Zhao on 06/04/2018.
//  Copyright © 2018 Zheyi Zhao. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <time.h>

#include<mach/mach.h>
struct task_basic_info t_info;
mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;

using namespace std;

const int numNode = 40; // define the size of the variable order
const int numAttributes = 7; // number of attributes in R
#include <stdio.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#define BUFFERLEN 128
#include <unistd.h>
#include <ios>
#include <iostream>
#include <fstream>
#include <string>


//-----------------------------------------------------------------
// Node
//
// Purpose: The unit for storing information in fatorised database
//
// Simplification: the fatorised database has a binary structure and
// each node has two children of vector of Nodes.
//
// Variables: val - value stored in the database
//          left - pointer to the left child which is a vector of Nodes
//          right - pointer to the right child which is a vector of Nodes
//-----------------------------------------------------------------


struct Node{
    int val;
    vector<Node*> left;
    vector<Node*> right;
    Node(int x): val(x),left(NULL),right(NULL){}
};

//-----------------------------------------------------------------
// VarOrder
//
// Purpose: The unit for variable order
//
// Simplification: the varible order is represented via a binary tree
// instead of a forest
//
// Variables: val - name of the attribute
//            num - position of the attribute in the tree
//            left - a pointer to the left child
//            right - a pointer to the right child
//-----------------------------------------------------------------

struct VarOrder{
    char val;
    int num;
    VarOrder* left;
    VarOrder* right;
    VarOrder(char x): val(x),left(NULL),right(NULL){}
};

//-----------------------------------------------------------------
// FindVarOrder
//
// Purpose : Find the pointer to the node of the given attribute
//           in the variable order .
// Input   : root - the root of the variable order tree
//           x - the given attribute
// Output : the pointer to the node of the given attribute
//-----------------------------------------------------------------

VarOrder* FindVarOrder(VarOrder* root,char x){
    VarOrder* res = NULL;
    
    if (root == NULL)
        return root;
    if (root->val == x){ // if the root contains the given attribute, return the root
        return root;
    }
    res = FindVarOrder(root->left,x); // recursively searching in the left sub-tree
    if (res != NULL) // found
        return res;
    else {
        res = FindVarOrder(root->right,x); // recursively searching in the right sub-tree
        return res;
        
    }
}
//-----------------------------------------------------------------
// BuildRelationMap
//
// Purpose : Build relation map according to the input decomposition.
// Input   : decomposition - input of CoverDB.
// Output : the map of relations
//-----------------------------------------------------------------
map<char,set<char>> BuildRelationMap(vector<vector<char>> decomposition){
    map<char,set<char>> mp; // a map from one attribute to its relating attributes
    // for every element in the given decomposition
    for (int i = 0; i < decomposition.size(); i++)
        for (int j = 0; j< decomposition[i].size(); j++)
            // find other attributes in the same relation
            for (int k = 0; k < decomposition[i].size(); k++)
                if (j != k ) // not itself
                    mp[decomposition[i][j]].insert(decomposition[i][k]); // insert into the mapping
    return mp;
}


//-----------------------------------------------------------------
// BuildAttributeList
//
// Purpose : Extract the ATTRs from the query
// Simplification: All the attributes are represented by alphabets
// Input   : query - the given query that contains attributes
// Output : a vector of pointers to the attributes in the variable order
//-----------------------------------------------------------------

vector<VarOrder*> BuildAttributeList(string query){
    vector<VarOrder*> res;
    int begin = (int)query.find(" ") + 1; // position after "SELECT "
    int end = (int)query.find("SUM") - 1; // position before "SUM"
    
    string attr = query.substr(begin,end-begin); // sub string between SELECT and SUM
    
    for (int i = 0; i < attr.length(); i++)
        if (attr[i] >= 'A' && attr[i] <= 'Z') // alphabets
        {
            VarOrder* v = new VarOrder (attr[i]); // create a new node in the variable order
            res.push_back(v); // store the node in the vector
        }
    return res;
}
//-----------------------------------------------------------------
// FindEXPR
//
// Purpose : Extract the EXPR from the query
// Simplification: An EXPR is represented by one alphabet
// Input   : query - the given query that contains attributes
// Output : a pointer to EXPR in the variable order
//-----------------------------------------------------------------
VarOrder* FindEXPR(string query){
    int begin = (int)query.find("SUM(") + 1; // position after "SUM("
    string EXPR = query.substr(begin,1); // extract the sub string after "SUM("
    VarOrder* v = new VarOrder (EXPR[0]); // Get the first character of the sub string and create a node in the variable order
    return v; // return this node
}

//-----------------------------------------------------------------
// SameRelation
//
// Purpose : Determine whether all the attributes in the query belong to the same relation
// Input   : mp - mapping of relations
//          attributes - a vector that contains all the attributes in the query
//           decomposition - input of CoverDB.
// Output : a boolean value
//-----------------------------------------------------------------

bool SameRelation(map<char,set<char>> mp,vector<VarOrder*> attributes,vector<vector<char>> decomposition){
    // for each relation, build a map that maps from the first attribute to the rest of attributes
    for (auto a:attributes){
        set<char> relation = mp[a->val];
        for (auto r:relation){
            for (auto b:attributes){
                if ( a != b && b->val == r) // found
                    return true;
            }
        }
            
    }
        
       return false;
}

//-----------------------------------------------------------------
// InAttr
//
// Purpose : Determine whether an attribute is in the attribute list
// Input   : attributes - a vector that contains all the attributes in the query
//           x - an input attribute .
// Output : a boolean value
//-----------------------------------------------------------------

bool InAttr(vector<VarOrder*> attributes,char x){
    for (auto a:attributes)
        if (x == a->val) // in the list
            return true;
    return false; // not in the list
}
//-----------------------------------------------------------------
// NotRoot
//
// Purpose : Determine whether an attribute can be the root of the variable order
// Input   : attr - the value of the input attribute
//           decomposition - input for CoverDB .
// Output : a boolean value
//-----------------------------------------------------------------
bool NotRoot(char attr,vector<vector<char>> decomposition){
    for (int i = 0; i < decomposition.size();i++){
        // if the attribute is in the relation of size 2
        if ( decomposition[i].size() == 2)
            for (int j = 0; j < decomposition[i].size(); j++){
                if (decomposition[i][j] == attr)
                    return true;
            }
        }
    return false;
}
//-----------------------------------------------------------------
// FindVarNode
//
// Purpose : Find a node in the variable order.
// Input   : root - the root of the variable order.
//           x - the value of the node to be found.
//           res - the result of finding
// Output :  none
//-----------------------------------------------------------------
void FindVarNode(VarOrder*& root, char x,VarOrder*& res){
    if (root == NULL)
        return ;
    if (root->val == x) // if the root is the target node
        res = root;
    FindVarNode(root->left,x,res); // recursively search the left sub tree
    FindVarNode(root->right,x,res);// recursively search the right sub tree
}
//-----------------------------------------------------------------
// FindVarOrder_BFS
//
// Purpose : Find a node in the variable order in a Breadth-First-Search way
// Input   : root - the root of the variable order.
//           relation - the set of relation whose positions are to be found
//           res - the result of finding
// Output :  none
//-----------------------------------------------------------------
void FindVarOrder_BFS(VarOrder*& root,set<char> relation,vector<VarOrder*>& res){
    if (root == NULL)
        return ;
    if (relation.find(root->val) != relation.end()){ // found the attribute in the variable order
        res.push_back(root);
    }
    FindVarOrder_BFS(root->left,relation,res); // left sub-tree
    FindVarOrder_BFS(root->right,relation,res); // right sub-tree
}
//-----------------------------------------------------------------
// FindVarOrder_DFS
//
// Purpose : Find a node in the variable order in a Depth-First-Search way
// Input   : root - the root of the variable order.
//           relation - the set of relation whose positions are to be found
//           res - the result of finding
// Output :  none
//-----------------------------------------------------------------

void FindVarOrder_DFS(VarOrder*& root,set<char> relation,vector<VarOrder*>& res){
    if (root == NULL)
        return ;
    FindVarOrder_DFS(root->left,relation,res); // left sub-tree
    
    if (relation.find(root->val) != relation.end()){ // if the root is the target node
        res.push_back(root);
    }
    FindVarOrder_DFS(root->right,relation,res); // right sub-tree
}
//-----------------------------------------------------------------
// QueryType
//
// Purpose : Determine the type of the query
// Input   : query - string for the input query
// Output :  the type of the query
//-----------------------------------------------------------------
int QueryType(string query){
    int type = 0;
    if (query.find("SUM(1)") !=std::string::npos) // SUM(1)
        type = 1;
    else if  (query.find("GROUP BY") !=std::string::npos){ //GROUP BY CLAUSE
        type = 3;
    }
    else{
        type = 2; // SUM(EXPR)
    }
    return type;
}

//-----------------------------------------------------------------
// InsertEXPR
//
// Purpose : Insert attribute EXPR in the variable order
// Input   : root - the root of the variable order.
//           encode - the position for each attribute in the variable order
//           query - the string for the input query
//           decomposition - the input for CoverDB
// Output :  None
//-----------------------------------------------------------------
void InsertEXPR(VarOrder* &root,vector<int>& encode,string query,vector<vector<char>> decomposition){
    int pos = (int)query.find("SUM(") + 1; // the position in the query string after "SUM("
    char attr = query[pos]; // get EXPR
    
    VarOrder* rootAttr = new VarOrder(attr);
    // insert first node
    root->left = rootAttr;
    root->left->num = 2 * root->num + 1; // position in the variable order
    encode[rootAttr->val - 'A' + 1] = root->left->num;
}
//-----------------------------------------------------------------
// InsertATTR
//
// Purpose : Insert attributes in the variable order
// Input   : root - the root of the variable order.
//           encode - the position for each attribute in the variable order
//           query - the string for the input query
//           decomposition - the input for CoverDB
//           type - the type of the query
// Output :  None
//-----------------------------------------------------------------

void InsertATTR(VarOrder* root,vector<int>& encode,string query,vector<vector<char>> decomposition,int & type){
    // build a list of attributes
    vector<VarOrder*> attributes = BuildAttributeList(query);
    map<char,set<char>> mp = BuildRelationMap(decomposition);
     // if there are more than one attribute
    if (attributes.size() > 1){
        // if the attribues does not belong to the same relation: type 4
        if (!SameRelation(mp,attributes, decomposition))
            type = 4;
        else
            type = 3;
    }
    // determine the root attribute in the variable order
    VarOrder* rootAttr = NULL;
    for (auto a:attributes)
        if (!NotRoot(a->val,decomposition)){ // if the attribute can be a root
            rootAttr = a;
            break;
        }
    // if the attributes belong to the same relation
    if (type == 3){
        VarOrder* matchR;
        // insert first node
        root->left = rootAttr;
        root->left->num = 2 * root->num + 1;
        encode[rootAttr->val - 'A' + 1] = root->left->num;
        matchR = root->left; // the last attribute inserted
        
        // change the order in the decompositions so that they start with the root attribute in the variable order
        for (int i = 0; i < decomposition.size();i++)
            for (int j = 0; j < decomposition[i].size(); j++)
                if (decomposition[i][j] == rootAttr->val && j != 0){
                    int temp =decomposition[i][0];
                    decomposition[i][0] = rootAttr->val;
                    decomposition[i][j] = temp;
                }
        
        
        // insert all the other attributes
        for (auto a:attributes)
            if (a != rootAttr){
                VarOrder* match = FindVarOrder(root,a->val); // find this attribute
                if (match == NULL){ // not found
                    if (matchR->left == NULL){ // insert on the left of the last attribute inserted
                        matchR->left = a;
                        matchR->left->num = 2 * matchR->num + 1; // record the position
                        encode[a->val - 'A' + 1] =   matchR->left->num;
                        matchR = matchR->left;
                        
                    }
                    else{// insert on the right of the last attribute inserted
                        matchR->right = a;
                        matchR->right->num = 2 * matchR->num + 2; // record the position
                        encode[a->val - 'A' + 1] = matchR->right->num;
                        matchR = matchR->right;
                    }
                }
            }
    }
}


//-----------------------------------------------------------------
// InsertAttributes
//
// Purpose : Build variable order by inserting all attributes in R by finding relational attributes in a depth-search-first manner
// Input   : mp - the map of relations.
//           decomposition - input of CoverDB.
//           root - the root of the variable order
//           encode - the vector that contains the positions of all attributes in the variable order
// Output : none
//-----------------------------------------------------------------
void InsertAttributes(map<char,set<char>> mp, vector<vector<char>> decomposition, VarOrder* root,vector<int>& encode){
    
    map<char,set<char>> :: iterator iter;

    for (iter = mp.begin(); iter != mp.end(); iter ++){
        // first attribute to be inserted into the variable order
        // default positiobn for the first attribute is the left child of the root
        if (root->left == NULL){
            root->left = new VarOrder(iter->first);
            root->left->num =root->num * 2 + 1; // the number for the new node
            encode[iter->first - 'A' + 1] = root->left->num; // put the number of the node into a vector
            continue; // next attribute
            
        }
        
        VarOrder* position = NULL; // the position of the current attribute in the variable order
        FindVarNode(root->left,iter->first,position); // find attribute in the order
        if (position == NULL){ // attribute not found: find the attributes it relates to in the order
            set<char> relation = iter->second;
            set<char> :: iterator siter;
            vector<VarOrder*> res;
            FindVarOrder_DFS(root,relation,res); // find the position of other attributes in the same relation in a depth-search-first manner
            for (auto pos:res){ // for every attribute in the relation
                
                if (pos != NULL){ // found
                    if (pos->left == NULL){ // try to insert to the left child first
                        pos->left = new VarOrder(iter->first); // create a new node for the current attribute
                        pos->left->num = pos->num * 2 + 1; // number for the new node
                        encode[iter->first - 'A' + 1] = pos->left->num; // record the number in the vector
                        break;
                    }
                    else if (pos-> right == NULL){ // try the right child if the left child is occupied
                        pos->right = new VarOrder(iter->first); // create a new node for the current attribute
                        pos->right->num = pos->num * 2 + 2;// number for the new node

                        encode[iter->first - 'A' + 1] = pos->right->num; // record the number in the vector
                        break;
                        
                    }
                }
            }
            
        }
        
        
    }
    
}
//-----------------------------------------------------------------
// InsertOtherAttributes
//
// Purpose : Build variable order by inserting all attributes in R except the EXPR  by finding relational attributes in a breadth-search-first manner
// Input   : mp - the map of relations.
//           decomposition - input of CoverDB.
//           root - the root of the variable order
//           encode - the vector that contains the positions of all attributes in the variable order
//           type - the type of the query
// Output : none
//-----------------------------------------------------------------
void InsertOtherAttributes(map<char,set<char>> mp, vector<vector<char>> decomposition, VarOrder* root,vector<int>& encode,int type){

    map<char,set<char>> :: iterator iter;
    
    for (iter = mp.begin(); iter != mp.end(); iter ++){
        
            VarOrder* position = NULL;
            // if the order is empty
            if (root->left == NULL){
                root->left = new VarOrder(iter->first);
                root->left->num = root->num * 2 + 1;
                encode[iter->first - 'A' + 1] = root->left->num;
                continue;
                
            }
            // find the current attribute
            FindVarNode(root->left,iter->first,position);
            if (position == NULL){
                set<char> relation = iter->second;
                set<char> :: iterator siter;
                vector<VarOrder*> res;
                //  finding relational attributes in a depth-search-first manner
                FindVarOrder_BFS(root,relation,res);
                
                for (auto pos:res){
                    
                    if (pos != NULL){
                        if (pos->left == NULL){
                            pos->left = new VarOrder(iter->first);
                            pos->left->num = pos->num * 2 + 1;
                            encode[iter->first - 'A' + 1] = pos->left->num;
                            break;
                        }
                        else if (pos-> right == NULL){
                            pos->right = new VarOrder(iter->first);
                            pos->right->num = pos->num * 2 + 2;
                            encode[iter->first - 'A' + 1] = pos->right->num;
                            break;
                            
                        }
                    }
                }
                
            }
        
        
    }
    
    
}
//-----------------------------------------------------------------
// BuildVariableOrder
//
// Purpose : Build Variable Order Delta according to the given query and decomposition.
// Input   :  encode - vetcor  which contains the position of each attribute in the                               variable order;
//           query - the query sentence;
//           decomposition - input of CoverDB;
//
// Output : the root of the variable order tree
//-----------------------------------------------------------------

VarOrder* BuildVariableOrder(vector<int>& encode, string query,vector<vector<char>> decomposition,int& type){
    // create a root for variable order tree
    // use '?' symbol to distinguish it between other nodes
    VarOrder* root = new VarOrder('?');
    root->num = 0; // the number of the root is zero
    //Build a relation map according to the decomposition
    map<char,set<char>> mp = BuildRelationMap(decomposition);
    
    // type 1: Sum(1)
    if (type == 1){
        // insert all the attribues in R
        InsertAttributes(mp,decomposition,root,encode);
    }
    // type 2: Sum of EXPR in R
    else if (type == 2){
        // insert EXPR first
        InsertEXPR(root,encode, query,decomposition);
        // insert other attribues except EXPR
        InsertAttributes(mp,decomposition,root,encode);
        
    }
    // type 3: Sum of two attributes with group-by clause where the group-by variables can be inserted on the above of other variables
    else if (type == 3)
    {
        // insert ATTR first
        InsertATTR(root,encode,query,decomposition,type);
        if (type == 3)
            // insert other attribues except ATTR
            InsertOtherAttributes(mp,decomposition,root,encode,type);
        // type 4: Sum of two attributes with group-by clause where the group-by variables cannot be inserted on the above of other variables
        else if (type == 4){
            // insert all attributes
            InsertAttributes(mp,decomposition,root,encode);

        }
    }
    
    return root; // the root of the variable order tree
    
}
//-----------------------------------------------------------------
// Split
//
// Purpose : separate the the input tuple string by the delim into vector of integer
// Input   : s - input tuple in the type of string.
//           delim - separator of the tuple
//
// Output : tuple in a vector form
//-----------------------------------------------------------------
vector<int> split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<int> tokens;
    while (getline(ss, item, delim)) { // for each line in the csv file
        tokens.push_back(stoi(item)); // convert string to integer
    }
    return tokens;
}
//-----------------------------------------------------------------
// Find
//
// Purpose : find a Node in the given vector
// Input   : left - input tuple in the type of string.
//           delim - separator of the tuple
//
// Output : tuple in a vector form
//-----------------------------------------------------------------
Node* Find(vector<Node*> left,Node* x){
    if (left.size() == 0)
        return NULL;
    for (auto l:left){
        if (l->val == x->val)
            return l;
    }
    return NULL;
}
//-----------------------------------------------------------------
// FindIndex
//
// Purpose : Return the attribute in the given position in the                               variable order
// Input   : encode - vetcor  which contains the position of each attribute in the                               variable order.
//           x - position of the given attribute in the variable order
//
// Output : the assigned number for the attribute. -1 if not found.
//-----------------------------------------------------------------
int FindIndex(vector<int> encode,int x){
    for (int i = 1; i < encode.size() ; i++){
        if (x == encode[i]) // the attribute has the position x
            return i; // return the assigned number of the attribute
    }
    return -1; // not found
}

//-----------------------------------------------------------------
// BuildFatorisedDB
//
// Purpose : Build a factorised database
// Input   : root - the root of the database
//           tuple - the input tuple from the cover
//           encode - the position of each attribute in the variable order
//
// Output : none
//-----------------------------------------------------------------
void BuildFatorisedDB(Node* root,vector<int> tuple,vector<int> encode){
    
    // build a list of nodes for the input tuples
    vector<Node*> t(numAttributes);
    for (int i = 0; i < tuple.size(); i++){
        t[i+1] =new Node(tuple[i]);
    }

    vector<Node*> start(numNode); // record the match of each level
    start[0] = root;
    
    
    for (int i = 1; i <= numNode - 1; i++){
        int index = FindIndex(encode, i); // encode[] == i
        
        
        if (index != -1 ){
            if (i % 2 != 0){ //the current node should be on the left
                Node* match = NULL;
                match = Find(start[(i - 1)/2]->left,t[index]); // find the match of the last level
                if (match == NULL){ // not found
                    Node* n = t[index]; // create a new node
                    start[(i - 1)/2]->left.push_back(n); // insert to the left
                    start[i] = n; // new match
                }
                else{
                    start[i] = match; // found
                }
            }
            else if (i % 2 == 0){//the current node should be on the right
                Node* match = NULL;
                match = Find(start[(i - 2)/2]->right,t[index]);// find the match of the last level

                if (match == NULL){// not found
                    Node* n = t[index];
                    start[(i - 2)/2]->right.push_back(n); // insert to the right
                    start[i] = n; // new match
                    
                }
                else{
                    start[i] = match; // found
                }
            }
            
        }
        
        
    }
    
    root = start[0];
    
}
//-----------------------------------------------------------------
// SumVector
//
// Purpose : Give the sum of a vector of integers
// Input   : x - input vector
//
// Output : the sum of the vector
//-----------------------------------------------------------------
int SumVector(vector<int> x){
    int sum = 0;
    for (auto i:x)
        sum += i;
    return sum;
}

//-----------------------------------------------------------------
// Count
//
// Purpose : count the number of tuples in the relation using a factorised representation of the cover
// Input   : root - the root of the factorised database
//
// Output : the number of tuples in the relation
//-----------------------------------------------------------------
int Count(vector<Node*> root){
    if (root.size() == 0) // base case
        return 1;
    else{
        int sum = 0;
        // the value of the node = the value of the left child * the value of the right child
        for (auto i:root){
            sum +=  Count(i->left) * Count(i->right); // recursive method
        }
        return sum;
    }
}
//-----------------------------------------------------------------
// SumColumn
//
// Purpose : Give the sum of a column
// Input   : root - the root of the factorised database
//
// Output : the number of tuples in the relation
//-----------------------------------------------------------------
int SumColumn(Node* root,vector<int> encode){
    int sum = 0;
    for (auto a:root->left){
        sum += a->val * Count(a->left) * Count(a->right);
    }
    return sum;
}
//-----------------------------------------------------------------
// Sum
//
// Purpose : Give the sum of the number of tuples represeneted in a subtree
// Input   : root - the root of the factorised database
//           x - specification of the subtree
//
// Output : sum
//-----------------------------------------------------------------

int Sum(Node* root,char x){
    if (x == '1')// sum(1)
        return  Count(root->left) * Count(root->right);
    
    else if (x == 'L'){ // count left sub-tree
        int sum = 0;
        for (auto a:root->left){
            sum +=  Count(a->left);
        }
        return sum;
    }
    else if (x == 'R'){ // count right sub-tree
        int sum = 0;
        for (auto a:root->right){
            sum +=  Count(a->right);
        }
        return sum;
    }
    else return 0;
    
}

//-----------------------------------------------------------------
// GroupByType3
//
// Purpose : Compute the result for type 3 group-by clause query
// Simplification : the number of attributes are two
//                  the structure of the variable order is known
// Input   : root - the root of the factorised database
//           x - specification of the subtree
//
// Output : a map between attributes and sum
//-----------------------------------------------------------------


map<string,int> GroupByType3(Node* root){
    map<string,int> mp;

    vector<int> nums;
    
    // Benchmark group by AB
    for (auto b: root->left){
        nums.push_back(Sum(b,'R')); // sum the right subtree of the first attribute
        
        for (auto a: b->left){
            nums.push_back(Sum(a,'L')); // Get the number of the second attribute
            int sum = 0;
            for (auto e: a->right){
                for (auto f:e->left){ // Sum(F)
                    sum += nums[0] * nums[1] * f->val; // value * number
                }
            }
            string ba = to_string(b->val) + " " + to_string(a->val); // a string represents the selection of attributes

            mp[ba] = sum; // mapping between attributes and sum

        }
    }
    return mp;
    
    
    
}
//-----------------------------------------------------------------
// GroupByType4
//
// Purpose : Compute the result for type 4 group-by clause query
// Simplification : the number of attributes are two
//                  the structure of the variable order is known
// Input   : root - the root of the factorised database
//
// Output : a map between attributes and sum
//-----------------------------------------------------------------
map<string,int> GroupByType4(Node* root){
    map<string,int> mp;

    for (auto a:root->left){
        //get value of F
        int fv = 0;
        for (auto e:a->right)
            for (auto f:e->left)
                fv += f->val;
        
        //get value of C & D
        for (auto b:a->left){
            for (auto c:b->left){
                for (auto d:b->right){
                    string cd = to_string(c->val) + " " + to_string(d->val);
                    mp[cd] += fv;
                }
            }
        }
    }
   
    return mp;
}
//-----------------------------------------------------------------
// Benchmark
//
// Purpose : List the four test cases
// Input   : none
// Output : a vector of queries
//-----------------------------------------------------------------
vector<string> Benchmark(){
    vector<string> queries;
  //  queries.push_back("SELECT SUM(1) FROM R;");
    //queries.push_back("SELECT SUM(A) FROM R;");
    queries.push_back("SELECT A,B, SUM(F) FROM R GROUP BY A,B;");
    queries.push_back("SELECT C,D, SUM(F) FROM R GROUP BY C,D;");
    return queries;
}
int pos1 = -1;
int pos2 = -1;
int pos3 = -1;
struct
{
    bool operator()(const vector< int >& lhs, vector< int >& rhs)
    {
        return ((to_string(lhs[pos1]) < to_string(rhs[pos1])) ||((to_string(lhs[pos1]) == to_string( rhs[pos1])) && (to_string(lhs[pos2]) <  to_string(rhs[pos2])) ));
    }
} ColumnLess;


void StandardDB(){
    
    vector<string> queries = Benchmark() ;

    /* StandardDB*/
    cout << "StandardDB" << endl;
    /* read Cover */
    ifstream R("dsi-exam18/5/R.csv");

    vector<vector<int>> SDB;
    string relation;
    while (getline(R, relation, '\n')){
        vector<int> tuple = split(relation,'|');
        SDB.push_back(tuple);
    }
    
    for (auto q:queries){
        
        int type = QueryType(q);
        if (type == 1){
            int cnt = (int) SDB.size();
            cout << "Answers to query 1" << endl;
            cout << cnt << endl;
        }
        else if (type == 2){
            int begin = (int)q.find("SUM(") + 4; // position before "SUM"
            string attr = q.substr(begin,1); // sub string between SELECT and SUM
            
            int pos = attr[0] - 'A';
            int sum = 0;
            for (int i = 0; i < SDB.size(); i++){
                sum += SDB[i][pos];
            }
            cout << "Answes to query 2" << endl;
            cout << sum << endl;
        }
        else{
            // type 3 or 4
            pos1 = -1;
            pos2 = -1;
            int begin = (int)q.find(" ") + 1; // position after "SELECT "
            int end = (int)q.find("SUM") - 1; // position before "SUM"
            
            string attr = q.substr(begin,end-begin); // sub string between SELECT and SUM
            
            for (int i = 0; i < attr.length(); i++)
                if (attr[i] >= 'A' && attr[i] <= 'Z') // alphabets
                {
                    
                    if (pos1 == -1 )
                    {pos1 = attr[i] - 'A';}
                    else
                        pos2 = attr[i] - 'A';
                }
            
            
            sort(SDB.begin(), SDB.end(),ColumnLess );
            
            int a1 = SDB[0][pos1];
            int b1 = SDB[1][pos2];
            map<string,int> mpstd;
            int sum = 0;
            
            for (int i = 0; i < SDB.size(); i++){
                if (SDB[i][pos1] == a1 && SDB[i][pos2] == b1){
                    sum += SDB[i][5];
                }
                else{
                    string ab = to_string(a1) + " " + to_string(b1);
                    mpstd[ab] = sum;
                    sum = SDB[i][5];
                    if (SDB[i][pos1] != a1)
                        a1 =SDB[i][pos1];
                    if (SDB[i][pos2] != b1)
                        b1 =SDB[i][pos2];
                }
            }
            string ab = to_string(a1) + " " + to_string(b1);
            mpstd[ab] = sum;
            sum = 0;
            
            map<string,int> :: iterator iter2;
            for (iter2 = mpstd.begin(); iter2 != mpstd.end(); iter2 ++){
                cout <<iter2->first << " " << iter2->second << endl;
            }
            
            cout << "end for query" << endl;
            
        }
    }
    SystemInfo();

}
void CoverDB(){
  
    /* parameters */
    string cover; // read in the cover: one tuple at a time
    string query;
    vector<vector<char>> decomposition = {{'A','B','C'},{'A','B','D'},{'A','E'},{'E','F'}};
    vector<int> encode(numNode);
    vector<string> queries = Benchmark() ;
    cout << "CoverDB" << endl;
    vector<vector<int>> CDB;
    for (auto q:queries){
        int type = QueryType(q);
        
        /* Build variable order */
        VarOrder* VORoot = BuildVariableOrder(encode,q,decomposition,type);
        
        
        /* read Cover */
        ifstream C("dsi-exam18/7/C.csv");
        if (!C)
            cout << "Cannot open files" << endl;
        else{
            
            Node* root = new Node(-1);
            while (getline(C, cover, '\n')){
                vector<int> tuple = split(cover,'|');
                CDB.push_back(tuple);
                /* Build factorised database */
            }
            pos1 = q[7];
            pos2 = q[9];
           if (q[7] == 'A' || q[7] == 'C')
            sort(CDB.begin(),CDB.end(),ColumnLess);

            
            for (auto tuple:CDB)
                BuildFatorisedDB(root,tuple,encode);

            /* answers to the query */
            if (type == 1){
                cout << "Answers to query 1" << endl;
                cout << Sum(root,'1') << endl;
                
            }
            else if (type == 2){
                cout << "Answes to query 2" << endl;
                cout << SumColumn(root,encode) << endl;
            }
            else if (type == 3){
                cout << "Answers to query 3" << endl;
                map<string, int> mp = GroupByType3(root);
                map<string,int> :: iterator iter;
                for (iter = mp.begin(); iter != mp.end(); iter ++){
                    cout <<iter->first << " " << iter->second << endl;
                }
            }
            else if (type == 4){
                cout << "Answer for query 4" << endl;
                map<string, int> mp = GroupByType4(root);
                map<string,int> :: iterator iter;
                for (iter = mp.begin(); iter != mp.end(); iter ++){
                    cout <<iter->first << " " << iter->second << endl;
                }
                
            }
            delete root;
            
        }
        delete VORoot;
        
        
    }
    SystemInfo();
    
}


//////////////////////////////////////////////////////////////////////////////
//http://nadeausoftware.com/articles/2012/07/c_c_tip_how_get_process_resident_set_size_physical_memory_use

// process_mem_usage(double &, double &) - takes two doubles by reference,
// attempts to read the system-dependent data for a process' virtual memory
// size and resident set size, and return the results in KB.
//
// On failure, returns 0.0, 0.0
//
void process_mem_usage(double& vm_usage, double& resident_set)
{
    using std::ios_base;
    using std::ifstream;
    using std::string;
    
    vm_usage     = 0.0;
    resident_set = 0.0;
    
    // 'file' stat seems to give the most reliable results
    //
    ifstream stat_stream("/proc/self/stat",ios_base::in);
    
    // dummy vars for leading entries in stat that we don't care about
    //
    string pid, comm, state, ppid, pgrp, session, tty_nr;
    string tpgid, flags, minflt, cminflt, majflt, cmajflt;
    string utime, stime, cutime, cstime, priority, nice;
    string O, itrealvalue, starttime;
    
    // the two fields we want
    //
    unsigned long vsize;
    long rss;
    
    stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
    >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
    >> utime >> stime >> cutime >> cstime >> priority >> nice
    >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest
    
    stat_stream.close();
    
    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    vm_usage     = vsize / 1024.0;
    resident_set = rss * page_size_kb;
}
/*
 * Author:  David Robert Nadeau
 * Site:    http://NadeauSoftware.com/
 * License: Creative Commons Attribution 3.0 Unported License
 *          http://creativecommons.org/licenses/by/3.0/deed.en_US
 */

#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>
#include <sys/resource.h>

#if defined(__APPLE__) && defined(__MACH__)
#include <mach/mach.h>

#elif (defined(_AIX) || defined(__TOS__AIX__)) || (defined(__sun__) || defined(__sun) || defined(sun) && (defined(__SVR4) || defined(__svr4__)))
#include <fcntl.h>
#include <procfs.h>

#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
#include <stdio.h>

#endif

#else
#error "Cannot define getPeakRSS( ) or getCurrentRSS( ) for an unknown OS."
#endif





/**
 * Returns the peak (maximum so far) resident set size (physical
 * memory use) measured in bytes, or zero if the value cannot be
 * determined on this OS.
 */
size_t getPeakRSS( )
{
#if defined(_WIN32)
    /* Windows -------------------------------------------------- */
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo( GetCurrentProcess( ), &info, sizeof(info) );
    return (size_t)info.PeakWorkingSetSize;
    
#elif (defined(_AIX) || defined(__TOS__AIX__)) || (defined(__sun__) || defined(__sun) || defined(sun) && (defined(__SVR4) || defined(__svr4__)))
    /* AIX and Solaris ------------------------------------------ */
    struct psinfo psinfo;
    int fd = -1;
    if ( (fd = open( "/proc/self/psinfo", O_RDONLY )) == -1 )
        return (size_t)0L;      /* Can't open? */
    if ( read( fd, &psinfo, sizeof(psinfo) ) != sizeof(psinfo) )
    {
        close( fd );
        return (size_t)0L;      /* Can't read? */
    }
    close( fd );
    return (size_t)(psinfo.pr_rssize * 1024L);
    
#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
    /* BSD, Linux, and OSX -------------------------------------- */
    struct rusage rusage;
    getrusage( RUSAGE_SELF, &rusage );
#if defined(__APPLE__) && defined(__MACH__)
    return (size_t)rusage.ru_maxrss;
#else
    return (size_t)(rusage.ru_maxrss * 1024L);
#endif
    
#else
    /* Unknown OS ----------------------------------------------- */
    return (size_t)0L;          /* Unsupported. */
#endif
}





/**
 * Returns the current resident set size (physical memory use) measured
 * in bytes, or zero if the value cannot be determined on this OS.
 */
size_t getCurrentRSS( )
{
#if defined(_WIN32)
    /* Windows -------------------------------------------------- */
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo( GetCurrentProcess( ), &info, sizeof(info) );
    return (size_t)info.WorkingSetSize;
    
#elif defined(__APPLE__) && defined(__MACH__)
    /* OSX ------------------------------------------------------ */
    struct mach_task_basic_info info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
    if ( task_info( mach_task_self( ), MACH_TASK_BASIC_INFO,
                   (task_info_t)&info, &infoCount ) != KERN_SUCCESS )
        return (size_t)0L;      /* Can't access? */
    return (size_t)info.resident_size;
    
#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
    /* Linux ---------------------------------------------------- */
    long rss = 0L;
    FILE* fp = NULL;
    if ( (fp = fopen( "/proc/self/statm", "r" )) == NULL )
        return (size_t)0L;      /* Can't open? */
    if ( fscanf( fp, "%*s%ld", &rss ) != 1 )
    {
        fclose( fp );
        return (size_t)0L;      /* Can't read? */
    }
    fclose( fp );
    return (size_t)rss * (size_t)sysconf( _SC_PAGESIZE);
    
#else
    /* AIX, BSD, Solaris, and Unknown OS ------------------------ */
    return (size_t)0L;          /* Unsupported. */
#endif
}

void SystemInfo(){
    char buffer[BUFFERLEN];
    size_t bufferlen = BUFFERLEN;
    sysctlbyname("machdep.cpu.brand_string",&buffer,&bufferlen,NULL,0);
    printf("%s\n", buffer);
    
    
    size_t currentSize = getCurrentRSS( );
    size_t peakSize    = getPeakRSS( );
    cout <<currentSize <<" " << peakSize << endl;
}



int main(int argc, const char * argv[]) {
   clock_t t1,t2;
    float diff;
    
  /*  t1=clock();
    CoverDB();
    t2=clock();
     diff = ((float)t2-(float)t1)/CLOCKS_PER_SEC;
    cout<< diff <<endl;
  */
    t1=clock();
    StandardDB();
    t2=clock();
    diff = ((float)t2-(float)t1)/CLOCKS_PER_SEC;
    cout<< diff <<endl;
  
    return 0;
}
