#include<iostream>
#include<queue>
#include<string>
#include<sstream>
#include<vector>
#include<string.h>
using namespace std;

int destinLow[6],destinHigh[6],sourceLow[6],sourceHigh[6];

template<class TYPE>
struct Splaynode
{
    TYPE value;
    vector<int> filters;
    Splaynode<TYPE>* parent;
    Splaynode<TYPE>* left;
    Splaynode<TYPE>* right;
    Splaynode<TYPE>* dest;
    Splaynode(TYPE,std::vector<TYPE>);
};

template<class TYPE>
Splaynode<TYPE>::Splaynode(TYPE element,std::vector<TYPE> filter)
{
    value = element;
    left = NULL;
    right = NULL;
    parent = NULL;
    dest=NULL;
    filters=filter;
}


template<class TYPE>
class Splay
{
        private:
            Splaynode<TYPE>* root;
            int size;
            void levelorder_();
        public:
            Splay();
            void insert(TYPE,std::vector<TYPE>);
            void splay(Splaynode<TYPE>*);
            void rightrotate(Splaynode<TYPE>*);
            void leftrotate(Splaynode<TYPE>*);
            void levelorder();
            void search(TYPE);
            void splay(TYPE);
            void del_splay(TYPE);
            Splaynode<TYPE>* successor();
            Splaynode<TYPE>* predecessor();
            Splaynode<TYPE>* get_root();
            void connect(Splay<TYPE>);
            vector<TYPE> matching(TYPE);
};

template<class TYPE>
Splay<TYPE>::Splay()
{
    root = NULL;
    size = 0;
}

template<class TYPE>
Splaynode<TYPE>* Splay<TYPE>::get_root()
{
        return root;
}

template<class TYPE>
std::vector<TYPE> Splay<TYPE>::matching(TYPE source)
{
        std::vector<TYPE> filter;
        int flag=0;
        Splaynode<TYPE>* pre;
        std::queue<Splaynode<TYPE>*> q;
        Splaynode<TYPE>* ptr = root;
        q.push(ptr);
        while(!q.empty())
        {
            if(ptr->left)
                q.push(ptr->left);
            if(ptr->right)
                q.push(ptr->right);
            if(ptr->value==source)
            {
                flag=1;
                pre=ptr;
            }
            q.pop();
            ptr = q.front();
        }
        if(flag==0)
        {
                insert(source,filter);
                pre=predecessor();
                del_splay(source);
        }
        filter=pre->filters;
        return filter;
}

template<class TYPE>
void Splay<TYPE>::connect(Splay<TYPE> Destination)
{
        Splaynode<TYPE>* rootdest=Destination.get_root();
        std::queue<Splaynode<TYPE>*> q;
        Splaynode<TYPE>* ptr = root;
        q.push(ptr);
        while(!q.empty())
        {
            if(ptr->left)
                q.push(ptr->left);
            if(ptr->right)
                q.push(ptr->right);
            q.front()->dest=rootdest;
            q.pop();
            ptr = q.front();
        }
}


template<class TYPE>
void Splay<TYPE>::leftrotate(Splaynode<TYPE>* node)
{
//    std::cerr<<node->value;
    Splaynode<TYPE>* par = node->parent;
    Splaynode<TYPE>* newroot = node->right;
    node -> right = newroot -> left;
//    std::cerr<<"Hi";
    if(node -> right)
        node -> right -> parent = node;
    //std::cerr<<"Hello";
    newroot-> left = node;
    node->parent = newroot;
    if(par)
    {
        newroot -> parent = par;
        if(par->value > node->value)
        {
            par -> left = newroot;
        }
        else
        {
            par -> right = newroot;
        }
    }
    else
    {
        this->root = newroot;
        newroot -> parent = NULL;
    }
    //std::cerr<<"Exiting Rotation";
}

template<class TYPE>
void Splay<TYPE>::rightrotate(Splaynode<TYPE>* node)
{
    //std::cerr<<node->value;
    //std::cerr << "hi" << '\n';
    Splaynode<TYPE> *par = node->parent;
    Splaynode<TYPE>* newroot = node->left;
    node -> left = newroot-> right;
    if(newroot->right)
        newroot -> right -> parent = node;
    //std::cerr << "hi" << '\n';
    newroot-> right = node;
    node-> parent = newroot;
    if(par)
    {
        newroot -> parent = par;
        if(par->value > node->value)
        {
            par -> left = newroot;
        }
        else
        {
            par -> right = newroot;
        }
    }
    else
    {
        //std::cerr<<"hello";
        this->root = newroot;
        newroot -> parent = NULL;
        //std::cerr<<root->right->value;
    }
    //std::cerr << "bye" << '\n';
}

template<class TYPE>
void Splay<TYPE>::insert(TYPE element,std::vector<TYPE> filter)
{
    Splaynode<TYPE>* ptr = root;
    if(root == NULL)
    {
        root = new Splaynode<TYPE>(element,filter);
        return;
    }
    Splaynode<TYPE>* node = new Splaynode<TYPE>(element,filter);
    while(ptr)
    {
        if(ptr->value < element)
        {
            if(ptr->right)
                ptr = ptr->right;
            else
            {
                ptr-> right = node;
                node -> parent = ptr;
                break;
            }
        }
        else
        {
            if(ptr->left)
                ptr = ptr->left;
            else
            {
                ptr-> left = node;
                node -> parent = ptr;
                break;
            }
        }

    }
    while (node!=root)
    {
        splay(node);
    }
}

template<class TYPE>
void Splay<TYPE>::splay(Splaynode<TYPE>* node)
{
    if(node->parent==NULL)
    {
        return;
    }
    if(root->right == node )
    {
        leftrotate(root);
        return;
    }
    if(root->left == node)
    {
        rightrotate(root);
        return;
    }
    Splaynode<TYPE>* par = node->parent;
    Splaynode<TYPE>* grandpar = par->parent;
    if(grandpar->right)
    {
        if(grandpar->right->right == node)
        {
            leftrotate(grandpar);
            leftrotate(par);
        }
        else if(grandpar->right->left == node)
        {
            rightrotate(par);
            leftrotate(grandpar);
        }
    }
    if(grandpar->left)
    {
        if(grandpar->left->left == node)
        {
            rightrotate(grandpar);
            rightrotate(par);
        }
        else if(grandpar->left->right == node)
        {
            leftrotate(par);
            rightrotate(grandpar);
        }
    }
        return;
}

template<class TYPE>
void Splay<TYPE>::search(TYPE key)
{
        Splaynode<TYPE> *ptr=root,*node=NULL;
        while(ptr)
        {
            if(ptr->value < key)
            {
                if(ptr->right)
                    ptr = ptr->right;
            }
            else if(ptr->value > key)
            {
                if(ptr->left)
                    ptr = ptr->left;
            }
            else
            {
                    node=ptr;
                    break;
            }
        }
        if(node==NULL)
                cout<<"Element Not Present";
        else
        {
                while (node!=root)
                {
                    splay(node);
                }
        }
}

template<class TYPE>
Splaynode<TYPE>* Splay<TYPE> :: successor()
{
        Splaynode<TYPE> *ptr=root;
        ptr=ptr->right;
        while(ptr->left)
        {
                ptr=ptr->left;
        }
        return ptr;
}

template<class TYPE>
Splaynode<TYPE>* Splay<TYPE> :: predecessor()
{
        Splaynode<TYPE> *ptr=root;
        ptr=ptr->left;
        while(ptr->right)
        {
                ptr=ptr->right;
        }
        return ptr;
}

template<class TYPE>
void Splay<TYPE>::del_splay(TYPE key)
{
        search(key);
        Splaynode<TYPE> *ptr=root,*node=NULL;
        if(ptr->left==NULL&&ptr->right==NULL)
                delete ptr;
        else if(ptr->right)
                node=successor();
        else
                node=predecessor();
        if(node&&node->parent->left==node&&node->parent!=root)
                node->parent->left=node->right;
        else if(node&&node->parent->right==node&&node->parent!=root)
                node->parent->right=node->left;
        else if(node&&node->parent->left==node&&node->parent==root)
                node->parent->left=node->left;
        else if(node&&node->parent->right==node&&node->parent==root)
                node->parent->right=node->right;
        if(node)
        {
            node->parent=NULL;
            node->right=root->right;
            node->left=root->left;
            if(node->left)
                node->left->parent=node;
            if(node->right)
            node->right->parent=node;
            root=node;
        }
}

template<class TYPE>
void Splay<TYPE>::levelorder()
{
    levelorder_();
}

template<class TYPE>
void Splay<TYPE>::levelorder_()
{
    std::queue<Splaynode<TYPE>*> q;
    std::vector<int> :: iterator i;
    std::cerr<<"levelorder";
    Splaynode<TYPE>* ptr = root;
    q.push(ptr);
    while(!q.empty())
    {
        if(ptr->left)
            q.push(ptr->left);
        if(ptr->right)
            q.push(ptr->right);
        std::cerr<<q.front()->value<<" ";
        for(i=q.front()->filters.begin();i!=q.front()->filters.end();i++)
        {
                cout<<*i<<" ";
        }
        cout<<"\n";
        q.pop();
        ptr = q.front();
    }
}

int lowBit(int value,int strLen)
{
    while(strLen<6)
    {
        value=value*10;
        strLen++;
    }
    return value;
}

int highBit(int value,int strLen)
{
    while(strLen<6)
    {
        value=value*10+1;
        strLen++;
    }
    return value;
}

int bitToInt(unsigned int binary)
{
    int decimal=0,i=1,rem;
    while(binary!=0)
    {
        rem=binary%10;
        decimal=decimal+rem*i;
        i=i*2;
        binary=binary/10;
    }
    return decimal;
}

std::vector<int> generateSourceIndex()
{
    cout<<"Source Search Table:\n";
    string sourcePrefix[]={"01","1","10","01","00","*"};
    int strLength,value,lowValue,highValue;
    string sampValue;
    int i;
    std::vector<int> sourceIndex;
    for(i=0;i<6;i++)
    {
        if(sourcePrefix[i]=="*")
        {
            sourceLow[i]=0;
            sourceHigh[i]=63;
        }
        else
        {
            sampValue=sourcePrefix[i];
            strLength=sampValue.length();
            //cout << strLength << "\n";
            stringstream str(sampValue);
            str >> value;
            lowValue=lowBit(value,strLength);
            sourceLow[i]=bitToInt(lowValue);
            highValue=highBit(value,strLength);
            sourceHigh[i]=bitToInt(highValue);
        }
        std::cerr<<"F"<<i+1<<'\t' << sourceLow[i] << "  -  " << sourceHigh[i] << "\n";
    }
    int flag;
    std::vector<int>::iterator it;
    for(i = 0; i < 6; i++)
    {
            flag=0;
            for(it=sourceIndex.begin();it!=sourceIndex.end();it++)
            {
                    if(*it==sourceLow[i])
                        flag=1;
            }
            if(flag==0)
                sourceIndex.push_back(sourceLow[i]);
            flag=0;
            for(it=sourceIndex.begin();it!=sourceIndex.end();it++)
            {
                if(*it==sourceHigh[i])
                        flag=1;
            }
            if(flag==0)
                sourceIndex.push_back(sourceHigh[i]);
    }
    return sourceIndex;
}

std::vector<int> generateDestinationIndex()
{
    cout<<"\n\nDestination Search Table:\n";
    string destinPrefix[]={"01","01","0","00","1","1"};
    int i;
    int strLength,value,lowValue,highValue;
    std::vector<int> destinIndex;
    string sampValue;
    std::cerr << "\n";
    for(int i=0;i<6;i++)
    {
        if(destinPrefix[i]=="*")
        {
            destinLow[i]=0;
            destinHigh[i]=63;
        }
        else
        {
            sampValue=destinPrefix[i];
            strLength=sampValue.length();
            stringstream str(sampValue);
            str >> value;
            lowValue=lowBit(value,strLength);
            destinLow[i]=bitToInt(lowValue);
            highValue=highBit(value,strLength);
            destinHigh[i]=bitToInt(highValue);
        }
        std::cerr <<"F" <<i+1<<'\t' << destinLow[i] << "  -  " << destinHigh[i] << "\n";
    }
    int flag;
    std::vector<int>::iterator it;
    for(i = 0; i < 6; i++)
    {
            flag=0;
            for(it=destinIndex.begin();it!=destinIndex.end();it++)
            {
                    if(*it==destinLow[i])
                        flag=1;
            }
            if(flag==0)
                destinIndex.push_back(destinLow[i]);
            flag=0;
            for(it=destinIndex.begin();it!=destinIndex.end();it++)
            {
                if(*it==destinHigh[i])
                        flag=1;
            }
            if(flag==0)
                destinIndex.push_back(destinHigh[i]);
    }
    return destinIndex;
}

std::vector<int> common_ele(std::vector<int> v1,std::vector<int> v2)
{
        std::vector<int> common;
        std::vector<int>::iterator it1,it2;
        for(it1=v1.begin();it1!=v1.end();it1++)
        {
                for(it2=v2.begin();it2!=v2.end();it2++)
                {
                        if(*it1==*it2)
                                common.push_back(*it1);
                }
        }
        return common;
}

int main(void)
{
    int source,destination,binary;
    Splay<int> Source,Destination;
    std::vector<int> Sou,Dest,filter,Source_filters,Dest_filters;
    Sou=generateSourceIndex();
    Dest=generateDestinationIndex();
    int i,j;
    for(i=0;i<Sou.size();i++)
    {
        for(j=0;j<6;j++)
        {
                if(Sou[i]>=sourceLow[j]&&Sou[i]<sourceHigh[j])
                {
                        filter.push_back(j+1);
                }
        }
        Source.insert(Sou[i],filter);
        filter.clear();
    }
    std::cerr<<endl;
    for(i=0;i<Dest.size();i++)
    {
        for(j=0;j<6;j++)
        {
                if(Dest[i]>=destinLow[j]&&Dest[i]<destinHigh[j])
                {
                        filter.push_back(j+1);
                }
        }
        Destination.insert(Dest[i],filter);
        filter.clear();
    }
    std::cerr<<"\n";
    // Source.levelorder();
    // std::cerr<<"\n";
    // Destination.levelorder();
    Source.connect(Destination);
    entry_source:
    cout<<"Enter The Source Search Packet in Binary";
    cin>>binary;
    source=bitToInt(binary);
    if(source>63)
    {
        cout<<"Invalid Packet Size\n";
        goto entry_source;
    }
    entry_dest:
    cout<<"Enter The Destination Search Packet in Binary";
    cin>>binary;
    destination=bitToInt(binary);
    if(destination>63)
    {
        cout<<"Invalid Packet Size\n";
        goto entry_dest;
    }
    Source_filters=Source.matching(source);
    Dest_filters=Destination.matching(destination);
    std::vector<int> ::iterator it;
    cout<<"Source Filters: ";
    for(it=Source_filters.begin();it!=Source_filters.end();it++)
    {
            cout<<"F"<<*it<<"  ";
    }
    cout<<"\nDestination Filters: ";
    for(it=Dest_filters.begin();it!=Dest_filters.end();it++)
    {
            cout<<"F"<<*it<<"  ";
    }
    int flag=1;
    cout<<"\nBest Matching Filters: ";
    filter=common_ele(Dest_filters,Source_filters);
    for(it=filter.begin();it!=filter.end();it++)
    {
            cout<<"F"<<*it<<" ";
    }
    if(filter.size()==0)
        cout<<"No Matching Filters Found ";
}
