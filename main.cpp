#include <iostream>
#include <cstdlib>
#include <vector>
#include <windows.h>
#include <queue>
#include <assert.h>
using namespace std;
enum NODE_CLR{
    RED,BLK
};

template <class T>
class RB_tree{
public:
    RB_tree(){
        root = nullptr;
    }
    ~RB_tree(){
        clear_tr();
    }
    bool insert_val(T val);
    bool search_val(T val);
    bool delete_val(T val);
    void clear_tr();
    void print_t();

    int tsize();

protected:
    struct RB_node;

    RB_node* root;
    //RB_node* pre;
    //RB_node* nil;
    int _size;

    RB_node* left_rotate(RB_node* rbnode);
    RB_node* right_rotate(RB_node* rbnode);

    RB_node* findv(T v,RB_node*& pre);
    void delete_t(RB_node *r);

    inline RB_node* bro(RB_node* rbnode){
        return rbnode == rbnode->p->lc ? rbnode->p->rc : rbnode->p->lc;
    }
    bool init(T v);
    void solve_lost_black(RB_node* rbnode);
    //void solve_double_red(RB_node* rbnode);
};

template <class T>
struct RB_tree<T>::RB_node{
    T val;
    RB_node *p;
    RB_node *lc;
    RB_node *rc;
    NODE_CLR clr;

    RB_node(T value,RB_node* par = nullptr,RB_node* lchild = nullptr ,RB_node* rchild = nullptr)
        :val(value),p(par),lc(lchild),rc(rchild),clr(RED){}
    RB_node* succ (){ // only in the r-child, use for delete
        RB_node* ptn = rc;
        while(ptn->lc){
            ptn = ptn->lc;
        }
        return ptn;
    }

    RB_node* right(){ // mid-order next
        RB_node* ptn = this;
        if(!rc){
            while(ptn->p && ptn->p->rc == ptn){
                ptn = ptn->p;
            }
            ptn = ptn->p;
        }else{
            ptn = rc;
            while(ptn->lc){
                ptn = ptn->lc;
            }
        }
        return ptn;
    }

    RB_node* left(){
        RB_node* ptn = this;
        if(!lc){
            while(ptn->p && ptn->p->lc == ptn){
                ptn = ptn->p;
            }
            ptn = ptn->p;
        }else{
            ptn = lc;
            while(ptn->rc){
                ptn = ptn->rc;
            }
        }
        return ptn;
    }
};

template <class T>
typename RB_tree<T>::RB_node* RB_tree<T>::left_rotate(RB_node* rbnode){
    // update rbnode->p
    RB_node* y = rbnode->rc;
    if(rbnode == root) root = y;
    rbnode->rc = y->lc;
    if(rbnode->rc) rbnode->rc->p = rbnode;
    y->p = rbnode->p;
    if(y->p){
        if(y->p->lc == rbnode){
            y->p->lc = y;
        }else{
            y->p->rc = y;
        }
    }
    rbnode->p = y;
    y->lc = rbnode;
    return y;

    //return rbnode->p;
}

template <class T>
typename RB_tree<T>::RB_node* RB_tree<T>::right_rotate(RB_node* rbnode){
    RB_node* y = rbnode->lc;
    if(rbnode == root) root = y;
    rbnode->lc = y->rc;
    if(rbnode->lc) rbnode->lc->p = rbnode;
    y->p = rbnode->p;
    if(y->p){
        if(y->p->lc == rbnode){
            y->p->lc = y;
        }else{
            y->p->rc = y;
        }
    }
    rbnode->p = y;
    y->rc = rbnode;
    return y;


}

template <class T>
typename RB_tree<T>::RB_node* RB_tree<T>::findv(T v,RB_node*& pre){
    RB_node* ptn = root;
    pre = nullptr;
    while(ptn && ptn->val != v){
        pre = ptn;
        if(v < ptn->val){
            ptn = ptn->lc;
        }else{
            ptn = ptn->rc;
        }
    }
    return ptn;
}

template <class T>
bool RB_tree<T>::init(T v){
    //cout<<"init"<<endl;
    if(root) return false;
    root = new RB_node(v);
    root->clr = BLK;
    _size = 1;
    return true;
}

template <class T>
bool RB_tree<T>::insert_val(T val){
    RB_node* pre = nullptr;
    if(findv(val,pre)){
        return false;
    }
    if(!pre){
        init(val);
    }else{
        _size++;
        //cout<<"s"<<_size<<endl;
        RB_node* rb = new RB_node(val,pre);
        if(val < pre->val){
            pre->lc = rb;
        }else{
            pre->rc = rb;
        }
        RB_node* ptn = rb;
        while(ptn->p && ptn->p->clr == RED){
            pre = ptn->p;
            if(bro(pre)&&bro(pre)->clr == RED){ // uncle is red
                pre->clr = BLK;
                bro(pre)->clr = BLK;
                pre->p->clr = RED;
                ptn = pre->p;
            }else{
                if(pre == pre->p->lc){
                    if(ptn == pre->rc){ // red on the r-child
                        ptn = ptn->p;
                        left_rotate(ptn);
                    }
                    ptn->p->clr = BLK;  // red on the l-child
                    ptn->p->p->clr = RED;
                    right_rotate(ptn->p->p);
                }else{
                    if(ptn == pre->lc){
                        ptn = ptn->p;
                        right_rotate(ptn);
                    }
                    ptn->p->clr = BLK;
                    ptn->p->p->clr = RED;
                    left_rotate(ptn->p->p);
                }
            }
        }
        root->clr = BLK;
    }
    return true;
}

template <class T>
bool RB_tree<T>::search_val(T val){
    RB_node* pre = nullptr;
    if(!findv(val,pre)){
        return false;
    }
    return true;
}

template <class T>
void RB_tree<T>::solve_lost_black(RB_node* rbnode){
    RB_node* ptn = rbnode;
    RB_node* br = nullptr;
    while(ptn != root && ptn->clr == BLK){
        br = bro(ptn);
        if(br&&br->clr == RED){ // case 1
            br->clr = BLK;
            ptn->p->clr = RED;
            if(ptn == ptn->p->lc){
                left_rotate(ptn->p);
            }else{
                right_rotate(ptn->p);
            }
            br = bro(ptn);
        }
        if(!br||((!br->lc || br->lc->clr == BLK) && (!br->rc || br->rc->clr == BLK))){ //case 2
            if(br) br->clr = RED;
            ptn = ptn->p;
        }else{
            if(ptn == ptn->p->lc){
                if(br->lc && br->lc->clr == RED){ // case 3
                    br->clr = RED;
                    br->lc->clr = BLK;
                    right_rotate(br);
                    br = br->p;
                }
                br->clr = ptn->p->clr;   // case 4
                ptn->p->clr = BLK;
                if(br->rc) br->rc->clr = BLK;
                left_rotate(ptn->p);
            }else{
                if(br->rc && br->rc->clr == RED){ // case 3
                    br->clr = RED;
                    br->rc->clr = BLK;
                    left_rotate(br);
                    br = br->p;
                }
                br->clr = ptn->p->clr;
                ptn->p->clr = BLK;
                if(br->lc) br->lc->clr = BLK;
                right_rotate(ptn->p);
            }
            ptn = root;
        }
    }
    if(ptn)
        ptn->clr = BLK;
}


template <class T>
bool RB_tree<T>::delete_val(T val){
    RB_node* pre = nullptr;
    RB_node* ptn = findv(val,pre);
    if(!ptn){
        return false;
    }
    while(ptn->lc||ptn->rc){
        pre = ptn;
        if(ptn->rc){
            if(!ptn->lc){
                assert(ptn->clr == BLK);
            }
            ptn = ptn->right();

        }else if(ptn->lc){
            assert(ptn->clr == BLK);
            ptn = ptn->left();
        }
        pre->val = ptn->val;
    }
    if(ptn->clr == BLK){
        solve_lost_black(ptn);
    }
    if(ptn->p){
        if(ptn == ptn->p->lc)
            ptn->p->lc = nullptr;
        else
            ptn->p->rc = nullptr;
    }else{
        root = nullptr;
    }
    _size--;
    delete ptn;
    return true;
}

template <class T>
void RB_tree<T>::delete_t(RB_node *r){
    if(!r) return;
    delete_t(r->lc);
    delete_t(r->rc);
    delete r;
    r = nullptr;
}


template <class T>
void RB_tree<T>::clear_tr(){
    delete_t(root);
    _size = 0;
}

template <class T>
void RB_tree<T>::print_t(){
    if(!root){
        cout<<"empty print"<<endl;
        return;
    }
    queue<RB_node*> q;
    q.push(root);
    RB_node* ptn;
    while(!q.empty()){
        ptn = q.front();
        q.pop();

        cout<<"color: " <<ptn->clr<<"  val: "<<ptn->val<<"   ";
        if(ptn != root) {
            if(ptn == ptn->p->lc) {
                cout<<"lc";
                assert(ptn->p->val > ptn->val);
            }else {
                cout<<"rc";
                assert(ptn->p->val > ptn->val);
            }
            cout<<"   parent:"<< ptn->p->val<<endl;
        }
        else cout<<endl;
        if(ptn->lc) q.push(ptn->lc);
        if(ptn->rc) q.push(ptn->rc);
    }
}

template <class T>
int RB_tree<T>::tsize(){
    return _size;
}



inline void Test1(){
    RB_tree<int>* rbt = new RB_tree<int>();
    int ran;
    cout<<"================= Random insert and search all ==================="<<endl;
    // Random insert
    vector<int> vv;
    const int ini_size = 6000;
    cout << "Insert "<<ini_size<<" random numbers from 1-10000"<<endl;
    for(int i = 0;i < ini_size; i++){
        ran = rand()%10000;
       // cout<<i<<","<<ran<<endl;
        rbt->insert_val(ran);
        vv.push_back(ran);
    }
    //rbt->print_t();
    cout<<"tree size: " <<rbt->tsize()<<endl;

    //Delete all
    int cnts = 0,cntf = 0;

    //Search all
    cnts = 0;cntf = 0;
    for(int i : vv){
        if(rbt->search_val(i)){
            //cout<<"?"<<i<<endl;
            cnts++;
        }else{
            cntf++;
        }
    }
    cout<<"search success "<<cnts<<"  failed  "<<cntf<<endl;

    cout<<"================= delete all ==================="<<endl;
    //Delete all
    cnts = 0;cntf = 0;
    for(int i : vv){
        //cout<<"delete "<<i<<endl;
        //rbt->print_t();
        if(rbt->delete_val(i)){
            cnts++;
        }else{
            //cout<<"delete failed  "<<i<<endl;
            cntf++;
        }

    }
    cout<<"delete success "<<cnts<<"  failed  "<<cntf<<endl;
    cout<<"tree size: " <<rbt->tsize()<<endl;

    //Search after delete
    cout<<"================= search after delete ==================="<<endl;
    cnts = 0;cntf = 0;
    for(int i : vv){
        if(rbt->search_val(i)){
            //cout<<"?"<<i<<endl;
            cnts++;
        }else{
            cntf++;
        }
    }
    cout<<"search success "<<cnts<<"  failed  "<<cntf<<endl;

    cout<<"================= single node test ==================="<<endl;
    cout<<"tree size: "<< rbt->tsize()<<endl;
    cout<<"insert 33: "<< rbt->insert_val(33)<<endl;
    cout<<"tree size: "<< rbt->tsize()<<endl;
    cout<<"search 33: "<< rbt->search_val(33)<<endl;
    cout<<"insert 33: "<< rbt->insert_val(33)<<endl;
    cout<<"delete 33: "<< rbt->delete_val(33)<<endl;
    cout<<"search 33: "<< rbt->search_val(33)<<endl;


    cout<<"tree size: "<<rbt->tsize()<<endl;
    delete rbt;
    //rbt.clear_tr();
}

int main()
{
    Test1();
    return 0;
}
