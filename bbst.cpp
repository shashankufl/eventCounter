/* Binary Counter using Red-Black Tree
   Created by : Shashank Sharma */
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

using namespace std;

struct node // Structure for defining a node in the RedBlack tree
{
       int id;
       int count;
       char color;
       node *parent;
       node *left;
       node *right;
};

class RBtree // Class for managing the RedBlack tree and it's functions
{
      node *root;
      int height;
      vector<int> ids;
      vector<int> counts;
   public :
      RBtree() // Constructor initializes a null tree when RBtree class object is created
      {
              root=NULL;
              height=0;
      }
      void createBST(char infile[]); // Create input sorted arrays using the dynamically provided input file & initialize the RB tree
      node* createRedBlackTree(int start,int end,int h); // Create BST(Binary Search Tree)
      int insert(int z,int m); // insert corresponds to the increase(theID) function
      void insertfix(node *); // Fix Black Height in the RedBlack tree after an event is inserted
      int update(int z,int m); // Update the count value for an existing event
      void leftrotate(node *); // Left rotate at a node in the RedBlack tree
      void rightrotate(node *); // Right rotate at a node in the RedBlack tree
      int del(int x,int cnt); // Deleting a node from RedBlack tree
      void delfix(node *); // Fix Black Height in the RedBlack tree after an event is deleted
      void transplant(node* u,node* v); // Function to transplant a node to the 2 degree node that's getting deleted
      node* successor(node *); // Determine successor for a node
      node* getNextNode(int ); // getNextNode corresponds to the next(theID) function
      node* getPrevNode(int ); // getPrevNode corresponds to the prev(theID) function
      node* search(int ); // search function to search an event ID in the RedBlack tree
      int getIdsInRange(int id1,int id2); // getIdsInRange corresponds to the inrange function
};

int RBtree::insert(int z,int m)
{
     node *p,*q;
     node *t=new node;
     node* temp=search(z);
     if(temp!=NULL) // Update if event ID already exists
     {
        if(update(z,m))
        {
           return(temp->count);
        }
     }
     else if(m<=0) // Can't insert into tree if event ID doesn't exist & count is <=0
     {
         return 0;
     }
     else // Insert the event ID & count if event ID doesn't exist in the RedBlack tree and count >0
     {
     t->id=z;
     t->count=m;
     t->left=NULL;
     t->right=NULL;
     t->color='r';
     p=root;
     q=NULL;
     if(p==NULL)
     {
           root=t;
           t->parent=NULL;
     }
     else
     {
         while(p!=NULL)
         {
              q=p;
              if(p->id<t->id)
                  p=p->right;
              else
                  p=p->left;
         }
         t->parent=q;
         if(q->id<t->id)
              q->right=t;
         else
              q->left=t;
     }
     insertfix(t);
     return(t->count);
     }
}

void RBtree::insertfix(node *t)
{
    node *parent=t->parent,*grand_parent;
    while((t!=root)&&(t->color=='r') && (t->parent->color=='r'))
    {
        parent=t->parent;
        grand_parent=t->parent->parent;
        if(parent == grand_parent->left) // When parent of t is grand_parent's left child
        {
            node* uncle=grand_parent->right;
            if((uncle!=NULL)&&(uncle->color=='r')) // The case where uncle is red
            {
                grand_parent->color='r';
                parent->color='b';
                uncle->color='b';
                t=grand_parent;
            }
            else // The case where uncle is black
            {
                if(t==parent->right)
                {
                    leftrotate(parent);
                    t=parent;
                    parent=t->parent;
                }
                rightrotate(grand_parent);
                swap(parent->color,grand_parent->color);
                t=parent;
            }
        }
        else // When parent of t is grand_parent's right child
        {
            node* uncle=grand_parent->left;
            if((uncle!=NULL)&&(uncle->color=='r')) // The case where uncle is red
            {
                grand_parent->color='r';
                parent->color='b';
                uncle->color='b';
                t=grand_parent;
            }
            else // The case where uncle is black
            {
                if(t==parent->left)
                {
                    rightrotate(parent);
                    t=parent;
                    parent=t->parent;
                }
                leftrotate(grand_parent);
                swap(parent->color,grand_parent->color);
                t=parent;
            }
        }
    }
    root->color='b';
}

void RBtree::transplant(node* u,node* v)
{
    if(u->parent==NULL)
    {
        root=v;
    }
    else if(u==u->parent->left)
    {
        u->parent->left=v;
    }
    else
    {
        u->parent->right=v;
    }
    if(v!=NULL)
    {
        v->parent=u->parent;
    }
}

int RBtree::del(int px,int cnt)
{
     if(root==NULL) // Can't execute reduce(theID) if the root is null
     {
           return 0;
     }
     node* tempNode=search(px);
     if(tempNode==NULL) // Can't execute reduce(theID) if the event doesn't exist
     {
         return 0;
     }
     if(tempNode->count <= cnt) // Delete node if the event exists & new count is less than or equal to current event count
     {
         node *y=tempNode,*x;
         char y_originalColor=y->color;
         if(tempNode->left==NULL)
         {
             x=tempNode->right;
             transplant(tempNode,tempNode->right);
         }
         else if(tempNode->right==NULL)
         {
             x=tempNode->left;
             transplant(tempNode,tempNode->left);
         }
         else
         {
             y=tempNode->right;
             while(y->left!=NULL)
             {
                 y=y->left;
             }
             y_originalColor = y->color;
             x=y->right;
             if((x!=NULL))
             {
                 if(y->parent==tempNode)
                 {
                     x->parent=y;
                 }
                 else
                 {
                     transplant(y,y->right);
                     y->right=tempNode->right;
                     y->right->parent=y;
                 }
             }
             transplant(tempNode,y);
             y->left=tempNode->left;
             y->left->parent=y;
             y->color=tempNode->color;
         }
         if((y_originalColor == 'b')&&(x!=NULL))
         {
             delfix(x);
         }
         return(0);
     }
     else // Update node if the event exists & new count is more than current event count
     {
        if(update(tempNode->id,(-1)*cnt))
        {
            return(tempNode->count);
        }
     }
}

void RBtree::delfix(node *x)
{
    node *w;
    while((x!=root)&&(x->color=='b'))
    {
        if(x==x->parent->left) // Considering the left child
        {
            w=x->parent->right;
            if(w->color=='r')
            {
                w->color='b';
                x->parent->color='r';
                leftrotate(x->parent);
                w=x->parent->right;
            }
            if((w->left->color=='b')&&(w->right->color=='b'))
            {
                w->color='r';
                x=x->parent;
            }
            else if(w->right->color=='b')
            {
                w->left->color='b';
                w->color='r';
                rightrotate(w);
                w=x->parent->right;
            }
            w->color=x->parent->color;
            x->parent->color='b';
            w->right->color='b';
            leftrotate(x->parent);
            x=root;
        }
        else // Considering the right child
        {
            w=x->parent->left;
            if(w->color=='r')
            {
                w->color='b';
                x->parent->color='r';
                rightrotate(x->parent);
                w=x->parent->left;
            }
            if((w->right->color=='b')&&(w->left->color=='b'))
            {
                w->color='r';
                x=x->parent;
            }
            else if(w->left->color=='b')
            {
                w->right->color='b';
                w->color='r';
                leftrotate(w);
                w=x->parent->left;
            }
            w->color=x->parent->color;
            x->parent->color='b';
            w->left->color='b';
            rightrotate(x->parent);
            x=root;
        }
    }
    x->color='b';
}

void RBtree::leftrotate(node *p)
{
     if(p->right==NULL) // Right Rotation not required & not possible if there's no right child
           return ;
     else
     {
           node *y=p->right;
           if(y->left!=NULL)
           {
                  p->right=y->left;
                  y->left->parent=p;
           }
           else
                  p->right=NULL;
           if(p->parent!=NULL)
                y->parent=p->parent;
           if(p->parent==NULL)
           {
               root=y;
               y->parent=NULL;
           }
           else
           {
               if(p==p->parent->left)
                       p->parent->left=y;
               else
                       p->parent->right=y;
           }
           y->left=p;
           p->parent=y;
     }
}
void RBtree::rightrotate(node *p)
{
     if(p->left==NULL) // Left Rotation not required & not possible if there's no left child
          return ;
     else
     {
         node *y=p->left;
         if(y->right!=NULL)
         {
                  p->left=y->right;
                  y->right->parent=p;
         }
         else
                 p->left=NULL;
         if(p->parent!=NULL)
                 y->parent=p->parent;
         if(p->parent==NULL)
         {
             root=y;
             y->parent=NULL;
         }
         else
         {
             if(p==p->parent->left)
                   p->parent->left=y;
             else
                   p->parent->right=y;
         }
         y->right=p;
         p->parent=y;
     }
}

node* RBtree::successor(node *p)
{
     node *y=NULL;
     if(p->right!=NULL) // Determine successor from right node if present
     {
         y=p->right;
         while(y->left!=NULL)
              y=y->left;
     }
     else if(p->parent!=NULL) // Determine successor if right node is not present
     {
         if(p==p->parent->left)
         {
             y=p->parent;
         }
         else
         {
             node *t=p;
             while(t->parent!=NULL)
             {
                 t=t->parent;
                 if(t->parent!=NULL)
                 {
                 if(t==t->parent->left)
                 {
                     y=t->parent;
                     break;
                 }
                 }
             }
         }
     }
     return y;
}

node* RBtree::search(int x)
{
     if(root==NULL) // Event not present if root is null
     {
           return  NULL;
     }
     node *p=root;
     int found=0;
     while(p!=NULL && found==0) // Searching down the tree starting from the root
     {
            if(p->id==x)
            {
                found=1;
                break;
            }
            if(found==0)
            {
                 if(p->id<x)
                      p=p->right;
                 else
                      p=p->left;
            }
     }
     if(found==0)
          p=NULL;
     return p;
}

int RBtree::update(int z,int m)
{
     node *p=root;
     int flag=0;
     while((p!=NULL) && (flag==0))
     {
            if(p->id==z) // If event is found, update it's count value
            {
              p->count = p->count + m;
              flag=1;
            }
            if(flag==0)
            {
                 if(p->id<z)
                      p=p->right;
                 else
                      p=p->left;
            }
     }
     return flag;
}

node* RBtree::getNextNode(int id)
{
    node *temp =root,*t;
    if(temp==NULL) // Event not present if root is null
    {
        return NULL;
    }
   t=NULL;
   while(temp!=NULL)
   {
       if((temp->id>id)&&(t==NULL))
       {
           t=temp;
       }
       else if((temp->id>id)&&(t!=NULL)&&(t->id>temp->id))
       {
           t=temp;
       }
       if(temp->id<=id)
       {
           temp=temp->right;
       }
       else
       {
           temp=temp->left;
       }
   }
   return t;
}

node* RBtree::getPrevNode(int id)
{
    node *temp =root,*t;
    if(temp==NULL) // Event not present if root is null
    {
        return NULL;
    }
   t=NULL;
   while(temp!=NULL)
   {
       if((temp->id<id)&&(t==NULL))
       {
           t=temp;
       }
       else if((temp->id<id)&&(t!=NULL)&&(t->id<temp->id))
       {
           t=temp;
       }
       if(temp->id<id)
       {
           temp=temp->right;
       }
       else
       {
           temp=temp->left;
       }
   }
   return t;
}

int RBtree::getIdsInRange(int id1,int id2)
{
    int sum=0;
    node *t=search(id1);
    if(id1>id2) // Condition that id1 <= id2 isn't satisfied , then return 0 as output
    {
        return 0;
    }
    if(t==NULL) // Start with id1 if it's present
        t=getNextNode(id1);
    if(t==NULL) // Nothing to display if id1 itself isn't found
        return 0;
    node* current=t;
    while(current!=NULL)
    {
        if(current->id>id2)
            break;
        sum=sum+current->count;
        current=successor(current);
    }
    return(sum);
}

void RBtree::createBST(char infile[])
{
    int total_inputs,id,count;
    ifstream myfile (infile); // Read input from file whose name is specified at runtime
    myfile>>total_inputs;
    for(int i=0;i<total_inputs;i++) // For storing all events & their counts in two different arrays
    {
        myfile>>id>>count;
        ids.push_back(id);
        counts.push_back(count);
        //insert(id,count);
    }
    myfile.close();
    height = log2(ids.size());
    root=createRedBlackTree(0,ids.size()-1,0); // Create the Red Black tree using the two arrays above with initial height =0
    root->parent = NULL;
}

node* RBtree::createRedBlackTree(int start,int end,int h)
{
    char setColor;
    if(start>end)
        return NULL;
    if(h==height)
        setColor = 'r';
    else
        setColor = 'b';
    int mid=(start+end)/2;
    node* root=new node;
    root->id=ids[mid];
    root->color=setColor;
    root->count=counts[mid];
    node *x=new node,*y=new node;
    x = createRedBlackTree(start,mid-1,h+1); // Recursively create the left subtree
    root->left = x;
    if(x!=NULL)
        x->parent = root;
    y = createRedBlackTree(mid+1,end,h+1); // Recursively create the right subtree
    root->right = y;
    if(y!=NULL)
        y->parent = root;
    return root;
}


int main(int argc, char *argv[]) // Obtain arguments specified during runtime
{
    int flag=0;
    RBtree obj; // Object created for the RBtree class for the RedBlack tree
    obj.createBST(argv[1]); // Call function for RBtree initialization with argv[1] pointing to the input file name
    int id,cnt,id1,id2; // Variables for taking inputs for a command
    string str;
    node *temp,*t;
    do
    {
                cin>>str; // Command Name
                          if(str=="increase")
                          {
                              cin>>id>>cnt;
                              cnt=obj.insert(id,cnt);
                              cout<<cnt<<endl;
                          }
                          else if(str=="reduce")
                          {
                              cin>>id>>cnt;
                              cnt=obj.del(id,cnt);
                              cout<<cnt<<endl;
                          }
                          else if(str=="count")
                          {
                              cin>>id;
                              temp=obj.search(id);
                               if(temp!=NULL)
                               {
                                   cout<<temp->count<<endl;
                               }
                               else
                               {
                                   cout<<"0"<<endl;
                               }
                          }
                          else if(str=="next")
                          {
                              cin>>id;
                               t=obj.getNextNode(id);
                               if(t!=NULL)
                               {
                                   cout<<t->id<<" "<<t->count<<endl;
                               }
                               else
                                cout<<"0 0"<<endl;
                          }
                          else if(str=="previous")
                          {
                              cin>>id;
                               t=obj.getPrevNode(id);
                               if(t!=NULL)
                               {
                                   cout<<t->id<<" "<<t->count<<endl;
                               }
                               else
                                cout<<"0 0"<<endl;
                          }
                          else if(str=="inrange")
                          {
                              cin>>id1>>id2;
                              cnt=obj.getIdsInRange(id1,id2);
                              cout<<cnt<<endl;
                          }
                          else if(str=="quit")
                          {
                              flag=1;
                          }

    }while(flag!=1);
    return 1;
}
