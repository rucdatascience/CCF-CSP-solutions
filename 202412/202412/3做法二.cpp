#include <bits/stdc++.h>
#define IOS ios::sync_with_stdio(0), cin.tie(0)
#define int long long
#define endl '\n'
#define lowbit(x) ((x) & (-x))
#define all(s) s.begin(), s.end()
#define pii pair<int, int>
#define ls(x) (x << 1)
#define rs(x) (x << 1 | 1)

using namespace std;

template <class T>
inline void read(T &x) {
    x = 0;
    char c = getchar();
    bool f = 0;
    for (; !isdigit(c); c = getchar())
        f ^= (c == '-');
    for (; isdigit(c); c = getchar())
        x = (x << 3) + (x << 1) + (c ^ 48);
    x = f ? -x : x;
}

template <class T>
inline void write(T &x) {
    if (x < 0)
        putchar('-'), x = -x;
    if (x < 10)
        putchar(x + 48);
    else
        write(x / 10), putchar(x % 10 + 48);
}

const int MAX_CACHE_BLOCKS = 70000;

// 缓存系统的数据结构
set<int> cache_blocks[MAX_CACHE_BLOCKS];  // 每个缓存块中存储的内存块编号
set<pair<int,int>> lru[MAX_CACHE_BLOCKS];  // LRU，存储(时间戳, 内存块编号)
map<int, int> cnt;  // cnt[i] 表示内存块 i  最后一次被修改的时间
map<int, int> change;  // 内存块的修改标志
 int n, N, q;
/**
 * 处理缓存访问请求
 * @param cache_id 缓存块ID
 * @param memory_block 内存块编号
 * @param current_time 当前时间戳
 * @param is_write 是否为写操作
 */
void handleCacheAccess(int cache_id, int memory_block, int current_time, bool is_write) {
    // 情况1: 内存块不在缓存中且缓存块有空间
    if (!cache_blocks[cache_id].count(memory_block) && cache_blocks[cache_id].size() < n) {
        
        cache_blocks[cache_id].insert(memory_block);//
       
        lru[cache_id].insert({current_time, memory_block});
        
        cnt[memory_block]=current_time;
        
        cout << "0 " << memory_block << endl;
        
        if (is_write) {
            change[memory_block] = 1;  // 写操作标记为已修改
        }
    }
    // 情况2: 内存块不在缓存中且缓存块已满
    else if (!cache_blocks[cache_id].count(memory_block) && cache_blocks[cache_id].size() == n) {
        
        auto time=(*lru[cache_id].begin()).first;
        
        auto lru_block = (*lru[cache_id].begin()).second;// 最早被更新的块 
         
        cache_blocks[cache_id].erase(lru_block);
        
        lru[cache_id].erase({time,lru_block});
        // 如果被移除的内存块被修改过，需要写回
        if (change[lru_block] == 1) {
           
            cout << "1 " << lru_block << endl;

            change[lru_block] = 0;
        }
        
        // 插入新的内存块
        cache_blocks[cache_id].insert(memory_block);

        lru[cache_id].erase({cnt[memory_block],memory_block});//删除旧的记录

        lru[cache_id].insert({current_time, memory_block});//加入新的记录
        
        cnt[memory_block]=current_time;
        
        cout << "0 " << memory_block << endl;
        
        if (is_write) {
            change[memory_block] = 1;  // 写操作标记为已修改
        }
    }
    // 情况3: 内存块已经在缓存中
    else {

        lru[cache_id].erase({cnt[memory_block],memory_block});//删除旧的记录

        lru[cache_id].insert({current_time, memory_block});//加入新的记录
        
        cnt[memory_block]=current_time;

        if (is_write) {
            change[memory_block] = 1;  // 写操作标记为已修改
        }
    }
}

signed main() {
    IOS;
    cin >> n >> N >> q;
    
    int current_time = 0;
    
    while (q--) {
        current_time++;
        int operation_type, memory_block;
        cin >> operation_type >> memory_block;
        
        // 计算内存块对应的缓存块ID
        int cache_id = (memory_block / n) % N;
        
        // 根据操作类型调用统一的处理函数
        bool is_write = (operation_type == 1);
        handleCacheAccess(cache_id, memory_block, current_time, is_write);
    }
    
    return 0;
}