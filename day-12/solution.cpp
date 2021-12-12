#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>
#include <queue>
#include <unordered_map>

#define USE_EXAMPLE 0
#define P1 false
#define P2 true
#if USE_EXAMPLE
    #define FILE "./example.txt"
    #define P1_EXPECT 10
    #define P2_EXPECT 36
#else
    #define FILE "./input.txt"
    #define P1_EXPECT 4413
    #define P2_EXPECT 118803
#endif

void split(std::string str, std::string splitBy, std::vector<std::string>& tokens)
{
    tokens.push_back(str);

    size_t splitAt;
    size_t splitLen = splitBy.size();
    std::string frag;

    while (true)
    {
        frag = tokens.back();
        splitAt = frag.find(splitBy);
        if (splitAt == std::string::npos)
        {
            break;
        }
        tokens.back() = frag.substr(0, splitAt);
        tokens.push_back(frag.substr(splitAt + splitLen, frag.size() - (splitAt + splitLen)));
    }
}

struct node
{
    std::string id = "";
    bool isSmall = false;

    node(std::string id)
        : id{ id }
    {
        isSmall = islower(id[0]);
    }
    bool operator<(const node& other) const { return id.compare(other.id); }
};

struct edge
{
    node* start = nullptr;
    node* end = nullptr;
};

struct graph
{
    std::vector<node*> nodes;
    std::vector<edge*> edges;
};

std::unordered_map<std::string, std::vector<node*>*> out_nodes;

void insert_node(graph* cave_graph, node* _node)
{
    for (node* n : cave_graph->nodes)
    {
        if (n->id == _node->id)
        {
            return;
        }
    }
    cave_graph->nodes.push_back(_node);
}

void find_start_end(graph* cave_graph, node*& out_start, node*& out_end)
{
    for (node* n : cave_graph->nodes)
    {
        if (n->id == "start")
        {
            out_start = n;
        }
        else if (n->id == "end")
        {
            out_end = n;
        }
    }
}

typedef std::vector<node*> path;

std::vector<node*> get_out_nodes(graph* cave_graph, node* _node)
{
    std::vector<node*> out_nodes;
    for (edge* e : cave_graph->edges)
    {
        if (e->start->id == _node->id && e->end->id != "start")
        {
            out_nodes.push_back(e->end);
        }
    }
    return out_nodes;
}

bool has_path_used_twice(path* p)
{
    for (int i = 0; i < p->size(); ++i)
    {
        for (int j = i+1; j < p->size(); ++j)
        {
            node* a = p->at(i);
            node* b = p->at(j);
            if (a->isSmall && b->isSmall && a->id == b->id)
            {
                return true;
            }
        }
    }
    return false;
}

std::vector<path*> get_possible_moves(graph* cave_graph, path _path, bool allow_twice_small)
{
    size_t s = _path.size() - 1;
    node* last_node = _path[s];

    std::vector<path*> new_paths;

    std::vector<node*> out_nodes = get_out_nodes(cave_graph, last_node);
    for (node* out_node : out_nodes)
    {
        if (out_node->isSmall)
        {
            bool small_cave_already_visited = false;
            for (node* k : _path)
            {
                if (k->id == out_node->id)
                {
                    small_cave_already_visited = true;
                    break;
                }
            }

            if (!small_cave_already_visited)
            {
                path* new_path = new path(_path);
                new_path->push_back(out_node);
                new_paths.push_back(new_path);
            }
            else if (allow_twice_small)
            {
                if (!has_path_used_twice(&_path))
                {
                    path* new_path = new path(_path);
                    new_path->push_back(out_node);
                    new_paths.push_back(new_path);
                }
            }
        }
        else
        {
            path* new_path = new path(_path);
            new_path->push_back(out_node);
            new_paths.push_back(new_path);
        }
        
    }

    return new_paths;
}

void dump_path(path* _path)
{
    for (int i = 0; i < _path->size()-1; ++i)
    {
        printf("%s -> ", _path->at(i)->id.c_str());
    }
    printf("%s\n", _path->at(_path->size()-1)->id.c_str());
}

int do_part(bool twice_small_cave)
{
    std::fstream input(FILE);
    graph cave_graph;

    std::string line;
    while (input >> line)
    {
        std::vector<std::string> results;
        split(line, "-", results);
        
        node* start_node = new node(results[0]);
        insert_node(&cave_graph, start_node);

        node* end_node = new node(results[1]);
        insert_node(&cave_graph, end_node);

        edge* new_edge = new edge();
        new_edge->start = start_node;
        new_edge->end = end_node;

        edge* new_edge2 = new edge();
        new_edge2->start = end_node;
        new_edge2->end = start_node;

        cave_graph.edges.push_back(new_edge);
        cave_graph.edges.push_back(new_edge2);
    }

    node* start = nullptr;
    node* end = nullptr;
    find_start_end(&cave_graph, start, end);

    std::vector<std::string> small_caves_visited;
    
    path* p = new path();
    p->push_back(start);

    std::queue<path*> all_paths;
    all_paths.push(p);

    int path_count = 0;
    while (!all_paths.empty())
    {
        path* _path = all_paths.front();
        all_paths.pop();

        //dump_path(_path);
        //printf("=>\n");
        std::vector<path*> possible_paths = get_possible_moves(&cave_graph, *_path, twice_small_cave);
        for (path* pp : possible_paths)
        {
            //dump_path(pp);
            if (pp->at(pp->size() - 1)->id == "end")
            {
                ++path_count;
            }
            else
            {
                all_paths.push(pp);
            }
        }
        //printf("\n\n");
    }

    printf("%d\n", path_count);
    return path_count;
}


int main()
{
    assert(do_part(P1) == P1_EXPECT);
    assert(do_part(P2) == P2_EXPECT);
}