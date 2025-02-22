#include <iostream>
#include <vector>
#include <deque>

using namespace std;

struct Node {
    string path;
    string fullPath;
    vector<Node*> children;
    vector<char> indexes;   //存储孩子们的路径首字母,不可重复
    bool hasFunction;       //用hasfunction代替本应存储的函数指针
    //int priortity;        //优先级，提升路由性能,暂未实现  思路：indexes的顺序根据priority变化,高频次的放在前面
};

int findCommonPrefix(string str1, string str2)
{
    int i=0;
    int max = std::min(str1.length(), str2.length());
    while(i < max && str1[i] == str2[i])
        i++;
    return i;
}

void split(Node* root, string rootFullPath, int common)
{
    Node* newNode = new Node();
    newNode->path = rootFullPath.substr(common,rootFullPath.length());
    newNode->fullPath = rootFullPath;
    newNode->hasFunction = true;

    root->children.push_back(newNode);
    root->path =  root->path.substr(0,root->path.length()-(root->fullPath.length()-common));
    root->fullPath = rootFullPath.substr(0,common);
    root->hasFunction = false;
    root->indexes.push_back(rootFullPath[common]);
}

void createNewNodeAfterRoot(Node* root, string newPath, int common)
{
    Node* newNode = new Node();
    newNode->path = newPath.substr(common,newPath.length());
    newNode->fullPath = newPath;
    newNode->hasFunction = true;

    root->children.push_back(newNode);
    root->indexes.push_back(newPath[common]);
}

Node* findNextByIndex(Node* root, char index)
{
    int i=0 ;
    for(; i < root->indexes.size(); i++)
    {
        if(root->indexes[i] == index)
            break;
    }
    return root->children[i];
}

bool containsIndex(Node* root, char index)
{
    for(auto it = root->indexes.begin(); it != root->indexes.end(); ++it)
    {
        if(*it == index)
            return true;
    }
    return false;
}

void insertNewNode(string newPath, Node* root)
{
    string rootFullPath = root->fullPath;

    int common = findCommonPrefix(newPath, rootFullPath);

    if(common < rootFullPath.length())
    {
        //split origin root to two node
        split(root,rootFullPath,common);
    }

    if(common < newPath.length())
    {
        //create new node after root
        if(!containsIndex(root,newPath[common])){
            createNewNodeAfterRoot(root, newPath, common);
        }else
        {
            Node* next = findNextByIndex(root,newPath[common]);
            insertNewNode(newPath,next);
        }
        return;
    }

    if(root->hasFunction){
        printf("error same path  has functions\n");
    }
    
}

void printNode(Node* root)
{
    std::deque<Node*> q;
    q.push_back(root);
    while(q.size() != 0)
    {
        std::deque<Node*> q1;
        while(q.size() != 0)
        {
            Node* n = q.front();
            for(int i = 0; i < n->children.size(); i++)
            {
                q1.push_back(n->children[i]);
            }
            q.pop_front();
            printf("path:%s fullPath:%s  indexes: ", n->path.c_str(), n->fullPath.c_str());
            for(auto it = n->indexes.begin(); it != n->indexes.end(); ++it)
            {
                printf(" %c ", *it);
            }
            printf("\n");
        }
        printf("=======\n");
        for (int i = 0; i < q1.size(); i++)
        {
            q.push_back(q1[i]);
        }
    }
}
int main() {

    Node* root = new Node();
    root->path = "\\";
    root->fullPath = "\\";
    root->hasFunction = true;
    insertNewNode("\\a",root);
    insertNewNode("\\b",root);
    insertNewNode("\\a",root);
    insertNewNode("\\b",root);
    insertNewNode("\\ab",root);
    insertNewNode("\\abc",root);
    insertNewNode("\\abb",root);
    insertNewNode("\\abd",root);
    insertNewNode("\\bab",root);
    insertNewNode("\\bac",root);
    insertNewNode("\\bdb",root);
    printNode(root);

/*
    error same path  has functions
    error same path  has functions
    path:\ fullPath:\  indexes:  a  b 
    =======
    path:a fullPath:\a  indexes:  b 
    path:b fullPath:\b  indexes:  a  d 
    =======
    path:b fullPath:\ab  indexes:  c  b  d 
    path:a fullPath:\ba  indexes:  b  c 
    path:db fullPath:\bdb  indexes: 
    =======
    path:c fullPath:\abc  indexes: 
    path:b fullPath:\abb  indexes: 
    path:d fullPath:\abd  indexes: 
    path:b fullPath:\bab  indexes: 
    path:c fullPath:\bac  indexes: 
    =======
*/
}