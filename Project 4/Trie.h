#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED

#include <string>
#include <vector>
#include <list>
#include <algorithm>

template<typename ValueType>
class Trie
{
public:
    Trie();
    ~Trie();
    void reset();
    void insert(const std::string& key, const ValueType& value);
    std::vector<ValueType> find(const std::string& key, bool exactMatchOnly) const;

      // C++11 syntax for preventing copying and assignment
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
private:
    
    struct Node {
        char letter;
        std::vector<ValueType> values;
        Node* children[256] = {};
    };
    
    std::vector<ValueType> findHelper(const std::string& key, bool exactMatchOnly, Node* root) const;
    void resetHelper(Node* root);
    
    Node* head;
};

template<typename ValueType>
Trie<ValueType>::Trie()
{
    head = new Node;
}

template<typename ValueType>
Trie<ValueType>::~Trie()
{
    reset();
    delete head;
}

template<typename ValueType>
void Trie<ValueType>::reset()
{
    resetHelper(head);
    head = new Node;
}

template<typename ValueType>
void Trie<ValueType>::insert(const std::string& key, const ValueType& value)
{
    Node* p = head;
    for(int i = 0; i < key.length(); i++) {
        if(p->children[key[i]] != NULL) {
            p = p->children[int(key[i])];
        }
        else {
            Node* n = new Node;
            n->letter = key[i];
            p->children[key[i]] = n;
            p = n;
        }
    }
    p->values.push_back(value);
}

template<typename ValueType>
std::vector<ValueType> Trie<ValueType>::find(const std::string& key, bool exactMatchOnly) const {
    return findHelper(key.substr(1), exactMatchOnly, head->children[key[0]]);
}


template<typename ValueType>
std::vector<ValueType> Trie<ValueType>::findHelper(const std::string& key, bool exactMatchOnly, Node* root) const {
    
    std::vector<ValueType> v;
    
    if(root == NULL) return v;
    
    if(key.size() == 0) {
        
        return root->values;
    }

    if(exactMatchOnly) {
        
        return findHelper(key.substr(1), true, root->children[key[0]]);
    }
    
    else {

        for(int i = 0; i < 256; i++) {
            
            if(key[0] == i) {
                
                std::vector<ValueType> a = findHelper(key.substr(1), false, root->children[i]);
                v.insert(v.end(), a.begin(), a.end());
            }
    
            else {
                std::vector<ValueType> a = findHelper(key.substr(1), true, root->children[i]);
                v.insert(v.end(), a.begin(), a.end());
            }
            
        
        }
    }
    return v;
}

template<typename ValueType>
void Trie<ValueType>::resetHelper(Node *root)
{
    Node* p = root;
    for(int i = 0; i < 256; i++) {
        if(root->children[i] != NULL) {
            resetHelper(root->children[i]);
        }
    }
    delete p;
}

#endif // TRIE_INCLUDED
