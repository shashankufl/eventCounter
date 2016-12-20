#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

struct node
{
       int id;
       int count;
       char color;
       node *parent;
       node *left;
       node *right;
};

class RBtree
{
      node *root;
      node *q;
      vector<int> ids;
      vector<int> counts;
   public :
      RBtree()
      {
              q=NULL;
              root=NULL;
      }
      void createBST(char infile[]);
      node* arrayBST(int start,int end);
      void initializeRBtree();
      int insert(int z,int m); // Inserting a node into 'r''b' tree
      int getIdsInRange(int id1,int id2);
      int update(int z,int m);
      void insertfix(node *);
      void leftrotate(node *);
      void rightrotate(node *);
      int del(int x,int cnt); // Deleting a particular node from 'r''b' tree
      void delfix(node *);
      void transplant(node* u,node* v);
      node* successor(node *);
      void disp();
      node* getNextNode(int );
      node* getPrevNode(int );
      void display( node *);
      node* search(int );
};
int RBtree::insert(int z,int m)
{
     int i;
     {
     node *p,*q;
     node *t=new node;
     node* temp=search(z);
     if(temp!=NULL)
     {
        if(update(z,m))
        {
           return(temp->count);
        }
     }
     else
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
}

void RBtree::insertfix(node *t)
{
    node *parent=t->parent,*grand_parent;
    while((t!=root)&&(t->color=='r')&&(t->parent->color=='r'))
    {
        parent=t->parent;
        grand_parent=t->parent->parent;
        /* When parent of t is grand_parent's left child*/
        if(parent == grand_parent->left)
        {
            node* uncle=grand_parent->right;
            if((uncle!=NULL)&&(uncle->color=='r'))
            {
                grand_parent->color='r';
                parent->color='b';
                uncle->color='b';
                t=grand_parent;
            }
            else
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
        else
        {
            node* uncle=grand_parent->left;
            if((uncle!=NULL)&&(uncle->color=='r'))
            {
                grand_parent->color='r';
                parent->color='b';
                uncle->color='b';
                t=grand_parent;
            }
            else
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
     if(root==NULL)
     {
           return 0;
     }
     node* tempNode=search(px);
     if(tempNode==NULL)
     {
         return 0;
     }
     //actual RBtree node delete code
     if(tempNode->count <= cnt)
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
     else
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
        if(x==x->parent->left)
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
        else
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
     if(p->right==NULL)
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
     if(p->left==NULL)
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
     if(p->right!=NULL)
     {
         y=p->right;
         while(y->left!=NULL)
              y=y->left;
     }
     else if(p->parent!=NULL)
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

void RBtree::disp()
{
     display(root);
}
void RBtree::display(node *p)
{
     if(root==NULL)
     {
          cout<<"\nEmpty Tree.";
          return ;
     }
     if(p!=NULL)
     {
                cout<<"\n\t NODE: ";
                cout<<"\n id: "<<p->id;
                cout<<"\n count: "<<p->count;
                cout<<"\n Color: ";
                if(p->color == 'r')
                    cout<<"'r'";
                else
                    cout<<"'b'";
    if(p->left)
    {
                 cout<<"\n\nLeft:\n";
     display(p->left);
    }
    /*else
     cout<<"\nNo Left Child.\n";*/
    if(p->right)
    {
     cout<<"\n\nRight:\n";
                 display(p->right);
    }
    /*else
     cout<<"\nNo Right Child.\n"*/
     }
}
node* RBtree::search(int x)
{
     if(root==NULL)
     {
           return  NULL;
     }
     node *p=root;
     int found=0;
     while(p!=NULL && found==0)
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
            if(p->id==z)
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
    if(temp==NULL)
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
    if(temp==NULL)
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
    if(t==NULL)
        t=getNextNode(id1);
    if(t==NULL)
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
    ifstream myfile (infile);
    myfile>>total_inputs;
    for(int i=0;i<total_inputs;i++)
    {
        myfile>>id>>count;
        ids.push_back(id);
        counts.push_back(count);
        //insert(id,count);
    }
    myfile.close();
    root=arrayBST(0,ids.size()-1);
    root->parent = NULL;
}

node* RBtree::arrayBST(int start,int end)
{
    if(start>end)
        return NULL;
    int mid=(start+end)/2;
    node* root=new node;
    root->id=ids[mid];
    root->color='b';
    root->count=counts[mid];
    node *x=new node,*y=new node;
    x = arrayBST(start,mid-1);
    root->left = x;
    if(x!=NULL)
        x->parent = root;
    y = arrayBST(mid+1,end);
    root->right = y;
    if(y!=NULL)
        y->parent = root;
    if((end-start+1)%2==0)
    {
        node *temp = new node;
        temp=root;
        while(temp->right!=NULL)
        {
            temp=temp->right;
        }
        temp->color='r';
    }
    return root;
}

void RBtree::initializeRBtree()
{
    cout<<root->id<<endl;
}


int main(int argc, char *argv[])
{
    int ch,y=0;
    map<string,int> m;
    m["increase"]=1;
    m["reduce"]=2;
    m["count"]=3;
    m["inrange"]=4;
    m["next"]=5;
    m["previous"]=6;
    m["display"]=7;
    m["quit"]=8;
    RBtree obj;
    obj.createBST(argv[1]);
    int id,cnt,id1,id2;
    string str;
    node *temp,*t;
    do
    {
                cin>>str;
                switch(m[str])
                {
                          case 1 : cin>>id>>cnt;
                                   cnt=obj.insert(id,cnt);
                                   cout<<cnt<<endl;
                                    break;
                          case 2 : cin>>id>>cnt;
                                   cnt=obj.del(id,cnt);
                                   cout<<cnt<<endl;
                                    break;
                          case 3 :  cin>>id;
                                    temp=obj.search(id);
                                       if(temp!=NULL)
                                       {
                                           cout<<temp->count<<endl;
                                       }
                                       else
                                       {
                                           cout<<"0"<<endl;
                                       }
                                       break;
                          case 7 :  obj.disp();
                                     break;
                          case 5 :  cin>>id;
                                   t=obj.getNextNode(id);
                                   if(t!=NULL)
                                   {
                                       cout<<t->id<<" "<<t->count<<endl;
                                   }
                                   else
                                    cout<<"0 0"<<endl;
                                   break;
                          case 6 : cin>>id;
                                   t=obj.getPrevNode(id);
                                   if(t!=NULL)
                                   {
                                       cout<<t->id<<" "<<t->count<<endl;
                                   }
                                   else
                                    cout<<"0 0"<<endl;
                                   break;
                          case 4 : cin>>id1>>id2;
                                   cnt=obj.getIdsInRange(id1,id2);
                                   cout<<cnt<<endl;
                                   break;
                          case 8 : y=1;
                                   break;

                          default : cout<<"\nInvalid Command";
                }
    }while(y!=1);
    return 1;
}
