#include <bits/stdc++.h>

using namespace std;

namespace Huffman {
struct Node {
 char data;
 shared_ptr<Node> left;
 shared_ptr<Node> right;

 Node(char d) : data(d), left(nullptr), right(nullptr) {}
 Node() : data('\0'), left(nullptr), right(nullptr) {}
};

int index = 0;
shared_ptr<Node> rebuildHuffmanTree(const string& s) {
 if (index >= s.length()) return nullptr;

 if (s[index] == '1') {
     index++; // 跳过'1'
     char ch = s[index++]; // 读取字符
     return make_shared<Node>(ch);
 } else if (s[index] == '0') {
     index++; // 跳过'0'
     auto node = make_shared<Node>();
     node->left = rebuildHuffmanTree(s);
     node->right = rebuildHuffmanTree(s);
     return node;
 }
 return nullptr;
}

// 辅助函数：打印树结构（前序遍历）
void printTree(const shared_ptr<Node>& root, string prefix = "") {
 if (!root) return;

 if (root->data != '\0') {
     cout << prefix << "Leaf: " << root->data << endl;
 } else {
     cout << prefix << "Internal Node" << endl;
 }
 printTree(root->left, prefix + "  ");
 printTree(root->right, prefix + "  ");
}
}

const int N = 1010;
vector<pair<string, string>> A, B;
int n, S, D;
void print(pair<string, string> pr) {
    cout << pr.first << ": " << pr.second << '\n';
}
string init(auto root, string str) {
    if(str[0] != 'H') return str;
    str = str.substr(1);
    if(str[0] == 'H') return str;
    int extra_zero = str.back() - '0';
    str.pop_back(), str.pop_back();
    string ans, binary;
    for(char c : str) {
        int x = (isdigit(c) ? c - '0' : c - 'a' + 10);
        for(int i = 3; i >= 0; i --)
            binary.push_back('0' + (x >> i & 1));
    }
    while(extra_zero) binary.pop_back(), extra_zero --;
    auto u = root;
    for(int i = 0; i < binary.size(); i ++) {
        if(binary[i] == '0') u = u->left;
        else u = u->right;
        if(u->data != '\0') ans.push_back(u->data), u = root;
    }
    return ans;
}

int main() {
    cin >> S >> D; A.resize(S + 1);
    for(int i = 1; i <= S; i ++) 
        cin >> A[i].first >> A[i].second;
    string encodedTree; cin >> encodedTree;
    auto root = Huffman::rebuildHuffmanTree(encodedTree);
    cin >> n;
    while(n --) {
        int op; cin >> op;
        if(op == 1) {
            int i; cin >> i;
            if(i <= S) print(A[i]);
            else print(B[(int)B.size() - (i - S)]);
        } else {
            int i; cin >> i;
            string key, val;
            if(i == 0) cin >> key, key = init(root, key);
            else key = (i <= S ? A[i].first : B[(int)B.size() - (i - S)].first);
            cin >> val, val = init(root, val);
            print({key, val});
            if(op == 2) B.push_back({key, val});
        }
    }
    return 0;
}